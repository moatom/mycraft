#ifndef INCLUDED_BLOCK_OBJECT_HPP
#define INCLUDED_BLOCK_OBJECT_HPP

#include <glm/glm.hpp>
#define UNIT 0.2f// ややこしいので、2倍に直す？名前を帰るか
// #define HALF_UNIT 0.1f

class BlockObject {
  public:
   protected:
    // 別に一度確保すれば静的だからポインタである必要はなく、そうすればbmの削除が楽になる？
    // blで探索して、結局配列位置は暗記しておく必要はあるのか。swap時にアドレスを通知しないと（イテレータでいける？）
    // 同時にたかだか一つ削除するなら、boからポジションポインタと、vectorからback-iを参照して値を入れ替えた後にポインタを入れ替える？
    // その場合、もう片方のboを発見しておく必要がある
    // それとも配列は重複させちゃって（先頭の値かなんかで）、gc的にあとでサイズを縮めればいいのでは？NULL入れるとどうなるんだろ
    // gcしたいなら、やっぱりoffset側からbo引けないと不便なのかな？いや、vectorのラッパで双方向にするしかないかも
    // 適当なタイミングで先頭との重複オフセットを探して削除したら、boのポインタをセットし直す必要がある。
    // てかでかくなりすぎたらもう一度作り直せばいいんじゃね？deleteをカウントして、一定回数で間引いた配列のposition値を用いてblを作り直せばいい
    glm::vec3 *Position;
    glm::vec3 Size;
  public:
    BlockObject(): Size(glm::vec3(2*UNIT)) {}
    BlockObject(glm::vec3* position): Position(position), Size(glm::vec3(2*UNIT)) {}

    glm::vec3 getPosition() {return *Position;}
    glm::vec3 getSize() {return Size;}
    void setPosition(glm::vec3* position) {Position = position;}
    void setPositionValue(glm::vec3& position) {*Position = position;}
    void addPositionValue(glm::vec3&& position) {*Position += position;}
};

#endif