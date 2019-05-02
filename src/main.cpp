// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ShaderManager.hpp>
#include <block/BlockManager.hpp>
#include <Canvas.hpp>
#include <util.hpp>

GLFWwindow* init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(800, 600, "mycraft", NULL, NULL);
  if (window == NULL) {
    std::cerr << "error!" << std::endl;
    exit(1);
  }

  glfwMakeContextCurrent(window);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_DEPTH_TEST);

  glfwSwapInterval(1);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  return window;
}

const GLfloat unit = 0.15f;// こういうのやめた方がいい。マクロで書くべき。
int main()
{
  GLFWwindow* window = init();

  ShaderManager sm;
  BlockManager bm;
  for (int i=0; i<20; ++i) {
    for (int j=0; j<20; ++j) {
      bm.addBlock(GRASS, BlockObject(), glm::vec3(2*j*unit, 0.0f, 2*i*unit));// IBO
    }
  }
  BlockRef br = bm.getBlockRef(GRASS);
  GLuint vao = sm.makeBOID(br.model.getCube(), br.model.getCubeIndex(), br.material.getImage());// VBO, EBO, TEX
  sm.attachObjects(vao, bm.getBlockOffsetsRef(GRASS));// IBO

  Canvas canvas;
  canvas.setModel(glm::scale(glm::mat4(1.0f), glm::vec3(unit)));
  canvas.setView();
  canvas.setProj();
  sm.setUniformM4f("model", canvas.getModelPtr());
  sm.setUniformM4f("view",  canvas.getViewPtr());
  sm.setUniformM4f("proj",  canvas.getProjPtr());


  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sm.render(vao);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;  
}