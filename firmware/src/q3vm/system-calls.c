#include "host/target-support.h"
#include "host/vm.h"
#include "includes/host-functions.h"
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
    return;
  }

  default:
    return dispatch_hosted_fn(vm, args);
  }
  return 0;
}
