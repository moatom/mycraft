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
    ShaderManager();
    ~ShaderManager();
    void setUniformM4f(const char*, glm::f32*);
    GLuint makeBOID(std::array<Vertex, 4*6>&, std::array<GLuint, 6*6>&, Image);
    void attachObjects(GLuint, std::vector<glm::vec3>&);
    void render(GLuint);

    
};
#endif