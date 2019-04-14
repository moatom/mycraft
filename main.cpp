#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <array>

#include <object.hpp>

const GLchar *vertexSource = R"glsl(
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
  // vec3 tPosition = position + offset;
  gl_Position = proj * view * (model * vec4(position, 1.0) + vec4(offset,0.0f));
  // gl_Position = proj * view * model * vec4(position, 1.0);
  // gl_Position = model*vec4(tPosition, 1.0);
  Color = color;
  Texcoord = vec2(texcoord.x, 1.0 - texcoord.y);
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

template<int n>
constexpr auto makeOffsets()
{
  std::array<glm::vec3 , n*n> offsets{};
  for (int i=0; i<n; ++i) {
    for (int j=0; j<n; ++j) {
      offsets[n*i+j].x = j/5.0f;
      offsets[n*i+j].y = 0.0f;
      offsets[n*i+j].z = i/5.0f;
    }
  }
  return offsets;
}
const auto offsets = makeOffsets<20>();// will make sure how this works

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

  glEnable(GL_DEPTH_TEST);

  glfwSwapInterval(1);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  Object object(new Model(CUBE));
  // Object object(new Model(SQUARE));
  // Object object(new Model(CIRCLE));


  // making shaders
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

  GLuint insVBO;
  glGenBuffers(1, &insVBO);
  glBindBuffer(GL_ARRAY_BUFFER, insVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * offsets.size(), offsets.data(), GL_STATIC_DRAW);
  GLint offsAttrib = glGetAttribLocation(shaderProgram, "offset");
  glEnableVertexAttribArray(offsAttrib);
  glVertexAttribPointer(offsAttrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), reinterpret_cast<void*> (0));
  glVertexAttribDivisor(offsAttrib, 1);

  GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
  auto t_start = std::chrono::high_resolution_clock::now();

  glm::mat4 view = glm::lookAt(
    glm::vec3(7.0f, 2.0f, 7.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f)
  );
  GLint uniView = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

  glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
  GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
    model = glm::scale(model ,glm::vec3(0.1f));
    // model = glm::rotate(model, time * glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // translate here!
    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));// reflect the changes of the view matrix

    glDrawElementsInstanced(GL_TRIANGLES, object._model.ecount , GL_UNSIGNED_INT, 0, 20*20);
    // glDrawElements(GL_TRIANGLES, object._model.ecount , GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteBuffers(1, &insVBO);
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
  glfwTerminate();;
  return 0;
}
