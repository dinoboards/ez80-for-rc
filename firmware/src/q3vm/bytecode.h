#ifndef __NOTQ3VM_SYMBOLS__
#define __NOTQ3VM_SYMBOLS__

#define Q3VM_FN_SET_BASE_REGISTERS                       (0x000000)
#define Q3VM_FN_VDP_SET_GRAPHIC_4                        (0x00005F)
#define Q3VM_FN_VDP_SET_GRAPHIC_7                        (0x000139)
#define Q3VM_FN_VDP_SET_LINES                            (0x000204)
#define Q3VM_FN_VDP_SET_REFRESH                          (0x000264)
#define Q3VM_FN_VDP_SET_SUPER_GRAPHIC_10                 (0x0002D1)
#define Q3VM_FN_VDP_SET_SUPER_GRAPHIC_1                  (0x00039E)
#define Q3VM_FN_VDP_SET_SUPER_GRAPHIC_21                 (0x00046C)
#define Q3VM_FN_VDP_SET_SUPER_GRAPHIC_26                 (0x00049B)
#define Q3VM_FN_VDP_SET_SUPER_GRAPHIC_2                  (0x0004CA)
#define Q3VM_FN_VDP_SET_SUPER_GRAPHIC_5                  (0x000598)
#define Q3VM_FN_PRINTF                                   (0x000666)
#define Q3VM_FN_STRLEN                                   (0x0006A3)
#define Q3VM_FN_ADDINT                                   (0x0006CF)
#define Q3VM_FN_ADDFLOAT                                 (0x000824)
#define Q3VM_FN_ADDSTRING                                (0x0009C8)
#define Q3VM_FN_VSPRINTF                                 (0x000AB3)
#define Q3VM_FN_VDP_CURRENT_MODE                         (0xFFFE00)

#define Q3VM_DATA_REGISTERS_MIRROR                       (0x00005C)
#define Q3VM_DATA_REGISTER_31_MIRROR                     (0x000068)
#define Q3VM_DATA__STACKBOTTOM                           (0x000069)

#endif
