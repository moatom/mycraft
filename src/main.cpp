#include <glm/glm.hpp>

#include <ShaderManager.hpp>
#include <block/BlockManager.hpp>
#include <vision/Canvas.hpp>
#include <vision/GameController.hpp>
#define DEBUG_MODE 1

const int windowWidth = 1200;
const int windowHeight = 900;
GLFWwindow* init();

constexpr glm::vec3 treeLeaf[] = {
  {-1.f, 3.f, 0.f}, {0.f, 3.f, 0.f}, {1.f, 3.f, 0.f}, {0.f, 3.f, 1.f}, {0.f, 3.f, -1.f},
  {-1.f, 2.f, 0.f},                  {1.f, 2.f, 0.f}, {0.f, 2.f, 1.f}, {0.f, 2.f, -1.f},
  {-2.f, 1.f, 0.f}, {-1.f, 1.f, 0.f},                  {1.f, 1.f, 0.f}, {2.f, 1.f, 0.f},\
  {-2.f, 1.f, 1.f}, {-1.f, 1.f, 1.f}, {0.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {2.f, 1.f, 1.f},\
                    {-1.f, 1.f, 2.f}, {0.f, 1.f, 2.f}, {1.f, 1.f, 2.f},\
  {-2.f, 1.f, -1.f},{-1.f, 1.f, -1.f},{0.f, 1.f, -1.f},{1.f, 1.f, -1.f},{2.f, 1.f, -1.f},\
                    {-1.f, 1.f, -2.f},{0.f, 1.f, -2.f},{1.f, 1.f, -2.f},
  {-2.f, 0.f, 0.f}, {-1.f, 0.f, 0.f},                  {1.f, 0.f, 0.f}, {2.f, 0.f, 0.f},\
  {-2.f, 0.f, 1.f}, {-1.f, 0.f, 1.f}, {0.f, 0.f, 1.f}, {1.f, 0.f, 1.f}, {2.f, 0.f, 1.f},\
                    {-1.f, 0.f, 2.f}, {0.f, 0.f, 2.f}, {1.f, 0.f, 2.f},\
  {-2.f, 0.f, -1.f},{-1.f, 0.f, -1.f},{0.f, 0.f, -1.f},{1.f, 0.f, -1.f},{2.f, 0.f, -1.f},\
                    {-1.f, 0.f, -2.f},{0.f, 0.f, -2.f},{1.f, 0.f, -2.f}
};

constexpr glm::vec3 treeStem[] = {
  {0.f, 2.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, -1.f, 0.f}, {0.f, -2.f, 0.f}
};

int main()
{
  GLFWwindow* window = init();
  GameController controller;

  ShaderManager sm;// シングルトンで管理するかなー
  BlockManager bm;
  
  BlockRef br = bm.getBlockRef(GRASS);
  GLuint vao = sm.makeBOID(br.model.getCube(), br.model.getCubeIndex(), br.material.getImage());// VBO, EBO, TEX
  
  BlockRef br2 = bm.getBlockRef(CHARACTER);
  GLuint vao2 = sm.makeBOID(br2.model.getCube(), br2.model.getCubeIndex(), br2.material.getImage());// VBO, EBO, TEX
  bm.addBlock(CHARACTER, BlockObject(), glm::vec3(8*UNIT, 6*UNIT, 16*UNIT));
  sm.attachObjects(vao2, bm.getBlockOffsetsRef(CHARACTER));// IBO
  
  // append terrain and wall
  for (int i=0; i<20; ++i) {
    for (int j=0; j<20; ++j) {
      bm.addBlock(GRASS, BlockObject(), glm::vec3(2*j*UNIT, 0.0f, 2*i*UNIT));// IBO
    }
  }
  for (int i=0; i<20; ++i) {
    for (int j=0; j<20; ++j) {
      bm.addBlock(GRASS, BlockObject(), glm::vec3(2*j*UNIT, 2*i*UNIT + 2*UNIT, 0.0f));// IBO
    }
  }
  sm.attachObjects(vao, bm.getBlockOffsetsRef(GRASS));// IBO

  // append tree
  BlockRef tlRef = bm.getBlockRef(TREE_LEAF);
  BlockRef tsRef = bm.getBlockRef(TREE_STEM);
  GLuint tlVao = sm.makeBOID(tlRef.model.getCube(), tlRef.model.getCubeIndex(), tlRef.material.getImage());
  GLuint tsVao = sm.makeBOID(tsRef.model.getCube(), tsRef.model.getCubeIndex(), tsRef.material.getImage());
  {
    glm::vec3 treeBasePos = 2*UNIT*glm::vec3{2.f, 2.f+1.f, 5.f};
    for (int i=0; i<sizeof(treeLeaf)/sizeof(treeLeaf[0]); ++i) {
      bm.addBlock(TREE_LEAF, BlockObject(), 2*UNIT*treeLeaf[i] + treeBasePos);
    }
    for (int i=0; i<sizeof(treeStem)/sizeof(treeStem[0]); ++i) {
      bm.addBlock(TREE_STEM, BlockObject(), 2*UNIT*treeStem[i] + treeBasePos);
    }
  }
  {
    glm::vec3 treeBasePos = 2*UNIT*glm::vec3{10.f, 2.f+1.f, 5.f};
    for (int i=0; i<sizeof(treeLeaf)/sizeof(treeLeaf[0]); ++i) {
      bm.addBlock(TREE_LEAF, BlockObject(), 2*UNIT*treeLeaf[i] + treeBasePos);
    }
    for (int i=0; i<sizeof(treeStem)/sizeof(treeStem[0]); ++i) {
      bm.addBlock(TREE_STEM, BlockObject(), 2*UNIT*treeStem[i] + treeBasePos);
    }
  }
  sm.attachObjects(tlVao, bm.getBlockOffsetsRef(TREE_LEAF));
  sm.attachObjects(tsVao, bm.getBlockOffsetsRef(TREE_STEM));

  Canvas canvas(bm.getCharacter(), windowWidth / windowHeight);
  canvas.setCallbackFunction(window);
  canvas.setModel(glm::scale(glm::mat4(1.0f), glm::vec3(UNIT)));
  canvas.setView();
  canvas.setProj();
  sm.setUniformM4f("model", canvas.getModelPtr());
  sm.setUniformM4f("view",  canvas.getViewPtr());
  sm.setUniformM4f("proj",  canvas.getProjPtr());

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 特定種のブロックを表示上消せるのは便利だけど、コードがわかりにくい
    sm.render(vao);
    sm.render(vao2);
    sm.render(tlVao);
    sm.render(tsVao);

    canvas.setTime(glfwGetTime());
    controller.keyInputsPoll(window);
    canvas.setView();
    canvas.setProj();
    sm.setUniformM4f("view",  canvas.getViewPtr());
    sm.setUniformM4f("proj",  canvas.getProjPtr());
    canvas.update(controller.getKeyInputs(), bm, !DEBUG_MODE);
    sm.attachObjects(vao2, bm.getBlockOffsetsRef(CHARACTER));// 更新

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
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