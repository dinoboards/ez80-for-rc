#ifndef __EZ80_H
#define __EZ80_H

#include <defines.h>
#include <eZ80F92-extra.h>
#include <stdint.h>

/*
 *_port_out() - Output a byte to an I/O port
 * @port: the PORT_IO variable to be assigned
 * @value: the byte to be written to the port
 *
 * equivalent to port = value;
 */

#define port_out(port, value)                                                                                                      \
  { (*(XSFR)port) = value; }

extern void _port_out(const uint24_t data);

#define PORT_IO(x) (*(XSFR)(x))

#endif
