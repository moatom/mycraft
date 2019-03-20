#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <array>

#include "model.hpp"

// constexpr float box[] = {
//    1,1,1, -1,1,1, -1,1,-1, 1,1,-1,
//    1,-1,1, -1,-1,1, -1,-1,-1, 1,-1,-1
// };

// constexpr GLuint boxIndex[] = {
// 			   0,4,5, 4,1,0,
// 			   0,1,3, 3,1,2,
// 			   4,7,5, 7,6,5,
// 			   1,5,6, 1,6,2,
// 			   3,7,4, 3,4,0,
// 			   2,6,7, 2,7,3	   
// };

/*
.......(1,1,-1,)...3
. top  .
.. . ..(1,1,1)...0

.......(1,-1,-1)...7
. bot .
.. ....(1,-1,1)...4
*/

static constexpr vertex square[] = {
  {{-0.5f,-0.5f,0.0f}, {0.8f,0.0f,0.0f,1.0f}, {0.0f, 0.0f}},
  {{-0.5f,0.5f,0.0f},  {0.8f,0.0f,0.0f,1.0f}, {0.0f, 1.0f}},
  {{0.5f,0.5f,0.0f},   {0.8f,0.0f,0.0f,1.0f}, {1.0f, 1.0f}},
  {{0.5f,-0.5f,0.0f},  {0.8f,0.0f,0.0f,1.0f}, {1.0f, 0.0f}}
};
static constexpr GLuint squareIndex[] = {
  0,1,2,
  0,2,3
};

// sum up it to class
constexpr std::array<vertex, 361> makeCircle()
{
  std::array<vertex, 361> circ{};// initializing by zero
  circ[0] = {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
  for (int i=0;i<360;++i) {
    GLfloat theta = i * M_PI / 180;
    circ[i+1] = {{0.5f*std::cosf(theta), 0.5f*std::sinf(theta), 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f},{static_cast<float> (i%2), static_cast<float> (i%2)}};
  }
  return circ;
}

constexpr std::array<GLuint, 360*3> makeCircleIndex()
{
  // 0,1,2,  0,2,3, 0,i,i+1 1<=i<360
  std::array<GLuint, 360*3> circIndex{};// initializing by zero
  for (int i=1;i<360;++i) {
    for (int j=0;j<2;++j) {
      circIndex.data()[3*(i-1)+1+j] = i+j;
    }
  }
  circIndex[359*3+1] = 360;
  circIndex[359*3+2] = 1;
  return circIndex;
}

const std::array<vertex,361> circle = makeCircle();
const std::array<GLuint,360*3> circleIndex = makeCircleIndex();

//----member----
// want to do pattern machhing with function template
Model::Model(modelType type)
{
  switch (type)
  {
    case SQUARE:
      id = SQUARE;
      _vertices = square;
      _elements = squareIndex;
      vcount = sizeof(square) / sizeof(square[0]);
      ecount = sizeof(squareIndex) / sizeof(squareIndex[0]);
      break;
  
    case CIRCLE:
      id = CIRCLE;
      _vertices = circle.data();
      _elements = circleIndex.data();
      vcount = circle.size();
      ecount = circleIndex.size();
      break;

    default:
      std::cerr << "shape fault" << std::endl;
      exit(1);
      // break;
  }


}