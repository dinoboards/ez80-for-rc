
static uint8_t bit_code(point_t p) {
  uint8_t code = 0;

  if (p.x < gviewport.left)
    code |= 1; // left
  else if (p.x > gviewport.right)
    code |= 2; // right

  if (p.y < gviewport.bottom)
    code |= 4; // bottom
  else if (p.y > gviewport.top)
    code |= 8; // top

  return code;
}

static point_t intersect(line_t l, uint8_t edge) {
  return edge & 8   ? point_new(l.a.x + (l.b.x - l.a.x) * (gviewport.top - l.a.y) / (l.b.y - l.a.y), gviewport.top)
         : edge & 4 ? point_new(l.a.x + (l.b.x - l.a.x) * (gviewport.bottom - l.a.y) / (l.b.y - l.a.y), gviewport.bottom)
         : edge & 2 ? point_new(gviewport.right, l.a.y + (l.b.y - l.a.y) * (gviewport.right - l.a.x) / (l.b.x - l.a.x))
         : edge & 1 ? point_new(gviewport.left, l.a.y + (l.b.y - l.a.y) * (gviewport.left - l.a.x) / (l.b.x - l.a.x))
                    : point_new(-1, -1); // will this happen?
}

static bool line_clip(line_t *l) {

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
