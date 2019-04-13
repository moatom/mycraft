#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <GL/glew.h>
#include <vector>
#include <SOIL.h>
#include <iostream>

#include <model.hpp>

constexpr float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
// constexpr float pixels[] = {
//     0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
// };

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
      // glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, tex);

      int width, height;
      unsigned char *image = SOIL_load_image("sample.png", &width, &height, 0, SOIL_LOAD_RGB);
      if (image == 0) {
        std::cerr << "image fault" << std::endl;
        std::cerr << SOIL_last_result() << std::endl;
      }
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      SOIL_free_image_data(image);

      glGenerateMipmap(GL_TEXTURE_2D);// call it before using texParams because those depend on it
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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