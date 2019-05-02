#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Canvas {
  private:
    glm::vec3 Position = glm::vec3(1.0f, 1.0f, 1.5f);
    glm::vec3 Front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up       = glm::vec3(0.0f, 1.0f,  0.0f);
    
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Proj;
    float fov   =  45.0f;

  public:
    void setModel(glm::mat4 model) {Model = model;}
    void setView() {View  = glm::lookAt(Position, glm::vec3(0,0,0), Up);}
    void setProj() {Proj  = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 1.0f, 100.0f);}

    auto getModelPtr() {return glm::value_ptr(Model);}
    auto getViewPtr() {return glm::value_ptr(View);}
    auto getProjPtr() {return glm::value_ptr(Proj);}


};

#endif