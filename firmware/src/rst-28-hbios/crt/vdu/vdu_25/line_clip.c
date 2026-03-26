#include "../../vdu.h"
#include "../rst-28-vars.h"
#include <stdint.h>

static uint8_t bit_code(point_t p) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  uint8_t code = 0;

  if (p.x < vdu->gviewport.left)
    code |= 1; // left
  else if (p.x > vdu->gviewport.right)
    code |= 2; // right

  if (p.y < vdu->gviewport.bottom)
    code |= 4; // bottom
  else if (p.y > vdu->gviewport.top)
    code |= 8; // top

  return code;
}

static point_t intersect(line_t l, uint8_t edge) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  if (edge & 8)
    return point_new(l.a.x + (l.b.x - l.a.x) * (vdu->gviewport.top - l.a.y) / (l.b.y - l.a.y), vdu->gviewport.top);

  if (edge & 4)
    return point_new(l.a.x + (l.b.x - l.a.x) * (vdu->gviewport.bottom - l.a.y) / (l.b.y - l.a.y), vdu->gviewport.bottom);

  if (edge & 2)
    return point_new(vdu->gviewport.right, l.a.y + (l.b.y - l.a.y) * (vdu->gviewport.right - l.a.x) / (l.b.x - l.a.x));

  if (edge & 1)
    return point_new(vdu->gviewport.left, l.a.y + (l.b.y - l.a.y) * (vdu->gviewport.left - l.a.x) / (l.b.x - l.a.x));

  return point_new(-1, -1); // will this happen?
}

bool line_clip(line_t *l) {
  vdu_vars_t *const vdu = &hbios_vars->vdu;

  uint8_t codeA = bit_code(l->a);
  uint8_t codeB = bit_code(l->b);

  while (true) {
    if (!(codeA | codeB)) // both points within viewport
      return true;

    if (codeA & codeB) // line does not intersect viewport
      return false;

    if (codeA) { // a outside, intersect with clip edge
      l->a  = intersect(*l, codeA);
      codeA = bit_code(l->a);
      continue;
    }

    if (codeB) { // b outside, intersect with clip edge
      l->b  = intersect(*l, codeB);
      codeB = bit_code(l->b);
    }
  }

  return true;
}
