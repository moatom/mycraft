// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ShaderManager.hpp>
#include <block/BlockManager.hpp>
#include <vision/Canvas.hpp>
#include <vision/GameController.hpp>
// #include <util.hpp>

const int windowWidth = 1200;// 800"600
const int windowHeight = 900;
GLFWwindow* init();

int main()
{
  GLFWwindow* window = init();
  GameController controller;

  ShaderManager sm;
  BlockManager bm;
  BlockRef br = bm.getBlockRef(GRASS);
  GLuint vao = sm.makeBOID(br.model.getCube(), br.model.getCubeIndex(), br.material.getImage());// VBO, EBO, TEX
  
  for (int i=0; i<20; ++i) {
    for (int j=0; j<20; ++j) {
      bm.addBlock(GRASS, BlockObject(), glm::vec3(2*j*UNIT, 2*i*UNIT + 2*UNIT, 0.0f));// IBO
    }
  }
  for (int i=0; i<20; ++i) {
    for (int j=0; j<20; ++j) {
      bm.addBlock(GRASS, BlockObject(), glm::vec3(2*j*UNIT, 0.0f, 2*i*UNIT));// IBO
    }
  }
  sm.attachObjects(vao, bm.getBlockOffsetsRef(GRASS));// IBO

  Canvas canvas(windowWidth / windowHeight);
  canvas.setCallbackFunction(window);
  canvas.setModel(glm::scale(glm::mat4(1.0f), glm::vec3(UNIT)));
  canvas.setView();
  canvas.setProj();
  sm.setUniformM4f("model", canvas.getModelPtr());
  sm.setUniformM4f("view",  canvas.getViewPtr());
  sm.setUniformM4f("proj",  canvas.getProjPtr());

  // sm.attachObjects(vao, bm.getBlockOffsetsRef(GRASS));// IBO
  int cnt = 0;
  while (!glfwWindowShouldClose(window)) {
    // if (cnt == 10) {
    // }

    canvas.setTime(glfwGetTime());
    controller.keyInputsPoll(window);
    canvas.update(controller.getKeyInputs());

    canvas.setView();
    canvas.setProj();
    sm.setUniformM4f("view",  canvas.getViewPtr());
    sm.setUniformM4f("proj",  canvas.getProjPtr());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sm.render(vao);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  ++cnt;
  return 0;  
}

GLFWwindow* init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "mycraft", NULL, NULL);
  if (window == NULL) {
    std::cerr << "error!" << std::endl;
    exit(1);
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_DEPTH_TEST);

  glfwSwapInterval(1);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  return window;
}