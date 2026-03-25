#include "../print-string.h"
#include "../rst-10-drivers/usb/base-drv/usb-base-drv.h"
#include "../rst-10-drivers/usb/scsi-drv/scsi_driver.h"
#include "../rst-10-drivers/usb/ufi-drv/class_ufi.h"
#include "../rst-28-vars.h"
#include "rst-28-c-helpers.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint16_t wait_for_state(const uint8_t loop_counter, uint8_t state, const uint8_t desired_state) {
  uint16_t r = state;
  uint8_t  i;

  for (i = 0; i < loop_counter; i++) {
    if (state == desired_state)
      break;

    if (i & 1)
      print_string("\b ");
    else
      print_string("\b*");

    r     = usb_init(state);
    state = r & 255;
  }

  return r;
}

extern const char ch376_driver_version[];

// there is a weird bug with the compiler - sometimes string literals containing
// a dollar sign -- the dollar sign is ignored!
const char comma_0_x_dollar[] = {' ', '0', 'x', 0};

static void ch376_usb_init() {
  uint8_t       state = 0;
  uint16_t      r;
  uint8_t       i;
  const uint8_t loop_counter = 5;

  print_string("    CH376: IO=0x");
  print_hex((uint8_t)&CH376_DATA_PORT);
  print_string(comma_0_x_dollar);
  print_hex((uint8_t)&CH376_COMMAND_PORT);
  print_string(comma_0_x_dollar);
  print_hex((uint8_t)&USB_MODULE_LEDS);
  print_string(" *");

  r     = wait_for_state(loop_counter, state, 1);
  state = r & 255;

  print_string("\bPRESENT (VER ");

  r     = usb_init(state);
  state = r & 255;
  if (state != 2) {
    print_string("\rCH376: ");
    print_string("VERSION FAILURE\r\n");
    return;
  }

  print_hex(r >> 8);
  print_string(")\r\n");

  print_string("      USB: *");

  r     = wait_for_state(loop_counter, state, 3);
  state = r & 255;

  if (state == 2) {
    print_string("\bDISCONNECTED");
    return;
  }

  print_string("\bCONNECTED");

  // enumerate....
  r     = usb_init(state);
  state = r & 255;

  for (i = 0; i < loop_counter; i++) {
    if (r >> 8 != 0)
      break;

    print_string(".");
    r     = usb_init(state);
    state = r & 255;
  }
}

static void convert_and_print_disk_size(uint32_t number_of_sectors) {
  if (number_of_sectors == 1474560 / 512) {
    print_string("1.44MB");
    return;
  }

  {
    const char *suffix = "KB";

    if (number_of_sectors <= 2880) {
      print_uint16(number_of_sectors / 2);
      print_string(suffix);
      return;
    }

    number_of_sectors /= 256;
    suffix = "MB";

    if (number_of_sectors >= 8192) {
      number_of_sectors /= 1024;
      suffix = "GB";
    }

    {
      const uint16_t a = (uint16_t)(number_of_sectors / 8);
      const uint16_t b = (uint16_t)(number_of_sectors % 8 * 100) / 8;

      print_uint16(a);

      if (b != 0) {
        print_string(".");
        print_uint16(b);
      }
      print_string(suffix);
    }
  }
}

uint8_t hbios_mount_ram_disk(uint8_t next_unit);
uint8_t hbios_mount_usb_scsi_storage_devices(uint8_t next_unit);
void    report_hbios_drivers();

void hbios_dio_init() {
  uint8_t next_unit = 0;

  memset(hbios_vars->dio_drivers, 0, sizeof(hbios_vars->dio_drivers));

  next_unit = hbios_mount_ram_disk(next_unit);
  next_unit = hbios_mount_usb_scsi_storage_devices(next_unit);

  report_hbios_drivers();
}

static void right_trim(char *buffer) {
  int i;
  for (i = strlen(buffer) - 1; i >= 0; i--)
    if (buffer[i] == ' ')
      buffer[i] = 0;
    else
      break;
}

static void print_partition_type(uint8_t type) {
  switch (type) {
  case 0x2E:
    print_string("CP/M Slices");
    return;

  case 0x01:
    print_string("FAT 12");
    return;

  case 0x05:
    print_string("Extended");
    return;

  case 0x06:
    print_string("16-bit FAT");
    return;

  default:
    printf("Other (%02X)", type);
    return;
  }
}

static const dio_fns_t dio_usb_scsi_fns = {NULL,
                                           NULL,
                                           &usb_scsi_dioseek,
                                           &usb_scsi_dioread,
                                           &usb_scsi_diowrite,
                                           NULL,
                                           NULL,
                                           &usb_scsi_device,
                                           &usb_scsi_media,
                                           NULL,
                                           &usb_scsi_capacity,
                                           NULL};

// call at boot time, to scan usb devices
// discover disks and their partitions
// and install into hbios_storage_units
uint8_t hbios_mount_usb_scsi_storage_devices(uint8_t next_unit) {
  uint8_t          dev_index;
  uint8_t          instance;
  device_config_t *dev;

  if (next_unit >= MAX_HBIOS_DIO_INSTANCES)
    return next_unit;

  ch376_usb_init();
  print_string("\r\n");

  instance = 1;

  for (dev_index = 1; dev_index < 6; dev_index++) {
    dev = usb_get_device_type(dev_index);
    if (dev->type == 255)
      return next_unit;

    print_string("            ");
    if (dev->type != USB_IS_MASS_STORAGE)
      continue;

    print_string("Mass Storage ");
    {
      scsi_read_capacity_result_t info;
      scsi_inquiry_result_t       response;
      uint32_t                    number_of_sectors;
      uint8_t                     buffer[512];

      usb_scsi_init(dev_index);
      memset(&response, 0, sizeof(scsi_inquiry_result_t));
      usb_scsi_inquiry(dev_index, &response);

      memset(buffer, 0, sizeof(buffer));
      memcpy(buffer, response.vendor_information, 8);
      right_trim((char *)buffer);
      print_string((const char *)buffer);
      print_string(", ");

      usb_scsi_read_capacity(dev_index, &info);
      number_of_sectors = convert_from_msb_first(info.number_of_blocks);
      convert_and_print_disk_size(number_of_sectors);
      print_string("\r\n");

      hbios_vars->dio_drivers[next_unit].funcs                    = &dio_usb_scsi_fns;
      hbios_vars->dio_drivers[next_unit].state.scsi.usb_dev_index = dev_index;
      hbios_vars->dio_drivers[next_unit].instance                 = instance++;

      next_unit++;
      if (next_unit >= MAX_HBIOS_DIO_INSTANCES)
        return next_unit;
    }
  }

  return next_unit;
}

static const dio_fns_t dio_ram_disk_fns = {NULL,
                                           NULL,
                                           &ram_disk_dioseek,
                                           &ram_disk_dioread,
                                           &ram_disk_diowrite,
                                           NULL,
                                           NULL,
                                           &ram_disk_device,
                                           &ram_disk_media,
                                           NULL,
                                           &ram_disk_capacity,
                                           NULL};

uint8_t hbios_mount_ram_disk(uint8_t next_unit) {
  uint8_t *ram_disk_store;

  if (next_unit >= MAX_HBIOS_DIO_INSTANCES)
    return next_unit;

  ram_disk_store = sys_alloc(512 * 1024, 64 * 1024);
  memset(ram_disk_store, 0xE5, 512 * 1024);

  // use SYS_ALLOC to allocate 512K of RAM

  hbios_vars->dio_drivers[next_unit].state.ram.store          = ram_disk_store;
  hbios_vars->dio_drivers[next_unit].state.ram.current_sector = 0;

  // TODO set all ram disk bytes to E5
  // Can we see if existing data there - from reboot?

  hbios_vars->dio_drivers[next_unit].funcs    = &dio_ram_disk_fns;
  hbios_vars->dio_drivers[next_unit].instance = 1;

  print_string(" RAM Disk: 512K\r\n");

  return next_unit + 1;
}

// TODO: move to a generic hbios driver reporter
void report_hbios_drivers() {
  uint8_t hbios_unit;
  char    buffer[32];

  print_string("\r\n"
               "Unit        Device      Type              Capacity/Mode\r\n"
               "----------  ----------  ----------------  --------------------\r\n");

  for (hbios_unit = 0; hbios_unit <= MAX_HBIOS_DIO_INSTANCES; hbios_unit++) {
    if (diodevice_getstatus(hbios_unit))
      break;

    sprintf(buffer, "%s%d", diodevice_getdriver_name(hbios_unit), diodevice_getnumber(hbios_unit));

    printf("Disk %-7d%-12s%-18s", hbios_unit, buffer, diodevice_getattributes_name(hbios_unit));

    convert_and_print_disk_size(diocapacity_get_sectors(hbios_unit));

    print_string("\r\n");
  }
}
