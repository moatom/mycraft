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
// 当たり判定は、ブロックIDからブロックへ
// offsetの消去は、ブロックのポジションから


// raserの場合はbm側で変更を行うけど、aabbの場合、キャンバスの速さ０化と移動の可否(bool)を制御する必要がある。
// 後者は前者にまとめられる。velocityを変えて渡せばいいのか。
bool BlockManager::collideWith(BlockObject& bo)
{// あっ、floorっていっても整数値じゃなくて、2*unit単位で制御しないと。　両方floorにかければ一対一対応するな　27マスとんないとだめ
// それか空間分割木で管理するか。まあPositionのインデックスは木中のIDに使えるし。
//ようは削除を同期させたいだけ。変更に関してはポジションの値しか変わらんからポインタからアクセスできる。
// 削除時はポジションのポインタ刺し直さないと
// というか、positionのアドレス値とoffsetListの開始値を比較すればインデックス計算できるな。。。
  glm::vec3 center = (glm::floor(bo.getPosition() / (2.f*UNIT))) * (2.f*UNIT);

  std::array<glm::vec3, 27> target;
  for (int i=0; i<3; ++i) {
    for (int j=0; j<3; ++j) {
      for (int k=0; k<3; ++k) {
        target.at(3*3*i+3*j+k) = center + 2*UNIT*glm::vec3(i-1,j-1,k-1);
      }
    }  
  }

  test("--------");
  // AABB
  glm::vec3 aMin, aMax;
  glm::vec3 bMin, bMax;
  aMin = bo.getPosition() - UNIT;
  aMax = bo.getPosition() + UNIT;
  // aMin = bo.getPosition() - bo.getSize()/2.f;
  // aMax = bo.getPosition() + bo.getSize()/2.f;

  for (auto& bs: BlockSets) {
    for (const auto& t: target) {
      for (auto& bo: bs.BlockList) {
        // find 床順からやった方が早いかと
        if (glm::all(glm::lessThan(glm::abs(bo.first - t), glm::vec3(glm::epsilon<float>())))) {
          // v3Print("", bo.first);
          bMin = t - UNIT;
          bMax = t + UNIT;
          if (glm::all(glm::greaterThan(aMax, bMin)) && glm::all(glm::greaterThan(bMax, aMin))) return true;
        }
      }

      // auto it = bs.BlockList.find(t);// 今の定義だとx>y>zの優先度で比較関数を適用するからまずいかも?こっちでは全ての要素の大小関係で判別したい。
      // if (it != bs.BlockList.end()) {
      //   bMin = it->second.getPosition() - UNIT;
      //   bMax = it->second.getPosition() + UNIT;
      //   // bMin = it->second.getPosition() - it->second.getSize()/2.f;
      //   // bMax = it->second.getPosition() + it->second.getSize()/2.f;
      //   if (glm::all(glm::greaterThan(aMax, bMin)) && glm::all(glm::greaterThan(bMax, aMin))) return true;
      // }
    }
  }
  return false;
}