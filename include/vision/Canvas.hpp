#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include "block/BlockManager.hpp"
#include "block/BlockObject.hpp"
#include "InputStruct.hpp"
#include "../util.hpp"

class Canvas {
  private:
    BlockObject Block;// このブロックもbmに渡した方がいいか？動的なもの管理したくない
    // glm::vec3 BlockPosition = glm::vec3(1.0f, 2*UNIT, 3.0f);
    glm::vec3 BlockPosition = glm::vec3(2*UNIT, 2*UNIT, 2*UNIT);

    // const glm::vec3 BlockBaseCanvasPosition = glm::vec3(0.f, 5*UNIT, 4*UNIT);
    const glm::vec3 BlockBaseCanvasPosition = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 Position = BlockBaseCanvasPosition + BlockPosition;
    glm::vec3 Front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up       = glm::vec3(0.0f, 1.0f,  0.0f);

    glm::vec3 Velocity = glm::vec3(0.0f, 0.0f,  0.0f);
    const float Acc = -1.0f;
    const float MaxSpeed = 0.5f;
    
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Proj;

    float DeltaTime;
    float PreviousTime;

    MouseManager MouseInputs;
    bool firstCall=true;
    float yaw   = -90.0f;
    float pitch =  0.0f;
    float fov   =  45.0f;
    const float sensitivity = 0.1f;

    const float Ratio;

  public:
    Canvas(float ratio): Block(&BlockPosition), Ratio(ratio) {}

    void setModel(glm::mat4 model) {Model = model;}
    void setView() {View  = glm::lookAt(Position,Position + Front, Up);}
    void setProj() {Proj  = glm::perspective(glm::radians(fov), Ratio, 1.0f, 1000.0f);}
    void setTime(float currentTime)
    {
      DeltaTime = currentTime - PreviousTime;
      PreviousTime = currentTime;
    }

    auto getModelPtr() {return glm::value_ptr(Model);}
    auto getViewPtr() {return glm::value_ptr(View);}
    auto getProjPtr() {return glm::value_ptr(Proj);}
    // auto getCharacter() {return }

    void setCallbackFunction(GLFWwindow* );
    void mouseCallbackFunction(GLFWwindow* , double, double);
    void scrollCallbackFunction(GLFWwindow* , double, double);

    void update(KeyManager, BlockManager&, int);
};
#endif