#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include "block/BlockObject.hpp"
#include "InputStruct.hpp"
#include "../util.hpp"

class Canvas {
  private:
    // glm::vec3 Position = glm::vec3(1.5f, 2*UNIT, 5.5f);
    glm::vec3 Position = glm::vec3(1.0f, 7*UNIT, 3.0f);
    glm::vec3 Front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up       = glm::vec3(0.0f, 1.0f,  0.0f);

    glm::vec3 Velocity = glm::vec3(0.0f, 0.0f,  0.0f);
    float Radius   = UNIT * 0.8f;
    // const float Acc = -9.8f;
    const float Acc = -1.0f;
    const float maxSpeed = 1.0f;
    
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

    float Ratio;

  public:
    Canvas(float ratio):Ratio(ratio) {}
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

    void setCallbackFunction(GLFWwindow *);
    void mouseCallbackFunction(GLFWwindow *, double, double);
    void scrollCallbackFunction(GLFWwindow *, double, double);

    void update(KeyManager km)
    {
      glm::vec3 velo(0.0f);
      velo += static_cast<float>(static_cast<int>(km.d) - static_cast<int>(km.a)) * glm::cross(Front, Up);
      velo += static_cast<float>(static_cast<int>(km.z) - static_cast<int>(km.x)) * Up;
      velo += static_cast<float>(static_cast<int>(km.w) - static_cast<int>(km.s)) * Front;
      if (!glm::all(glm::lessThan(glm::abs(velo), glm::vec3(glm::epsilon<float>())))) {
        velo = glm::normalize(velo);
      }

      Velocity.x = velo.x;
      Velocity.y += velo.y + Acc * DeltaTime;
      Velocity.z = velo.z;
      if (1) Velocity.y=0.0f;
      if (Velocity.y > maxSpeed) {
        Velocity.y = maxSpeed;
      } else if (Velocity.y < -maxSpeed) {
        Velocity.y = -maxSpeed;
      }

      Position += 2.5f * DeltaTime * Velocity;


      //update -> detectCollision -> if so, restore // status構造体でlastStatusを管理する。
    }
};

void Canvas::setCallbackFunction(GLFWwindow *window)
{
  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, +[](GLFWwindow* window, double a, double b){
      static_cast<Canvas*>(glfwGetWindowUserPointer(window))->mouseCallbackFunction(window, a, b);
  });
  glfwSetScrollCallback(window, +[](GLFWwindow* window, double a, double b){
      static_cast<Canvas*>(glfwGetWindowUserPointer(window))->scrollCallbackFunction(window, a, b);
  });
}

void Canvas::mouseCallbackFunction(GLFWwindow *window, double x, double y)
{
  if (firstCall) {
      MouseInputs.last_x = x;
      MouseInputs.last_y = y;
      firstCall = false;
  }

  MouseInputs.dx = x - MouseInputs.last_x;
  MouseInputs.dy = y - MouseInputs.last_y;
  MouseInputs.last_x = x;
  MouseInputs.last_y = y;
  
  yaw   += MouseInputs.dx * sensitivity;
  pitch -= MouseInputs.dy * sensitivity;

  if (pitch >  89.0f) pitch = 89.0f;
  if (pitch < -89.0f) pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  Front = glm::normalize(front);
}

void Canvas::scrollCallbackFunction(GLFWwindow *window, double dx, double dy)
{
  MouseInputs.mouse_dy = dy;
  if (fov >= 1.0f && fov <= 45.0f) fov -= MouseInputs.mouse_dy;
  if (fov <= 1.0f)                 fov = 1.0f;
  if (fov >= 45.0f)                fov = 45.0f;
}
#endif