#include <cpm.h>
#include <ez80-firmware-usb.h>
#include <ez80-firmware.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HID_GET_REPORT   0x01
#define HID_GET_IDLE     0x02
#define HID_GET_PROTOCOL 0x03
#define HID_SET_REPORT   0x09
#define HID_SET_IDLE     0x0A
#define HID_SET_PROTOCOL 0x0B

#define HID_BOOT_PROTOCOL   0x00
#define HID_REPORT_PROTOCOL 0x01

typedef uint8_t usb_device_t;
enum usb_device_e {
  USB_NOT_SUPPORTED   = 0,
  USB_IS_FLOPPY       = 1,
  USB_IS_MASS_STORAGE = 2,
  USB_IS_CDC          = 3,
  USB_IS_KEYBOARD     = 4,
  USB_IS_MOUSE        = 5,
  USB_IS_UNKNOWN      = 6,
  _USB_LAST_DEVICE_TYPE,
  USB_IS_HUB = 15
}; // 4 bits only

typedef struct {
  uint8_t toggle;
  uint8_t number;
  uint8_t max_packet_size; // TODO: change this to uint8_t and also in RomWBW & MSX version
} endpoint_param_t;

typedef struct {
  uint8_t          address;
  uint8_t          max_packet_size;
  uint8_t          interface_number;
  endpoint_param_t endpoints[1]; // Isochronous
} device_config_mouse_t;

device_config_mouse_t device_config_mouse = {0};

const setup_packet_t cmd_hid_set = {0x21, HID_SET_PROTOCOL, {0, 0}, {0, 0}, 0};

usb_error_t hid_set_protocol(const device_config_mouse_t *const dev, const uint8_t protocol) {
  setup_packet_t cmd;
  cmd = cmd_hid_set;

  cmd.bRequest  = HID_SET_PROTOCOL;
  cmd.bValue[0] = protocol;

  return usb_control_transfer(&cmd, NULL, dev->address, dev->max_packet_size);
}

usb_error_t hid_set_idle(const device_config_mouse_t *const dev, const uint8_t duration) {
  setup_packet_t cmd;
  cmd = cmd_hid_set;

  cmd.bRequest  = HID_SET_IDLE;
  cmd.bValue[1] = duration;

  return usb_control_transfer(&cmd, NULL, dev->address, dev->max_packet_size);
}

const setup_packet_t cmd_get_device_descriptor = {0x80, 6, {0, 0}, {0, 0}, 8};

#define CHECK(fn)                                                                                                                  \
  {                                                                                                                                \
    result = fn;                                                                                                                   \
    if (result != USB_ERR_OK)                                                                                                      \
      goto done;                                                                                                                   \
  }

usb_error_t
usbtrn_get_string(uint8_t *const buffer, const uint8_t buf_length, const uint8_t device_address, const uint8_t str_index) {
  usb_error_t    result;
  setup_packet_t cmd;
  cmd = cmd_get_device_descriptor;

  // 1 get first langid
  cmd.wLength   = 4;
  cmd.bValue[1] = 3;
  CHECK(usb_control_transfer(&cmd, (uint8_t *)buffer, device_address, 8));

  const uint16_t lang_id = (uint16_t)buffer[2] + ((uint16_t)buffer[3] << 8);

  // 2 get length of string for langid
  cmd.wLength   = 2;
  cmd.bValue[0] = str_index;
  cmd.bIndex[0] = lang_id & 255;
  cmd.bIndex[1] = lang_id >> 8;
  CHECK(usb_control_transfer(&cmd, (uint8_t *)buffer, device_address, 8));

  uint8_t str_length = buffer[0];
  if (str_length > buf_length)
    str_length = buf_length;

  // 3 get string
  cmd.wLength = str_length;
  CHECK(usb_control_transfer(&cmd, (uint8_t *)buffer, device_address, 8));

  // 4 convert to ascii
  uint8_t dest = 0;
  uint8_t src  = 2;
  for (uint8_t i = 0; i < str_length - 2; i++) {
    buffer[dest++] = buffer[src++];
    src++;
  }

  buffer[dest] = 0;

done:
  return result;
}

static char str_buffer[32];

const char *get_string(const uint8_t device_index, const uint8_t str_index) {
  if (str_index == 0)
    return "";

  const usb_error_t r = usbtrn_get_string((uint8_t *)str_buffer, 32, device_index, str_index);

  if (r == USB_ERR_OK)
    return str_buffer;

  return "?";
}

usb_error_t usbdev_dat_in_trnsfer_0(device_config_mouse_t *const device, uint8_t *const buffer, const uint8_t buffer_size) {
  usb_error_t             result;
  endpoint_param_t *const endpoint = &device->endpoints[0];

  result =
      usb_data_in_transfer(buffer, buffer_size, device->address, endpoint->number, endpoint->max_packet_size, &endpoint->toggle);

  // if (result == USB_ERR_STALL) {
  //   usbtrn_clear_endpoint_halt(endpoint->number, device->address, device->max_packet_size);
  //   endpoint->toggle = 0;
  //   return USB_ERR_STALL;
  // }

  return result;
}

void report_device_descriptor(const device_descriptor_t *const p, const uint8_t device_address) {
  printf("  length:             %d\n", p->bLength);
  printf("  bDescriptorType:    %d\n", p->bDescriptorType);
  printf("  bcdUSB:             0x%02X\n", p->bcdUSB);
  printf("  bDeviceClass:       0x%02X\n", p->bDeviceClass);
  printf("  bDeviceSubClass:    0x%02X\n", p->bDeviceSubClass);
  printf("  bDeviceProtocol:    0x%02X\n", p->bDeviceProtocol);

  if (p->bDeviceClass == 2)
    printf("  Description:        Communications and CDC Control\n");
  else if (p->bDeviceClass == 8 && (p->bDeviceSubClass == 6 || p->bDeviceSubClass == 5) && p->bDeviceProtocol == 80)
    printf("  Description:        Mass Storage\n");
  else if (p->bDeviceClass == 8 && p->bDeviceSubClass == 4 && p->bDeviceProtocol == 0)
    printf("  Description:        Floppy Disk\n");
  else if (p->bDeviceClass == 9 && p->bDeviceSubClass == 0 && p->bDeviceProtocol == 0)
    printf("  Description:        Hub\n");
  else if (p->bDeviceClass == 3 && p->bDeviceSubClass == 1 && p->bDeviceProtocol == 1) // Keyboard boot protocol
    printf("  Description:        HID Keyboard (boot mode)\n");
  else if (p->bDeviceClass == 3 && p->bDeviceSubClass == 1 && p->bDeviceProtocol == 2) // Mouse boot protocol
    printf("  Description:        HID Mouse (boot mode)\n");
  else if (p->bDeviceClass == 7)
    printf("  Description:        Printer\n");
  else if (p->bDeviceClass != 0)
    printf("  Description:        Other\n");

  printf("  bMaxPacketSize0:    %d\n", p->bMaxPacketSize0);
  printf("  idVendor:           0x%04X\n", p->idVendor);
  printf("  idProduct:          0x%04X\n", p->idProduct);
  printf("  bcdDevice:          0x%04X\n", p->bcdDevice);
  printf("  Manufacturer:       %s\n", get_string(device_address, p->iManufacturer));
  printf("  Product:            %s\n", get_string(device_address, p->iProduct));
  printf("  iSerialNumber:      %s\n", get_string(device_address, p->iSerialNumber));
  printf("  bNumConfigurations: %d\n", p->bNumConfigurations);
}

void report_device_configuration(const config_descriptor_t *const config) {
  printf("  Configuration: (%p)\n", config);
  printf("    length:              %d\n", config->bLength);
  printf("    bDescriptorType:     %d\n", config->bDescriptorType);
  printf("    wTotalLength:        %d\n", config->wTotalLength);
  printf("    bNumInterfaces:      %d\n", config->bNumInterfaces);
  printf("    bConfigurationvalue: %d\n", config->bConfigurationvalue);
  printf("    iConfiguration:      %d\n", config->iConfiguration);
  printf("    bmAttributes:        0x%02X\n", config->bmAttributes);
  printf("    bMaxPower:           %d\n", config->bMaxPower);
}

usb_device_t report_device_interface(const interface_descriptor_t *const interface) {
  usb_device_t result = USB_IS_UNKNOWN;

  printf("    Interface: (%p)\n", interface);
  printf("      bLength:            %d\n", interface->bLength);
  printf("      bDescriptorType:    %d\n", interface->bDescriptorType);
  printf("      bInterfaceNumber:   %d\n", interface->bInterfaceNumber);
  printf("      bAlternateSetting:  %d\n", interface->bAlternateSetting);
  printf("      bNumEndpoints:      %d\n", interface->bNumEndpoints);
  printf("      bInterfaceClass:    0x%02X\n", interface->bInterfaceClass);
  printf("      bInterfaceSubClass: 0x%02X\n", interface->bInterfaceSubClass);
  printf("      bInterfaceProtocol: 0x%02X\n", interface->bInterfaceProtocol);
  printf("      iInterface:         %d\n", interface->iInterface);

  if (interface->bInterfaceClass == 2)
    printf("      Description:        Communications and CDC Control\n");
  else if (interface->bInterfaceClass == 8 && (interface->bInterfaceSubClass == 6 || interface->bInterfaceSubClass == 5) &&
           interface->bInterfaceProtocol == 80)
    printf("      Description:        Mass Storage\n");
  else if (interface->bInterfaceClass == 8 && interface->bInterfaceSubClass == 4 && interface->bInterfaceProtocol == 0)
    printf("      Description:        Floppy Disk\n");
  else if (interface->bInterfaceClass == 9 && interface->bInterfaceSubClass == 0 && interface->bInterfaceProtocol == 0)
    printf("      Description:        Hub\n");
  else if (interface->bInterfaceClass == 3 && interface->bInterfaceSubClass == 1 &&
           interface->bInterfaceProtocol == 1) // Keyboard boot protocol
    printf("      Description:        HID Keyboard (boot mode)\n");
  else if (interface->bInterfaceClass == 3 && interface->bInterfaceSubClass == 1 &&
           interface->bInterfaceProtocol == 2) { // Mouse boot protocol
    printf("      Description:        HID Mouse (boot mode)\n");
    result = USB_IS_MOUSE;
  } else if (interface->bInterfaceClass == 7)
    printf("      Description:        Printer\n");
  else if (interface->bInterfaceClass != 0)
    printf("      Description:        Other\n");

  return result;
}

void report_device_endpoint(const endpoint_descriptor_t *const endpoint) {
  printf("      Endpoint: (%p)\n", endpoint);
  printf("        bLength:          %d\n", endpoint->bLength);
  printf("        bDescriptorType:  %d\n", endpoint->bDescriptorType);
  printf("        bEndpointAddress: 0x%02X\n", endpoint->bEndpointAddress);
  printf("        bmAttributes:     0x%02X\n", endpoint->bmAttributes);
  printf("        wMaxPacketSize:   %d\n", endpoint->wMaxPacketSize);
  printf("        bInterval:        %d\n", endpoint->bInterval);
}

typedef struct {
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint16_t bcdHID;
  uint8_t  bCountry;
  uint8_t  bNumDescriptors;
  uint8_t  bReportDescriptorType;
  uint16_t wReportDescriptorLength;
} hid_descriptor_t;

void report_hid_descriptor(const hid_descriptor_t *const hid) {
  printf("      HID Descriptor: (%p)\n", hid);
  printf("        bLength:                %d\n", hid->bLength);
  printf("        bDescriptorType:        %d\n", hid->bDescriptorType);
  printf("        bcdHID:                 0x%04X\n", hid->bcdHID);
  printf("        bCountry:               %d\n", hid->bCountry);
  printf("        bNumDescriptors:        %d\n", hid->bNumDescriptors);
  printf("        bReportDescriptorType:  %d\n", hid->bReportDescriptorType);
  printf("        wReportDescriptorLength:%d\n", hid->wReportDescriptorLength);
}

uint8_t buffer[512];

typedef struct {
  uint8_t buttons;
  int8_t  x;
  int8_t  y;
} mouse_report_t;

mouse_report_t mouse_report = {0};

typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
} usb_descriptor_t;

int main(/*const int argc, const char *argv[]*/) {
  usb_error_t         result;
  device_descriptor_t my_device_descriptor;
  int8_t              device_address = 1;

  do {
    memset(&my_device_descriptor, 0, sizeof(device_descriptor_t));
    printf("Device Address: %d\n", device_address);

    result = usbtrn_get_descriptor2(&my_device_descriptor, device_address);

    if (result != USB_ERR_OK) {
      printf("  ErrorCode: %d\n", result);
      break;
    }

    report_device_descriptor(&my_device_descriptor, device_address);

    for (uint8_t config_index = 0; config_index < my_device_descriptor.bNumConfigurations; config_index++) {
      memset(buffer, 0, sizeof(buffer));
      result = usbtrn_get_full_config_descriptor(config_index, device_address, my_device_descriptor.bMaxPacketSize0, 255, buffer);
      if (result) {
        printf("usbtrn_get_full_config_descriptor failed %d\n", result);
        continue;
      }

      uint8_t                         *p      = buffer;
      const config_descriptor_t *const config = (config_descriptor_t *)p;
      report_device_configuration(config);
      p += config->bLength;

      for (uint8_t interface_index = 0; interface_index < config->bNumInterfaces; interface_index++) {
        const interface_descriptor_t *const interface = (interface_descriptor_t *)p;
        p += interface->bLength;

        usb_device_t usb_device = report_device_interface(interface);

        hid_descriptor_t *xx = (hid_descriptor_t *)p;
        if (xx->bDescriptorType == 33) { // hid descriptor
          p += xx->bLength;
          report_hid_descriptor((hid_descriptor_t *)xx);
        }

        for (uint8_t endpoint_index = 0; endpoint_index < interface->bNumEndpoints; endpoint_index++) {
          const endpoint_descriptor_t *const endpoint = (endpoint_descriptor_t *)p;
          p += endpoint->bLength;

          if (usb_device == USB_IS_MOUSE && endpoint_index == 0) {
            device_config_mouse.address                      = device_address;
            device_config_mouse.interface_number             = interface_index;
            device_config_mouse.max_packet_size              = endpoint->wMaxPacketSize > 64 ? 64 : endpoint->wMaxPacketSize;
            device_config_mouse.endpoints[0].number          = endpoint->bEndpointAddress;
            device_config_mouse.endpoints[0].max_packet_size = device_config_mouse.max_packet_size;
            device_config_mouse.endpoints[0].toggle          = device_config_mouse.max_packet_size;
          }

          report_device_endpoint(&endpoint[endpoint_index]);
        }
      }
    }

    device_address++;
  } while (true);

  printf("Captured Mouse??\n");
  printf("  address: %d\n", device_config_mouse.address);
  printf("  intf: %d\n", device_config_mouse.interface_number);
  printf("  maxps: %d\n", device_config_mouse.max_packet_size);
  printf("  ep: %d\n", device_config_mouse.endpoints[0].number);

  usb_error_t r = hid_set_protocol(&device_config_mouse, 0);
  printf("hid_set_protocol: %d\n", r);

  r = hid_set_idle(&device_config_mouse, 1);
  printf("hid_set_idle: %d\n", r);

  memset(buffer, 0, sizeof(buffer));

  // while (mouse_report.buttons != 1) {
  // r = usbdev_dat_in_trnsfer_0(&device_config_mouse, (uint8_t *)&mouse_report, sizeof(mouse_report));
  // // if (r == 0) {
  //   printf("usbdev_dat_in_trnsfer_0: %d (%x, (%d, %d))\n", r, mouse_report.buttons, mouse_report.x, mouse_report.y);
  //   // }
  // }
  return 0;
}
