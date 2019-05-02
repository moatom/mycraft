#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <GL/glew.h>

#include "BlockStruct.hpp"
#include "../SOIL.h"

class Material {
  Image image;

  public:// spを取り除けばシェーダとの依存性がなくなって、BMとの繋がりが大きくなる。material
    Material()
    {
      image.body = SOIL_load_image("../resources/tex.png", &image.width, &image.height, 0, SOIL_LOAD_RGB);
      if (image.body == NULL) {
        std::cerr << "image fault" << std::endl;
        std::cerr << SOIL_last_result() << std::endl;
      }
    }

    ~Material()
    {
// error
// mycraft(36826,0x11b9e45c0) malloc: *** error for object 0x11278c000: pointer being freed was not allocated
// mycraft(36826,0x11b9e45c0) malloc: *** set a breakpoint in malloc_error_break to debug

      // SOIL_free_image_data(image.body);
    }       

    Image getImage()
    {
      return image;
    }

};

#endif