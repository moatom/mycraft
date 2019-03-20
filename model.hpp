#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <GL/glew.h>

// template<typename t>
// struct array {

// };

typedef struct vertex {
  GLfloat position[3];
  GLfloat color[4];
  GLfloat texcoord[2];
} vertex;

enum modelType {SQUARE, CIRCLE};

class Model {
  
  public:
    modelType id;
    const vertex *_vertices;
    const GLuint *_elements;
    unsigned int vcount;
    unsigned int ecount;

    Model(modelType);
};
#endif