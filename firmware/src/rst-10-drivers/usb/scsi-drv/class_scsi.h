#ifndef __CLASS_SCSI
#define __CLASS_SCSI

#include "../base-drv/protocol.h"
#include <stdbool.h>

typedef struct {
  uint8_t  dCBWSignature[4];
  uint16_t dCBWTag[2];
  uint32_t dCBWDataTransferLength;
  uint8_t  bmCBWFlags;
  uint8_t  bCBWLUN;
  uint8_t  bCBWCBLength;
} scsi_command_block_wrapper_t;

typedef struct {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t allocation_length;
  uint8_t reserved3;
  uint8_t pad[6];
} scsi_packet_request_sense_t;

typedef struct {
  scsi_command_block_wrapper_t cbw;
  scsi_packet_request_sense_t  request_sense;
} cbw_scsi_request_sense_t;

typedef struct {
  uint8_t operation_code;

  uint8_t IMMED : 1;
  uint8_t reserved : 7;

  uint8_t reserved2;

  uint8_t power_condition_modifier : 4;
  uint8_t reserved3 : 4;

  uint8_t start : 1;
  uint8_t loej : 1;
  uint8_t no_flush : 1;
  uint8_t reserved4 : 1;
  uint8_t power_condition : 4;

  uint8_t control;
} scsi_packet_eject_t;

typedef struct {
  scsi_command_block_wrapper_t cbw;
  scsi_packet_eject_t          eject;
} cbw_scsi_eject_t;

typedef struct {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t reserved3;
  uint8_t reserved4;
  uint8_t pad[6];
} scsi_packet_test_t;

typedef struct {
  scsi_command_block_wrapper_t cbw;
  scsi_packet_test_t           test;
} cbw_scsi_test_t;

typedef struct {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved[8];
  uint8_t pad[2];
} scsi_read_capacity_t;

typedef struct {
  scsi_command_block_wrapper_t cbw;
  scsi_read_capacity_t         read_capacity;
} cbw_scsi_read_capacity_t;

typedef struct __scsi_packet_inquiry { // contains information about a specific device
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t allocation_length;
  uint8_t reserved3;
  uint8_t pad[6];
} scsi_packet_inquiry_t;

typedef struct {
  scsi_command_block_wrapper_t cbw;
  scsi_packet_inquiry_t        inquiry;
} cbw_scsi_inquiry_t;

typedef struct {
  uint8_t device_type_and_qualifier;
  uint8_t removable_media; /* MSB only */
  uint8_t version;
  uint8_t byte_3;
  // uint8_t response_data_format : 4;
  // uint8_t hi_support : 1;
  // uint8_t norm_aca : 1;
  // uint8_t terminate_task : 1;
  // uint8_t aerc : 1;
  uint8_t additional_length;
  uint8_t reserved;
  uint8_t byte_6;
  // uint8_t addr16 : 1;
  // uint8_t addr32 : 1;
  // uint8_t ack_req_q : 1;
  // uint8_t medium_changer : 1;
  // uint8_t multi_port : 1;
  // uint8_t reserved_bit2 : 1;
  // uint8_t enclosure_services : 1;
  // uint8_t reserved_bit3 : 1;
  uint8_t byte_7;
  // uint8_t soft_reset : 1;
  // uint8_t command_queue : 1;
  // uint8_t transfer_disable : 1;
  // uint8_t linked_commands : 1;
  // uint8_t synchronous : 1;
  // uint8_t wide16_bit : 1;
  // uint8_t wide32_bit : 1;
  // uint8_t relative_addressing : 1;
  uint8_t vendor_information[8];
  uint8_t product_id[16];
  uint8_t product_revision[4];
  uint8_t vendor_specific[20];
  uint8_t reserved3[40];
} scsi_inquiry_result_t;

typedef struct __scsi_command_status_wrapper {
  uint8_t  dCSWSignature[4];
  uint16_t dCSWTag[2];
  uint8_t  dCSWDataResidue[4];
  uint8_t  bCSWStatus;
} scsi_command_status_wrapper_t;

typedef struct {
  uint8_t number_of_blocks[4];
  uint8_t block_size[4];
} scsi_read_capacity_result_t;

typedef struct {
  uint8_t error_code;
  uint8_t segment_number;
  uint8_t sense_key;
  uint8_t information[4];
  uint8_t additional_sense_length;
  uint8_t command_specific_information[4];
  uint8_t additional_sense_code;
  uint8_t additional_sense_code_qualifier;
  uint8_t field_replaceable_unit_code;
  uint8_t sense_key_specific[3];
} scsi_sense_result_t;

typedef struct {
  uint8_t operation_code;  /* 0 */
  uint8_t lun;             /* 1 */
  uint8_t lba[4];          /* 2-5  high-endian block number */
  uint8_t reserved1;       /* 6 */
  uint8_t transfer_len[2]; /* 7-8 high-endian in blocks of block_len (see scsi_capacity) */
  uint8_t reserved2;       /* 9 */
  uint8_t pad[2];          /* 10-11 */
} scsi_packet_read_write_t;

typedef struct {
  scsi_command_block_wrapper_t cbw;
  scsi_packet_read_write_t     scsi_cmd;
} cbw_scsi_read_write_t;

typedef struct {
  // scsi_command_block_wrapper_t cbw;
  uint8_t  cbw_dCBWSignature[4];
  uint16_t cbw_dCBWTag[2];
  uint32_t cbw_dCBWDataTransferLength;
  uint8_t  cbw_bmCBWFlags;
  uint8_t  cbw_bCBWLUN;
  uint8_t  cbw_bCBWCBLength;

  // scsi_packet_read_write_t     scsi_cmd;
  uint8_t scsi_cmd_operation_code;  /* 0 */
  uint8_t scsi_cmd_lun;             /* 1 */
  uint8_t scsi_cmd_lba[4];          /* 2-5  high-endian block number */
  uint8_t scsi_cmd_reserved1;       /* 6 */
  uint8_t scsi_cmd_transfer_len[2]; /* 7-8 high-endian in blocks of block_len (see scsi_capacity) */
  uint8_t scsi_cmd_reserved2;       /* 9 */
  uint8_t scsi_cmd_pad[2];          /* 10-11 */

} cbw_scsi_read_write_x_t;

extern scsi_command_block_wrapper_t scsi_command_block_wrapper;

extern usb_error_t do_scsi_cmd(device_config_storage_t *const      dev,
                               scsi_command_block_wrapper_t *const cbw,
                               void *const                         send_receive_buffer,
                               const bool                          send);

extern usb_error_t scsi_test(device_config_storage_t *const dev);

extern usb_error_t scsi_request_sense(device_config_storage_t *const dev, scsi_sense_result_t *const sens_result);

#endif
