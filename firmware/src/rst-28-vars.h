#ifndef __HBIOS_VARS
#define __HBIOS_VARS

#include <stdint.h>

/* HBIOS STATUS CODES */
#define ERR_OK     0  /* INVALID UNIT NUMBER*/
#define ERR_NOUNIT -4 /* INVALID UNIT NUMBER*/

// ;
// CHAR DEVICE IDS;
// ;
#define CIODEV_UART     0x00
#define CIODEV_ASCI     0x01
#define CIODEV_TERM     0x02
#define CIODEV_PRPCON   0x03
#define CIODEV_PPPCON   0x04
#define CIODEV_SIO      0x05
#define CIODEV_ACIA     0x06
#define CIODEV_PIO      0x07
#define CIODEV_UF       0x08
#define CIODEV_DUART    0x09
#define CIODEV_Z2U      0x0A
#define CIODEV_LPT      0x0B
#define CIODEV_ESPCON   0x0C
#define CIODEV_ESPSER   0x0D
#define CIODEV_SCON     0x0E
#define CIODEV_SSER     0x0F
#define CIODEV_EZ80UART 0x10
#define CIODEV_DLPSER   0x11
#define CIODEV_TSER     0x12
#define CIODEV_SCC      0x13

// ;
// ; MEDIA ID VALUES
// ;
#define MID_NONE  0
#define MID_MDROM 1
#define MID_MDRAM 2
#define MID_RF    3
#define MID_HD    4
#define MID_FD720 5
#define MID_FD144 6
#define MID_FD360 7
#define MID_FD120 8
#define MID_FD111 9
#define MID_HDNEW 10

// ;
// ; DISK DEVICE IDS
// ;
#define DIODEV_MD    0x00
#define DIODEV_FD    0x01
#define DIODEV_RF    0x02
#define DIODEV_IDE   0x03
#define DIODEV_ATAPI 0x04
#define DIODEV_PPIDE 0x05
#define DIODEV_SD    0x06
#define DIODEV_PRPSD 0x07
#define DIODEV_PPPSD 0x08
#define DIODEV_HDSK  0x09
#define DIODEV_PPA   0x0A
#define DIODEV_IMM   0x0B
#define DIODEV_SYQ   0x0C
#define DIODEV_CHUSB 0x0D
#define DIODEV_CHSD  0x0E
#define DIODEV_USB   0x0F
#define DIODEV_ESPSD 0x10
#define DIODEV_SCSI  0x11

typedef struct {
  uint8_t usb_dev_index;
} dio_scsi_disk_t;

typedef struct {
  uint8_t *store;
  uint24_t current_sector;
} dio_ram_disk_t;

typedef union {
  dio_scsi_disk_t scsi;
  dio_ram_disk_t  ram;
} dio_state_t;

extern void *usb_scsi_dioseek;
extern void *usb_scsi_dioread;
extern void *usb_scsi_diowrite;
extern void *usb_scsi_device;
extern void *usb_scsi_media;
extern void *usb_scsi_capacity;

extern void *ram_disk_dioseek;
extern void *ram_disk_dioread;
extern void *ram_disk_diowrite;
extern void *ram_disk_device;
extern void *ram_disk_media;
extern void *ram_disk_capacity;

typedef struct dio_fns_s {
  void *dio_status;   /* DISK STATUS*/
  void *dio_reset;    /* DISK RESET*/
  void *dio_seek;     /* DISK SEEK*/
  void *dio_read;     /* DISK READ SECTORS*/
  void *dio_write;    /* DISK WRITE SECTORS*/
  void *dio_verify;   /* DISK VERIFY SECTORS */
  void *dio_format;   /*  DISK FORMAT TRACK */
  void *dio_device;   /* DISK DEVICE INFO REPORT*/
  void *dio_media;    /* DISK MEDIA REPORT*/
  void *dio_defmed;   /* DEFINE DISK MEDIA*/
  void *dio_cap;      /* DISK CAPACITY REPORT*/
  void *dio_geometry; /* DISK GEOMETRY REPORT*/
} dio_fns_t;

typedef struct cio_fns_s {
  void *cio_in;     /* CHARACTER INPUT */
  void *cio_out;    /* CHARACTER OUTPUT */
  void *cio_ist;    /* CHARACTER INPUT STATUS */
  void *cio_ost;    /* CHARACTER OUTPUT STATUS */
  void *cio_init;   /* INIT/RESET DEVICE/LINE CONFIG */
  void *cio_query;  /* REPORT DEVICE/LINE CONFIG */
  void *cio_device; /* REPORT DEVICE INFO */
} cio_fns_t;

typedef struct {
  dio_fns_t  *funcs;       /* 3 BYTES */
  uint8_t     instance;    /* 1 BYTE */
  dio_state_t state;       /* 6 BYTES */
  uint8_t     reserved[6]; /* 6 BYTES */
} dio_driver_t;            /* sizeof(dio_driver_t) == 16 */

typedef union {
  uint8_t padding[6];
} cio_state_t;

typedef struct {
  cio_fns_t  *funcs;       /* 3 BYTES */
  uint8_t     instance;    /* 1 BYTE */
  cio_state_t state;       /* 6 BYTES */
  uint8_t     reserved[6]; /* 6 BYTES */
} cio_driver_t;            /* sizeof(cio_driver_t) == 16 */

#define MAX_HBIOS_DIO_INSTANCES 32
#define MAX_HBIOS_CIO_INSTANCES 2

typedef struct hbios_vars_s {
  uint8_t     *himem; /* pointer to start of allocated linear RAM */
  uint8_t      cio_count;
  uint8_t      dio_count;
  dio_driver_t dio_drivers[MAX_HBIOS_DIO_INSTANCES];
  cio_driver_t cio_drivers[MAX_HBIOS_CIO_INSTANCES];

} hbios_vars_t;

extern hbios_vars_t *hbios_vars;

uint8_t *sys_alloc(const uint24_t size, const uint24_t page_alignment);

/* C Wrapper helpers to marshall calls to HBIOS Funcs via RST 28 */

uint8_t  diodevice_getstatus(uint8_t);
uint8_t  dioseek(uint8_t, uint24_t lsb);
uint16_t dioread(const uint8_t disk_unit, uint8_t sector_count, const uint8_t *buffer);
uint16_t diowrite(const uint8_t disk_unit, uint8_t sector_count, const uint8_t *buffer);
uint16_t diomedia(const uint8_t disk_unit, const uint8_t flags);
uint24_t diocapacity_get_sectors(const uint8_t disk_unit);
uint8_t  diodevice_getattributes(uint8_t);
uint8_t  diodevice_getdriver(uint8_t);
uint8_t  diodevice_getnumber(uint8_t);
uint8_t  sysget_ciocount();
uint8_t  sysget_diocount();
uint8_t  ciodevice_getattributes(uint8_t);
uint8_t  ciodevice_getdriver(uint8_t);
uint8_t  ciodevice_getnumber(uint8_t);
uint24_t cioquery_get_baud_rate(uint8_t);
uint8_t  cioquery_get_line_control(uint8_t);

#endif
