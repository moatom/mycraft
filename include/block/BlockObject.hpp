#ifndef __BLOCK_OBJECT_HPP__
#define __BLOCK_OBJECT_HPP__

#include <glm/glm.hpp>

#define UNIT 0.15f

class BlockObject {
  public:
   protected:
    glm::vec3 *Position;
    glm::vec3 velocity;// このフレームで進む（んだ）方向=draw前、update後のもの
    glm::vec3 Size;
  public:
    void setPosition(glm::vec3 *position) {Position = position;}
    glm::vec3 getPosition() {return *Position;}
    glm::vec3 getSize() {return Size;}
    BlockObject()
    {
      Size = glm::vec3(2*UNIT);
    }
};

#endif