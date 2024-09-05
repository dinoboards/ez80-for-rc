#pragma printf = "%0X %X %x %s %c %u %f %d %u %ld %lld %llu %lu"

#include "hex-record.h"
#include "flash-lib.h"
#include <ifl.h>
#include <stdio.h>

#define getch getchar

#define REC_TYPE_DATA              0x00
#define REC_TYPE_EOF               0x01
#define REC_TYPE_EXT_SEG_ADDR      0x02
#define REC_TYPE_START_SEG_ADDR    0x03
#define REC_TYPE_EXT_LINEAR_ADDR   0x04
#define REC_TYPE_START_LINEAR_ADDR 0x05

static uint8_t  data;
static uint8_t  length;
static uint8_t  record_type;
static uint8_t  check_sum;
static uint8_t  offset_high, offset_low;
static uint32_t offset;
static uint32_t base_addr = 0;

static uint8_t get_next_byte(FILE *file) {
  uint8_t Hi, Lo;
  uint8_t ch1, ch2;

  ch1 = fgetc(file);
  Hi  = ch1 - '0';
  if (Hi > 9) {
    Hi -= 7;
  }

  ch2 = fgetc(file);
  Lo  = ch2 - '0';
  if (Lo > 9) {
    Lo -= 7;
  }
  uint8_t r = ((Hi << 4) | Lo);

  printf("[%c%c %X]", ch1, ch2, r);
  return r;
}

static void find_record_start(FILE *file) __z88dk_fastcall {
  do {
    data = fgetc(file);
  } while (data != ':');
}

static void read_record_start(FILE *file) __z88dk_fastcall {
  length      = get_next_byte(file);
  offset_high = get_next_byte(file);
  offset_low  = get_next_byte(file);
  record_type = get_next_byte(file);
  check_sum   = length + offset_high + offset_low + record_type;

  printf("length: %x, offset_high: %x, offset_low: %x, record_type: %x, check_sum: %x\r\n", length, offset_high, offset_low,
         record_type, check_sum);
}

int8_t process_hex_record(FILE *file) __z88dk_fastcall {
  static int8_t status = ZFL_ERR_SUCCESS;

  while (1) {
    find_record_start(file);
    read_record_start(file);

    if (status != ZFL_ERR_SUCCESS) {
      if (record_type != REC_TYPE_EOF) {
        continue;
      }
    }

    switch (record_type) {
    case (REC_TYPE_DATA): {
      putchar('.');
      offset = base_addr | (offset_high << 8) | offset_low;

      while (length--) {
        data = get_next_byte(file);
        check_sum += data;
        if (offset < IFL_FLASH_SIZE_BYTES) {
          // status = IFL_Program((HANDLE)offset, &data, 1);
          status = ZFL_ERR_SUCCESS;
          printf("IFL_Program(%lX, %X) %X\r\n", offset, data, check_sum);
        }
        if (status) {
          printf("Program status %X\r\n", status);
          break;
        }
        offset++;
      }

      if (status == ZFL_ERR_SUCCESS) {
        uint8_t x = get_next_byte(file);
        check_sum += x;
        if (check_sum) {
          printf("<check_sum error> %X - %X\r\n", check_sum, x);
          status = ZFL_ERR_VERIFY;
        }
      }
      break;
    }

    case (REC_TYPE_EOF): {
      check_sum += get_next_byte(file);
      if (check_sum != 0) {
        status = ZFL_ERR_FAILURE;
      }
      return (status);
    }

    case (REC_TYPE_EXT_SEG_ADDR): {
      printf("Unexpected 16-bit Extended Segment Address Record\r\n");
      base_addr = (get_next_byte(file) << 12) + (get_next_byte(file) << 4);
      break;
    }

    case (REC_TYPE_START_SEG_ADDR): {
      get_next_byte(file);
      get_next_byte(file);
      printf("Ignore statusrt Segment Address Record\r\n");
      break;
    }

    case (REC_TYPE_EXT_LINEAR_ADDR): {
      if (get_next_byte(file) == 0) {
        base_addr = ((uint32_t)get_next_byte(file) << 16);
        printf("base_addr: %lX\r\n", base_addr);
      } else {
        printf("Invalid Address\r\n");
        status = ZFL_ERR_ADDRESS;
      }
      break;
    }

    case (REC_TYPE_START_LINEAR_ADDR): {
      get_next_byte(file);
      get_next_byte(file);
      get_next_byte(file);
      get_next_byte(file);
      printf("Ignore statusrt Linear Address Record\r\n");
      break;
    }

    default: {
      printf("Unknown Record record_type: %X\r\n", record_type);
      return (status);
    }
    }
  }
}
