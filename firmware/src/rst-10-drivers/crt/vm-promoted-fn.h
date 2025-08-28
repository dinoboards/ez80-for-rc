

#ifndef __VM_PROMOTED_FN__
#define __VM_PROMOTED_FN__

// #define __FN_ON_HOST__

#if (defined(__ZDS__) && defined(__FN_ON_HOST__)) || (!defined(__ZDS__) && !defined(__FN_ON_HOST__))
#define ENABLE_FN_HOSTING
#endif

#if defined(__FN_ON_HOST__)

#ifdef __ZDS__
#include "../../q3vm/host/vm.h"
#include <stdint.h>
extern uint32_t dispatch_hosted_fn(vm_t *vm, uint8_t *args);
extern void     link_vm_globals(vm_t *vm);
extern uint8_t *_counter;
#define counter (*_counter)
#endif

#else
#define link_vm_globals(vm)
#define dispatch_hosted_fn(vm, args) (-1)
#endif

#endif
