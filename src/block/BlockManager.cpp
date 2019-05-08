#include <block/BlockManager.hpp>

void BlockManager::addBlock(BlockType type, BlockObject block, glm::vec3 position)
{
  BlockSet &bs = BlockSets[type];// シンボルのエイリアスは参照で受ける
  bs.OffsetList.push_back(position);
  block.setPosition(&bs.OffsetList.back());// 
  bs.BlockList.insert(std::make_pair(position,block));

  if (bs.needUpdate == false) {
    bs.lastModified = bs.OffsetList.size() - 1;
    bs.needUpdate = true;
  }// deleteすると短くなるので、他のパラメータを調節しないとまずい
}

constexpr std::array<glm::vec3, 6> DirectionVector{
  glm::vec3( 0.f, 0.f,-1.f),
  glm::vec3( 0.f, 0.f, 1.f),
  glm::vec3( 0.f,-1.f, 0.f),
  glm::vec3( 0.f, 1.f, 0.f),
  glm::vec3(-1.f, 0.f, 0.f),
  glm::vec3( 1.f, 0.f, 0.f)
};

// raserの場合はbm側で変更を行うけど、aabbの場合、キャンバスの速さ０化と移動の可否(bool)を制御する必要がある。
std::tuple<bool, glm::vec3, glm::vec3> BlockManager::collideWith(BlockObject& block)
{
  bool collision = false;
  glm::vec3 offset(0.f);
  glm::vec3 fixV(1.f);

  glm::vec3 center = (glm::floor(block.getPosition() / (2.f*UNIT))) * (2.f*UNIT);
  std::array<glm::vec3, 27> target;
  for (int i=0; i<3; ++i) {
    for (int j=0; j<3; ++j) {
      for (int k=0; k<3; ++k) {
        target.at(3*3*i+3*j+k) = center + 2*UNIT*glm::vec3(i-1,j-1,k-1);
      }
    }  
  }
  glm::vec3 aMin, aMax;
  glm::vec3 bMin, bMax;
  aMin = block.getPosition() - UNIT;
  aMax = block.getPosition() + UNIT;

  for (auto& bs: BlockSets) {
    for (const auto& t: target) {
      for (auto& bo: bs.BlockList) {
        // find
        if (glm::all(glm::lessThan(glm::abs(bo.first - t), glm::vec3(glm::epsilon<float>())))) {// 二分探索へ直す
          bMin = t - UNIT;
          bMax = t + UNIT;
          if (glm::all(glm::greaterThan(aMax, bMin)) && glm::all(glm::greaterThan(bMax, aMin))) {
            test("-----------");
            float max(0.f);
            int index=0;
            glm::vec3 relativeV = t - block.getPosition();
            for (int i=0; i<6; ++i) {
              float t = glm::dot(relativeV, DirectionVector.at(i));
              if (t>max) {
                max = t;
                index = i;
              }
            }

            if (glm::abs(glm::dot(offset, DirectionVector.at(index))) < glm::epsilon<float>()) {
              collision = true;// 0.2よりだいぶ大きくもどっている？4倍でかいな
              offset += -(UNIT * DirectionVector.at(index) - (-UNIT * DirectionVector.at(index) + DirectionVector.at(index) * relativeV))/4.f;
              // v3Print("", offset);
            }
            if (glm::abs(glm::dot(fixV, DirectionVector.at(index))) >= glm::epsilon<float>()) {
              fixV -= glm::abs(DirectionVector.at(index));// 最後に負だけ0にしても可。
            }
          }
        }
      }

      // auto it = bs.BlockList.find(t);
      // if (it != bs.BlockList.end()) {
      //   bMin = it->second.getPosition() - UNIT;
      //   bMax = it->second.getPosition() + UNIT;
      //   if (glm::all(glm::greaterThan(aMax, bMin)) && glm::all(glm::greaterThan(bMax, aMin))) return true;
      // }
    }
  }
  return std::tuple<bool, glm::vec3, glm::vec3>(collision, offset, fixV);
}