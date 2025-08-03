#!/bin/bash

mkdir -p .

C_FILES=()
H_FILES=()
ASM_FILES=()

H_FILES+=(v99x8-super.h)
H_FILES+=(v99x8.h)

C_FILES+=(v99x8/v99x8.c)
C_FILES+=(v99x8/vdp_get_screen_height.c)
C_FILES+=(v99x8/vdp_get_screen_max_unique_colours.c)
C_FILES+=(v99x8/vdp_get_screen_width.c)
C_FILES+=(v99x8/vdp_init.c)
C_FILES+=(v99x8/vdp_set_graphic_4.c)
C_FILES+=(v99x8/vdp_set_graphic_5.c)
C_FILES+=(v99x8/vdp_set_graphic_6.c)
C_FILES+=(v99x8/vdp_set_graphic_7.c)
C_FILES+=(v99x8/vdp_set_graphic_mode.c)
C_FILES+=(v99x8/vdp_set_lines.c)
C_FILES+=(v99x8/vdp_set_palette.c)
C_FILES+=(v99x8/vdp_set_refresh.c)
C_FILES+=(v99x8/vdp_draw_line.c)

C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_mode.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_1.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_2.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_3.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_4.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_5.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_6.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_7.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_8.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_9.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_10.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_11.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_12.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_21.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_22.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_23.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_24.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_25.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_26.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_27.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_28.c)
C_FILES+=(v99x8-hdmi/vdp_set_super_graphic_26.c)
C_FILES+=(v99x8-hdmi/vdp_set_extended_palette.c)
C_FILES+=(v99x8-hdmi/vdp_set_remap.c)

ASM_FILES+=(v99x8/vdp_cmd_logical_move_vdp_to_vram.asm)
ASM_FILES+=(v99x8/vdp_cmd_logical_move_vram_to_vram.asm)
ASM_FILES+=(v99x8/vdp_cmd_vdp_to_vram.asm)
ASM_FILES+=(v99x8/vdp_cmd_move_vram_to_vram.asm)
ASM_FILES+=(v99x8/vdp_cmd_pset.asm)
ASM_FILES+=(v99x8/vdp_out_reg_int16.asm)
ASM_FILES+=(v99x8/vdp_get_status.asm)
ASM_FILES+=(v99x8/vdp_cmd_wait_completion.asm)
ASM_FILES+=(v99x8/_vdp_reg_write.asm)
ASM_FILES+=(v99x8/vdp_cmd_line.asm)

for item in "${H_FILES[@]}"; do
  cp -u ../../../../../ez80-clang/include/${item} ../../includes/
done

for item in "${C_FILES[@]}"; do
  cp -u ../../../../../ez80-clang/src/${item} .
done

for item in "${ASM_FILES[@]}"; do
  source_file="../../../../../ez80-clang/src/${item}"
  destination_file="./$(basename ${source_file})"

  sed -E \
    -e "1i\;\r\n; Generated from source-doc/${source_file} -- not to be modify directly\r\n;\r\n; " \
    -e '/SECTION IGNORE/d' \
    -e 's/.section\s+\.text.*$/\tSECTION CODE/g' \
    -e 's/\"src\/v99x8\/common.inc\"/\"common.inc\"/g' \
    -e 's/0x([0-9a-fA-F]+)/%\1/g' \
    -e 's/\$([0-9a-fA-F]+)/%\1/g' \
    "$source_file" > "$destination_file"

  touch -d "$(date -R -r "$source_file")" "$destination_file"
done
