#ifndef __GAME_CONTROLLER_HPP__
#define __GAME_CONTROLLER_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "InputStruct.hpp"
// #include "../util.hpp"

class GameController {
  private:
    KeyManager KeyInputs;
  public:
    GameController(){}

    void keyInputsPoll(GLFWwindow*);

    KeyManager getKeyInputs() {return KeyInputs;}
};
#endif