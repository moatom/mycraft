#ifndef __BLOCK_MANAGER_HPP__
#define __BLOCK_MANAGER_HPP__

#include <vector>

#include "BlockObject.hpp"
#include "Model.hpp"
#include "Material.hpp"
#include "../util.hpp"

enum BlockType {
  GRASS=0, ROCK, BRICK
};//==tex

typedef struct {
  std::vector<BlockObject> BlockList;
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

    ~BlockManager() {test("aaaa");};
    void addBlock(BlockType type, BlockObject block, glm::vec3 position)
    {
      BlockSet &bs = BlockSets[type];// シンボルのエイリアスは参照で受ける
      bs.OffsetList.push_back(position);
      block.setPosition(&bs.OffsetList.back());// 
      bs.BlockList.push_back(block);

      if (bs.needUpdate == false) {
        bs.lastModified = bs.OffsetList.size() - 1;
        bs.needUpdate = true;
      }// deleteすると短くなるので、他のパラメータを調節しないとまずい
    }

    BlockRef getBlockRef(BlockType type)
    {
      return BlockRef{model, BlockSets[type].material};
    }

    std::vector<glm::vec3> &getBlockOffsetsRef(BlockType type)
    {
      return BlockSets[type].OffsetList;
    }
};
#endif