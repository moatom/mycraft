#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <GL/glew.h>
#include <vector>

#include "model.hpp"


constexpr float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
constexpr float pixels[] = {
    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
};

class Object {
  private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint tex;
 
  public:
    Model _model;
    Object(Model *model):_model(*model)// メモリ管理が不適切でよろしくない？ちゃんと右辺値参照で処理してね
    {
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * _model.vcount, _model._vertices, GL_STATIC_DRAW);

      glGenBuffers(1, &ebo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _model.ecount, _model._elements, GL_STATIC_DRAW);

      // tex configuration
      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_2D, tex);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
      glGenerateMipmap(GL_TEXTURE_2D);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

      // tex loading
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
    }

    ~Object()
    {
      glDeleteBuffers(1, &vbo);
      glDeleteBuffers(1, &ebo);
      glDeleteTextures(1, &tex);
      glDeleteVertexArrays(1, &vao);
    }
};

#endif