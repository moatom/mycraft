#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <cstddef>

#include "object.hpp"

const GLchar *vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec4 color;
in vec2 texcoord;
out vec4 Color;
out vec2 Texcoord;
void main()
{
  gl_Position = vec4(position, 1.0);
  Color = color;
  Texcoord = texcoord;
})glsl";

const GLchar *fragmentSource = R"glsl(
#version 150 core
in vec4 Color;
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D tex;
void main()
{
  outColor = texture(tex, Texcoord) * Color;
})glsl";

int main(int argc, char *argv[])
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
  GLFWwindow *window = glfwCreateWindow(800, 600, "mycraft", NULL, NULL);
  if (window == NULL) {
    std::cerr << "error!" << std::endl;
    exit(1);
  }
  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  glewInit();

  glfwSwapInterval(1);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Object object(new Model(SQUARE));
  Object object(new Model(CIRCLE));

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
    std::cerr << "vertex error:" << bufferV << "\nfragment error:" << bufferF << std::endl;
    exit(1);
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*> (offsetof(vertex, position)));
  
  GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colorAttrib);
  glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*> (offsetof(vertex, color)));
  
  GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
  glEnableVertexAttribArray(texAttrib);
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*> (offsetof(vertex, texcoord)));

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // glScalef(0.1f, 0.1f, 0.1f);

    glDrawElements(GL_TRIANGLES, object._model.ecount , GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
  glfwTerminate();;
  return 0;
}
