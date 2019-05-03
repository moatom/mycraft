#ifndef __SHADER_MANAGER_HPP__
#define __SHADER_MANAGER_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <unordered_map>
#include <array>
#include <vector>
#include <glm/glm.hpp>

#include "block/BlockStruct.hpp"
#include "util.hpp"

#define DEFUALT_IBO_SIZE 10000

constexpr GLchar *vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec4 color;
in vec2 texcoord;
in vec3 offset;

out vec4 Color;
out vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
  gl_Position = proj * view * (model * vec4(position, 1.0) + vec4(offset,0.0f));
  Color = color;
  Texcoord = vec2(texcoord.x, 1.0 - texcoord.y);
})glsl";

constexpr GLchar *fragmentSource = R"glsl(
#version 150 core
in vec4 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;

void main()
{
  outColor = texture(tex, Texcoord) * Color;
})glsl";

typedef struct {
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  GLuint IBO;
  GLuint TEX;
  int iboSize;
  int iboCapa=DEFUALT_IBO_SIZE;
} BOID;

class ShaderManager {
  private:
    GLuint ShaderProgram;
    GLuint vertexShader;
    GLuint fragmentShader;
    std::unordered_map<GLuint, BOID> BOIDs;

  public:
    ShaderManager()
    {
      vertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexShader, 1, &vertexSource, NULL);
      glCompileShader(vertexShader);

      fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
      glCompileShader(fragmentShader);

      GLint statusV, statusF;
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &statusV);
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &statusF);
      if (statusV != GL_TRUE || statusF != GL_TRUE) {
        std::cerr << "fault of compiling shaders!" << std::endl;
        char bufferV[512], bufferF[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, bufferV);
        glGetShaderInfoLog(fragmentShader, 512, NULL, bufferF);
        std::cerr << "vertex " << bufferV << "\nfragment " << bufferF << std::endl;
        exit(1);
      }

      ShaderProgram = glCreateProgram();
      glAttachShader(ShaderProgram, vertexShader);
      glAttachShader(ShaderProgram, fragmentShader);
      glBindFragDataLocation(ShaderProgram, 0, "outColor");
      glLinkProgram(ShaderProgram);
      glUseProgram(ShaderProgram);
    } ;

    ~ShaderManager()
    {
      for (auto boid : BOIDs) {// 共通していたらやばい
        glDeleteVertexArrays(1, &boid.second.VAO);
        glDeleteBuffers(1, &boid.second.VBO);
        glDeleteBuffers(1, &boid.second.EBO);
        glDeleteBuffers(1, &boid.second.IBO);
        glDeleteTextures(1, &boid.second.TEX);

      }
      glDeleteProgram(ShaderProgram);
      glDeleteShader(fragmentShader);
      glDeleteShader(vertexShader);
      
    }

    void setUniformM4f(const char* name, glm::f32 *valuePtr)
    {
      GLuint ID = glGetUniformLocation(ShaderProgram, name);
      glUniformMatrix4fv(ID, 1, GL_FALSE, valuePtr);
    }

    GLuint makeBOID(std::array<Vertex, 4*6> &cube, std::array<GLuint, 6*6> &cubeIndex, Image image)
    {
      GLuint vao, vbo, ebo, ibo, tex;
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * cube.size(), cube.data(), GL_STATIC_DRAW);
      // glBindBuffer(GL_ARRAY_BUFFER, 0);

      glGenBuffers(1, &ebo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * cubeIndex.size(), cubeIndex.data(), GL_STATIC_DRAW);
      // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
     
      glGenBuffers(1, &ibo);
      glBindBuffer(GL_ARRAY_BUFFER, ibo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * DEFUALT_IBO_SIZE, NULL, GL_STREAM_DRAW);//とりま大きめに
      // glBindBuffer(GL_ARRAY_BUFFER, 0);

      glGenTextures(1, &tex);
      // glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, tex);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.body);
      glGenerateMipmap(GL_TEXTURE_2D);// call it before using tex params because those depend on it
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      // glBindTexture(GL_TEXTURE_2D, 0);

      //--------------------------------------------------------

      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      #define SETATTRIB(name) ({\
        GLint attrib = glGetAttribLocation(ShaderProgram, #name);\
        glEnableVertexAttribArray(attrib);\
        glVertexAttribPointer(attrib, M_SIZE(Vertex, name)/ M_SIZE(Vertex, name[0]) , GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*> (offsetof(Vertex, name)));\
        })
      SETATTRIB(position);
      SETATTRIB(color);
      SETATTRIB(texcoord);
      #undef SETATTRIB

      GLint offsAttrib = glGetAttribLocation(ShaderProgram, "offset");
      glEnableVertexAttribArray(offsAttrib);
      glBindBuffer(GL_ARRAY_BUFFER, ibo);
      glVertexAttribPointer(offsAttrib, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*> (0));
      // glBindBuffer(GL_ARRAY_BUFFER, 0);
      glVertexAttribDivisor(offsAttrib, 1);

      // glBindBuffer(GL_ARRAY_BUFFER, vbo);
      // glBindVertexArray(0);
      
      BOIDs[vao] = BOID{vao, vbo, ebo, ibo, tex};
      return vao;
    }

    void attachObjects(GLuint vao, std::vector<glm::vec3> &offsets)
    {
      BOID &boid = BOIDs[vao];
      boid.iboSize = offsets.size();
      if (boid.iboCapa < boid.iboSize && boid.iboSize <= 2 * boid.iboCapa) {
        boid.iboCapa *= 2;
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * boid.iboCapa, NULL, GL_STREAM_DRAW);
      } else if (2 * boid.iboCapa < boid.iboSize) {
        boid.iboCapa = boid.iboSize;
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * boid.iboCapa, NULL, GL_STREAM_DRAW);
      }
      glBindBuffer(GL_ARRAY_BUFFER, boid.IBO);
      // glBufferSubData(GL_ARRAY_BUFFER,
      //                 OffsetLists[i].lastModified,
      //                 sizeof(glm::vec3) * OffsetLists[i].vector.size()-i,
      //                 OffsetLists[i].vector.data()+i);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * offsets.size(), offsets.data());
      // glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void render(GLuint vao)
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BOIDs[vao].EBO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BOIDs[vao].TEX);
        // インデックス数と、iboの数
        glDrawElementsInstanced(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0, BOIDs[vao].iboSize);
        glBindVertexArray(0);
    }
};
#endif