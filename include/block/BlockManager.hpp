#ifndef __BLOCK_MANAGER_HPP__
#define __BLOCK_MANAGER_HPP__

#include <vector>
#include <array>
#include <map>

#include "BlockObject.hpp"
#include "Model.hpp"
#include "Material.hpp"
// #include "../util.hpp"

enum BlockType {
  GRASS=0, ROCK, BRICK
};

struct cmpVec3ByElements {
  bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const
  {
      return lhs.x < rhs.x ||
            (lhs.x == rhs.x && (lhs.y < rhs.y || (lhs.y == rhs.y && lhs.z < rhs.z)));
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