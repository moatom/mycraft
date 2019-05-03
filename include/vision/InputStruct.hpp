#ifndef __INPUT_STRUCT_HPP__
#define __INPUT_STRUCT_HPP__

struct MouseManager {
  float dx, dy, last_x, last_y, mouse_dy;
};

struct KeyManager {
  bool w, a, s, d, z, x;
};

#endif