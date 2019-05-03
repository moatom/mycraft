#ifndef __GAME_CONTROLLER_HPP__
#define __GAME_CONTROLLER_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "InputStruct.hpp"
#include "../util.hpp"


class GameController {
  private:
    KeyManager KeyInputs;
  public:
    GameController(){}

    void keyInputsPoll(GLFWwindow *);

    KeyManager getKeyInputs() {return KeyInputs;}
};

void GameController::keyInputsPoll(GLFWwindow *window)// control function
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
      
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) KeyInputs.w = true;
  else KeyInputs.w = false;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) KeyInputs.a = true;
  else KeyInputs.a = false;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) KeyInputs.s = true;
  else KeyInputs.s = false;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) KeyInputs.d = true;
  else KeyInputs.d = false;
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) KeyInputs.z = true;
  else KeyInputs.z = false;
  if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) KeyInputs.x = true;
  else KeyInputs.x = false;
}
#endif