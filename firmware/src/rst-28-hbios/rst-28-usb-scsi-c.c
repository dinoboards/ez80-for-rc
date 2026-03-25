#include "../rst-10-drivers/usb/scsi-drv/class_scsi.h"
#include "../rst-10-drivers/usb/scsi-drv/scsi_driver.h"
#include "../rst-10-drivers/usb/ufi-drv/class_ufi.h"
#include "../rst-28-vars.h"
#include <stdint.h>
#include <string.h>

uint8_t usb_scsi_dio_seek(const dio_state_t *const state, const uint24_t lba) {
  usb_scsi_seek(state->scsi.usb_dev_index, lba);

  return 0;
}

uint16_t usb_scsi_dio_read(const dio_state_t *const state, uint8_t sector_count, uint8_t *buffer) {
  uint8_t sectors_read = 0;

  while (sector_count > 0) {
    usb_scsi_read(state->scsi.usb_dev_index, buffer);
    sector_count--;
    sectors_read++;
    buffer += 512;
  }

  return sectors_read << 8;
}

// TODO Are we incrementing the seek position after reads and writes???
uint16_t usb_scsi_dio_write(const dio_state_t *const state, uint8_t sector_count, uint8_t *buffer) {
  uint8_t sectors_written = 0;

  while (sector_count > 0) {
    usb_scsi_write(state->scsi.usb_dev_index, buffer);
    sector_count--;
    sectors_written++;
    buffer += 512;
  }

  return sectors_written << 8;
}

uint24_t usb_scsi_dio_capacity(const dio_state_t *const state) {
  scsi_read_capacity_result_t info;
  uint32_t                    number_of_sectors;

  usb_scsi_read_capacity(state->scsi.usb_dev_index, &info);

  number_of_sectors = convert_from_msb_first(info.number_of_blocks);

  return (number_of_sectors >= 0x1000000) ? 0xFFFFFF : number_of_sectors;
}

/* TODO Move ram disk functions to their own file */

uint8_t ram_disk_dio_seek(dio_state_t *const state, const uint24_t lba) {
  state->ram.current_sector = lba;

  return 0;
}

uint16_t ram_disk_dio_read(const dio_state_t *const state, const uint8_t sector_count, uint8_t *const buffer) {
  const uint8_t *const source = &state->ram.store[state->ram.current_sector * 512];
  const uint24_t       length = sector_count * 512;

  memcpy(buffer, source, length);

  return sector_count << 8;
}

uint16_t ram_disk_dio_write(const dio_state_t *const state, const uint8_t sector_count, uint8_t *const buffer) {
  const uint8_t *const source = &state->ram.store[state->ram.current_sector * 512];
  const uint24_t       length = sector_count * 512;

  memcpy(source, buffer, length);

  return sector_count << 8;
}
