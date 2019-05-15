#ifndef INCLUDED_INPUT_STRUCT_HPP
#define INCLUDED_INPUT_STRUCT_HPP

struct MouseManager {
  float dx, dy, last_x, last_y, mouse_dy;
};

struct KeyManager {
  bool w, a, s, d, z, x;
};

#endif