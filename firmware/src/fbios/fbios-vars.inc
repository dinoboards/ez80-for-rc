
	; entries in the fbios_vars table

FBIOS_MAX_STORAGE_UNITS EQU	(%50-%41+1)
FSTUN_SIZE	EQU	16
FSTUN_TSIZE	EQU	FSTUN_SIZE*FBIOS_MAX_STORAGE_UNITS

					; | SIZE        | TYPE                | Description
FB_DMA			EQU	0	; | 3           | uint24_t            | current active DMA address
FB_DISK 		EQU	3	; | 3           | uint8_t             | current active disk
FB_TRCK			EQU	6	; | 3           | uint16_t            | current active track
FB_SEC  		EQU	9	; | 3           | uint16_t            | current active sector
FB_OSLOADGUARD		EQU	12	; | 3           | uint16_t            | guard to prevent double loading of OS on cold boot
FB_BOOTDISK  		EQU	15	; | 3           | uint16_t            | os load location
FB_SYSLOC  		EQU	18	; | 3           | uint16_t            | os load location
FB_SYSEND		EQU	21	; | 3           | uint16_t            | os end location
FB_SYSENTRY		EQU	24	; | 3           | uint16_t            | os bios entry location
FB_SYSSECSIZE		EQU	27	; | 3           | uint8_t             | os bios entry location
FB_BOOTSEC		EQU	30	; | 3           | uint24_t            | LSB for OS loading
FB_CSTUN		EQU	33	; | 3           | fbios_storage_t*    | the current fb_storage_unit
FB_CACHE		EQU	36	; | 3           | fbios_disk_cache_t* | the current cached 512 hbios sector
FB_STUN			EQU	39	; | FSTUN_TSIZE | fbios_storage_t     | table of fbios drives mapped to hbios units


FS_HBIOS_UNIT		EQU	0			; /* 0: the hbios device index */
FS_SLICE		EQU	1			; /* 1: the specific slice for this disk mapping*/
FS_HBIOS_MEDIA_TYPE	EQU	2			; /* 2: the type of media detected by hbios */
FS_SECTORS_PER_TRACK	EQU	3 			; /* 3:*/
FS_DISK_REFERENCE	EQU	5			; /* 5: 16 bit address within the MBASE segment.  First 16 bit value is number of sectors per track */
FS_STARTING_SECTOR   	EQU	7			; /* 7: the starting sector number for the specific slice */
