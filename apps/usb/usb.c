#include <cpm.h>
#include <ez80-firmware-usb.h>
#include <ez80-firmware.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void report_device_descriptor(const device_descriptor_t *const p) {
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
  else if (p->bDeviceClass == 3 && p->bDeviceSubClass == 1 && p->bDeviceProtocol == 1) //Keyboard boot protocol
    printf("  Description:        HID Keyboard (boot mode)\n");
  else if (p->bDeviceClass == 3 && p->bDeviceSubClass == 1 && p->bDeviceProtocol == 2) //Mouse boot protocol
    printf("  Description:        HID Mouse (boot mode)\n");
  else if (p->bDeviceClass == 7)
    printf("  Description:        Printer\n");
  else if (p->bDeviceClass != 0)
    printf("  Description:        Other\n");

  printf("  bMaxPacketSize0:    %d\n", p->bMaxPacketSize0);
  printf("  idVendor:           0x%04X\n", p->idVendor);
  printf("  idProduct:          0x%04X\n", p->idProduct);
  printf("  bcdDevice:          0x%04X\n", p->bcdDevice);
  printf("  iManufacturer:      %d\n", p->iManufacturer);
  printf("  iProduct:           %d\n", p->iProduct);
  printf("  iSerialNumber:      %d\n", p->iSerialNumber);
  printf("  bNumConfigurations: %d\n", p->bNumConfigurations);
}

void report_device_configuration(const config_descriptor_t *const config) {
  printf("  Configuration:\n");
  printf("    length:              %d\n", config->bLength);
  printf("    bDescriptorType:     %d\n", config->bDescriptorType);
  printf("    wTotalLength:        %d\n", config->wTotalLength);
  printf("    bNumInterfaces:      %d\n", config->bNumInterfaces);
  printf("    bConfigurationvalue: %d\n", config->bConfigurationvalue);
  printf("    iConfiguration:      %d\n", config->iConfiguration);
  printf("    bmAttributes:        0x%02X\n", config->bmAttributes);
  printf("    bMaxPower:           %d\n", config->bMaxPower);
}

void report_device_interface(const interface_descriptor_t *const interface) {
  printf("    Interface:\n");
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
  else if (interface->bInterfaceClass == 3 && interface->bInterfaceSubClass == 1 && interface->bInterfaceProtocol == 1) //Keyboard boot protocol
    printf("      Description:        HID Keyboard (boot mode)\n");
  else if (interface->bInterfaceClass == 3 && interface->bInterfaceSubClass == 1 && interface->bInterfaceProtocol == 2) //Mouse boot protocol
    printf("      Description:        HID Mouse (boot mode)\n");
  else if (interface->bInterfaceClass == 7)
    printf("      Description:        Printer\n");
  else if (interface->bInterfaceClass != 0)
    printf("      Description:        Other\n");
}

void report_device_endpoint(const endpoint_descriptor_t *const endpoint) {
  printf("      Endpoint:\n");
  printf("        bLength:          %d\n", endpoint->bLength);
  printf("        bDescriptorType:  %d\n", endpoint->bDescriptorType);
  printf("        bEndpointAddress: 0x%02X\n", endpoint->bEndpointAddress);
  printf("        bmAttributes:     0x%02X\n", endpoint->bmAttributes);
  printf("        wMaxPacketSize:   %d\n", endpoint->wMaxPacketSize);
  printf("        bInterval:        %d\n", endpoint->bInterval);
}

uint8_t buffer[512];

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

    report_device_descriptor(&my_device_descriptor);

    for (uint8_t config_index = 0; config_index < my_device_descriptor.bNumConfigurations; config_index++) {
      memset(buffer, 0, sizeof(buffer));
      result = usbtrn_get_full_config_descriptor(config_index, device_address, my_device_descriptor.bMaxPacketSize0, 150, buffer);
      if (result) {
        printf("usbtrn_get_full_config_descriptor failed %d\n", result);
        continue;
      }

      const config_descriptor_t *const config = (config_descriptor_t *)buffer;
      report_device_configuration(config);

      for (uint8_t interface_index = 0; interface_index < config->bNumInterfaces; interface_index++) {
        const interface_descriptor_t *const interface =
            (interface_descriptor_t *)(buffer + sizeof(config_descriptor_t) + interface_index * sizeof(interface_descriptor_t));
        report_device_interface(interface);

        for (uint8_t endpoint_index = 0; endpoint_index < interface->bNumEndpoints; endpoint_index++) {
          const endpoint_descriptor_t *const endpoint = (endpoint_descriptor_t *)&interface[1];
          report_device_endpoint(&endpoint[endpoint_index]);
        }
      }
    }

    device_address++;
  } while (true);

  return 0;
}
