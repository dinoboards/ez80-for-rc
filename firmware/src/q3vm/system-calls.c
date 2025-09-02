#include "host/target-support.h"
#include "host/vm.h"
#include "includes/host-functions.h"
#include "includes/v99x8-super.h"
#include "includes/v99x8.h"
#include "src/system-calls-ids.h"
#include "vm-promoted-fn.h"
#include "vm_bytecode.h"

void print_string(const char *str);
int  putchar(int ch);

extern vm_t vm;

/*   VM HOST INTERFACE   */

#ifdef DEBUG_VM
/* Callback from the VM that something went wrong
 * @param[in] level Error id, see vmErrorCode_t definition.
 * @param[in] error Human readable error text. */
void Com_Error(vmErrorCode_t level) {
  // what to do if we have an error!
  //  fprintf(stderr, "Err (%i)\n", level);
  //  exit(level);
}
#endif

#define VRAM_SIZE         0x100000
#define FONT_8X8_STORED_Y (VRAM_SIZE - (8 * 256))

uint32_t systemCalls(vm_t *vm, uint8_t *args) {
  const int id = -1 - args[0];

  switch (id) {
  case SC_PRINT_STRING:
    print_string((const char *)VMA_PTR(3, vm));
    return 0;

  case SC_TRAP_ERROR:
    print_string((const char *)VMA_PTR(3, vm));
    return 0;

  case SC_MEMSET: {
    memset(VMA_PTR(3, vm), VMA_UINT24(6), VMA_UINT24(9));
    return VMA_UINT24(3);
  }

  case SC_MEMCPY: {
    memcpy(VMA_PTR(3, vm), VMA_PTR(6, vm), VMA_UINT24(9));
    return VMA_UINT24(3);
  }

  case SC_PUTCHAR: {
    putchar(VMA_UINT24(3));
    return 0;
  }

  case SC_VDP_CMD_LINE: {
    // OR can we do this?
    // POP retrun address
    // pop vm
    // pop first arg
    // push return address
    // jp vdp_cmd_line
    vdp_cmd_line(VMA_UINT24(3), VMA_UINT24(6), VMA_UINT24(9), VMA_UINT24(12), VMA_UINT24(15), VMA_UINT24(18), VMA_UINT24(21));
    return 0;
  }

  case SC_VDP_CMD_WAIT_COMPLETION: {
    vdp_cmd_wait_completion();
    return 0;
  }

  case SC_VDP_CMD_LOGICAL_MOVE_VDP_TO_VRAM: {
    vdp_cmd_logical_move_vdp_to_vram(VMA_UINT24(3), VMA_UINT24(6), VMA_UINT24(9), VMA_UINT24(12), VMA_UINT24(15), VMA_UINT24(18),
                                     VMA_UINT24(21));
    return 0;
  }

  case SC_VDP_CMD_PSET: {
    vdp_cmd_pset(VMA_UINT24(3), VMA_UINT24(6), VMA_UINT24(9), VMA_UINT24(12));
    return 0;
  }

  case SC_VDP_DRAW_LINE: {
    vdp_draw_line(VMA_UINT24(3), VMA_UINT24(6), VMA_UINT24(9), VMA_UINT24(12), VMA_UINT24(15), VMA_UINT24(18));
    return 0;
  }

  // extern void vdp_cpu_to_vram(const uint8_t *const source, screen_addr_t vdp_address, uint16_t length);
  case SC_VDP_CPU_TO_VRAM: {
    vdp_cpu_to_vram(VMA_PTR(3, vm), VMA_UINT24(6), VMA_UINT24(9));
    return 0;
  }

  // extern void vdp_cmd_move_linear_to_xy(
  //   screen_addr_t src_addr, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t direction, uint8_t operation);
  case SC_VDP_DRAW_CHAR: {
    screen_addr_t addr = FONT_8X8_STORED_Y + (VMA_UINT24(3) * 8);
    vdp_cmd_wait_completion();
    vdp_cmd_move_linear_to_xy(addr, VMA_UINT24(6), VMA_UINT24(9), 8, 8, DIX_RIGHT | DIY_DOWN, CMD_LOGIC_REMAP);
    return 0;
  }

  // extern void vdp_cmd_move_vram_to_vram(uint16_t x, uint16_t y, uint16_t to_x, uint16_t to_y, uint16_t width, uint16_t height,
  // uint8_t direction);
  case SC_VDP_CMD_MOVE_VRAM_TO_VRAM: {
    vdp_cmd_move_vram_to_vram(VMA_UINT24(3), VMA_UINT24(6), VMA_UINT24(9), VMA_UINT24(12), VMA_UINT24(15), VMA_UINT24(18),
                              VMA_UINT24(21));
    return 0;
  }

  // extern void vdp_cmd_vdp_to_vram(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t colour, uint8_t direction);
  case SC_VDP_CMD_VDP_TO_VRAM: {
    vdp_cmd_vdp_to_vram(VMA_UINT24(3), VMA_UINT24(6), VMA_UINT24(9), VMA_UINT24(12), VMA_UINT24(15), VMA_UINT24(18));
    return 0;
  }

  default:
    return dispatch_hosted_fn(vm, args);
  }
  return 0;
}
