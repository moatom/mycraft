#ifndef INCLUDED_BLOCK_MANAGER_HPP
#define INCLUDED_BLOCK_MANAGER_HPP

#include <vector>
#include <array>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <tuple>

#include "BlockObject.hpp"
#include "Model.hpp"
#include "Material.hpp"
#include "../util.hpp"

struct cmpVec3ByElements {
  bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const
  {
    return  (glm::abs<float>(lhs.x - rhs.x) >= glm::epsilon<float>() && lhs.x < rhs.x) ||
            (glm::abs<float>(lhs.x - rhs.x) <  glm::epsilon<float>() && ((glm::abs<float>(lhs.y - rhs.y) >= glm::epsilon<float>() && lhs.y < rhs.y) ||
            (glm::abs<float>(lhs.y - rhs.y) <  glm::epsilon<float>() &&  (glm::abs<float>(lhs.z - rhs.z) >= glm::epsilon<float>() && lhs.z < rhs.z))));
  }
};

struct BlockSet {
  std::map<glm::vec3, BlockObject, cmpVec3ByElements> BlockList;
  std::vector<glm::vec3> OffsetList;
  Material material;// ここでブロックタイプを渡す
  bool needUpdate;
  int lastModified;

  BlockSet(BlockType type): material(type) {}
};

typedef struct {
  Model &model;
  Material &material;
} BlockRef;

class BlockManager {
  Model model;
  BlockSet BlockSets[BLOCK_TYPE_SIZE]{GRASS, ROCK, BRICK, TREE_LEAF, TREE_STEM, CHARACTER};

  public:
    void addBlock(BlockType, BlockObject, glm::vec3);
    std::tuple<bool, glm::vec3, glm::vec3> collideWith(BlockObject&);

    BlockRef getBlockRef(BlockType type) {return BlockRef{model, BlockSets[type].material};}
    std::vector<glm::vec3>& getBlockOffsetsRef(BlockType type) {return BlockSets[type].OffsetList;}
    BlockObject& getCharacter() {return BlockSets[CHARACTER].BlockList.begin()->second;}
};
#endif