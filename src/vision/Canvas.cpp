#include <vision/Canvas.hpp>
#include <utility>

void Canvas::setCallbackFunction(GLFWwindow* window)
{
  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, +[](GLFWwindow* window, double a, double b){
      static_cast<Canvas*>(glfwGetWindowUserPointer(window))->mouseCallbackFunction(window, a, b);
  });
  glfwSetScrollCallback(window, +[](GLFWwindow* window, double a, double b){
      static_cast<Canvas*>(glfwGetWindowUserPointer(window))->scrollCallbackFunction(window, a, b);
  });
}

// キャラクター周りの回転にしたい
void Canvas::mouseCallbackFunction(GLFWwindow* window, double x, double y)
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

void Canvas::scrollCallbackFunction(GLFWwindow* window, double dx, double dy)
{
  MouseInputs.mouse_dy = dy;
  if (fov >= 1.0f && fov <= 45.0f) fov -= MouseInputs.mouse_dy;
  if (fov <= 1.0f)                 fov = 1.0f;
  if (fov >= 45.0f)                fov = 45.0f;
}

void Canvas::update(KeyManager km, BlockManager& bm, int mode)
{
  glm::vec3 velo(0.0f);
  velo += static_cast<float>(static_cast<int>(km.d) - static_cast<int>(km.a)) * glm::cross(Front, Up);
  velo += static_cast<float>(static_cast<int>(km.z) - static_cast<int>(km.x)) * Up;
  velo += static_cast<float>(static_cast<int>(km.w) - static_cast<int>(km.s)) * Front;
  if (!glm::all(glm::lessThan(glm::abs(velo), glm::vec3(glm::epsilon<float>())))) {
    velo = glm::normalize(velo);
  }

  Velocity.x = velo.x;
  if (mode) {
    Velocity.y = velo.y;
  } else {
    Velocity.y += velo.y + Acc * DeltaTime;
    if (Velocity.y > MaxSpeed) {
      Velocity.y = MaxSpeed;
    } else if (Velocity.y < -MaxSpeed) {
      Velocity.y = -MaxSpeed;
    }
  }
  Velocity.z = velo.z;

  // BlockPosition += 2.5f * DeltaTime * Velocity;
  Block.addPositionValue(2.5f * DeltaTime * Velocity);
  // Block.addPositionValue(0.05f * DeltaTime * Velocity);
  Position  = BlockBaseCanvasPosition + Block.getPosition();

  auto modify = bm.collideWith(Block);
  if (std::get<0>(modify)) {// 定期的に下に抜けるな。
    // v3Print("Position", std::get<1>(modify));
    // v3Print("Velocity", std::get<2>(modify));
    Block.addPositionValue(std::move(std::get<1>(modify)));
    Position = BlockBaseCanvasPosition + Block.getPosition();
    Velocity *= std::get<2>(modify);
  }
}