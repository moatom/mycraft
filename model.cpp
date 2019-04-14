#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <array>

#include <model.hpp>

constexpr vertex cube[] = {
  // Top
  {{-1,1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.0f, 0.0f}},
  {{1,1,1},     {1.0f,1.0f,1.0f,1.0f},  {1.0f, 0.0f}},
  {{1,1,-1},    {1.0f,1.0f,1.0f,1.0f},  {1.0f, 1.0f}},
  {{-1,1,-1},   {1.0f,1.0f,1.0f,1.0f},  {0.0f, 1.0f}},
  // Bottom
  {{1,-1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.0f, 0.0f}},
  {{1,-1,-1},   {1.0f,1.0f,1.0f,1.0f},  {1.0f, 0.0f}},
  {{-1,-1,-1},  {1.0f,1.0f,1.0f,1.0f},  {1.0f, 1.0f}},
  {{-1,-1,1},   {1.0f,1.0f,1.0f,1.0f},  {0.0f, 1.0f}},
  // Front
  {{-1,-1,1},   {1.0f,1.0f,1.0f,1.0f},  {0.0f, 0.0f}},
  {{1,-1,1},    {1.0f,1.0f,1.0f,1.0f},  {1.0f, 0.0f}},
  {{1,1,1},     {1.0f,1.0f,1.0f,1.0f},  {1.0f, 1.0f}},
  {{-1,1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.0f, 1.0f}},
  // Back
  {{1,-1,-1},   {1.0f,1.0f,1.0f,1.0f},  {0.0f, 0.0f}},
  {{-1,-1,-1},  {1.0f,1.0f,1.0f,1.0f},  {1.0f, 0.0f}},
  {{-1,1,-1},   {1.0f,1.0f,1.0f,1.0f},  {1.0f, 1.0f}},
  {{1,1,-1},    {1.0f,1.0f,1.0f,1.0f},  {0.0f, 1.0f}},
  // Right
  {{1,-1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.0f, 0.0f}},
  {{1,-1,-1},   {1.0f,1.0f,1.0f,1.0f},  {1.0f, 0.0f}},
  {{1,1,-1},    {1.0f,1.0f,1.0f,1.0f},  {1.0f, 1.0f}},
  {{1,1,1},     {1.0f,1.0f,1.0f,1.0f},  {0.0f, 1.0f}},
  // Left
  {{-1,-1,-1},  {1.0f,1.0f,1.0f,1.0f},  {0.0f, 0.0f}},
  {{-1,-1,1},   {1.0f,1.0f,1.0f,1.0f},  {1.0f, 0.0f}},
  {{-1,1,1},    {1.0f,1.0f,1.0f,1.0f},  {1.0f, 1.0f}},
  {{-1,1,-1},   {1.0f,1.0f,1.0f,1.0f},  {0.0f, 1.0f}},
};

constexpr GLuint cubeIndex[] = {
  // Top 0-3
  0,1,2, 2,3,0,
  // Bottom 4-7
  4,5,6, 6,7,4,
  // Front 8-11
  8,9,10, 10,11,8,
  // Back 12-15
  12,13,14, 14,15,12,
  // Right 16-19
  16,17,18, 18,19,16,
  // Left 20-23
  20,21,22, 22,23,20
};

/*
3.......(1,1,-1,)...2
 . top .
0.......(1,1,1)...1

6.......(1,-1,-1)...5
 . bot .
7.......(1,-1,1)...4
*/


// static constexpr vertex square[] = {// for dubug of the front surface
//   {{-0.5f,-0.5f,0.0f}, {1.0f,1.0f,1.0f,1.0f}, {0.0f, 0.0f}},
//   {{0.5f,-0.5f,0.0f},  {1.0f,1.0f,1.0f,1.0f}, {1.0f, 0.0f}},
//   {{0.5f,0.5f,0.0f},   {1.0f,1.0f,1.0f,1.0f}, {1.0f, 1.0f}},
//   {{-0.5f,0.5f,0.0f},  {1.0f,1.0f,1.0f,1.0f}, {0.0f, 1.0f}}
// };
static constexpr vertex square[] = {// for dubug of the right surface
  {{1,-1,1},    {1.0f,1.0f,1.0f,1.0f},  {0.0f, 0.0f}},
  {{1,-1,-1},   {1.0f,1.0f,0.0f,1.0f},  {1.0f, 0.0f}},
  {{1,1,-1},    {0.0f,1.0f,1.0f,1.0f},  {1.0f, 1.0f}},
  {{1,1,1},     {0.0f,1.0f,1.0f,1.0f},  {0.0f, 1.0f}},
};
// static constexpr vertex square[] = {// for dubug of the left surface
//   {{-1,-1,-1},  {1.0f,1.0f,1.0f,1.0f},  {0.0f, 0.0f}},
//   {{-1,-1,1},   {0.0f,1.0f,1.0f,1.0f},  {1.0f, 0.0f}},
//   {{-1,1,1},    {0.0f,1.0f,1.0f,1.0f},  {1.0f, 1.0f}},
//   {{-1,1,-1},   {0.0f,1.0f,1.0f,1.0f},  {0.0f, 1.0f}},
// };

static constexpr GLuint squareIndex[] = {
  0,1,2, 2,3,0
};

constexpr std::array<vertex, 361> makeCircle()
{
  std::array<vertex, 361> circ{};// initialization by zero
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
  std::array<GLuint, 360*3> circIndex{};// initialization by zero
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

    case CUBE:
      id = CUBE;
      _vertices = cube;
      _elements = cubeIndex;
      vcount = sizeof(cube) / sizeof(cube[0]);
      ecount = sizeof(cubeIndex) / sizeof(cubeIndex[0]);
      break;

    default:
      std::cerr << "shape fault" << std::endl;
      exit(1);
      // break;
  }


}