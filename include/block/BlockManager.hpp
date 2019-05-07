#ifndef __BLOCK_MANAGER_HPP__
#define __BLOCK_MANAGER_HPP__

#include <vector>
#include <array>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "BlockObject.hpp"
#include "Model.hpp"
#include "Material.hpp"
#include "../util.hpp"

enum BlockType {
  GRASS=0, ROCK, BRICK
};

struct cmpVec3ByElements {
  bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const
  {
    // 排他的じゃないが。。。
    return  lhs.x < rhs.x ||
            (glm::abs<float>(lhs.x - rhs.x) < glm::epsilon<float>() && (lhs.y < rhs.y || (glm::abs<float>(lhs.y - rhs.y) < glm::epsilon<float>()  && lhs.z < rhs.z)));
  }
};

typedef struct {
  std::map<glm::vec3, BlockObject, cmpVec3ByElements> BlockList;
  std::vector<glm::vec3> OffsetList;
  Material material;
  bool needUpdate;
  int lastModified;
} BlockSet;

typedef struct {
  Model &model;
  Material &material;
} BlockRef;

class BlockManager {
  Model model;
  BlockSet BlockSets[1];

  public:
    void addBlock(BlockType, BlockObject, glm::vec3);
    bool collideWith(BlockObject&);

    BlockRef getBlockRef(BlockType type)
    {
      return BlockRef{model, BlockSets[type].material};
    }

    std::vector<glm::vec3>& getBlockOffsetsRef(BlockType type)
    {
      return BlockSets[type].OffsetList;
    }
};
#endif