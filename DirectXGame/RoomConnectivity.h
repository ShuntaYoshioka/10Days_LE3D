#pragma once
#include "MapChipField.h"
#include <cstdint>
#include <vector>

///<summary>
/// 部屋(壁で区切られた通行可能マスの連結成分)同士が
/// 何本つながった(開通した)かを数えるクラス
///</summary>
class RoomConnectivity {
public:
	// 壁レイヤー(layer=1)を読み取り、kBlankマス同士をUnionして初期の部屋分けを行う
	void Initialize(MapChipField* mapChipField, uint32_t numHorizontal, uint32_t numVertical);

	// ブロックが破壊されたマスを通行可能にし、隣接マスとUnionを試みる
	// 実際に異なる部屋が繋がった場合はtrueを返す
	bool NotifyBlockBroken(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetOpenedConnectionCount() const { return openedConnectionCount_; }

	// 見栄え用のスコア基準点(開通0本のときの満点)
	static inline const uint32_t kBaseScore = 10000;
	// 開通1本につき減点する点数
	static inline const uint32_t kScorePerConnection = 100;

	// 開通数からスコアを算出する(0点未満にはならない)
	static uint32_t ComputeScore(uint32_t openedConnectionCount);

private:
	uint32_t ToCellIndex(uint32_t xIndex, uint32_t yIndex) const { return yIndex * numHorizontal_ + xIndex; }

	uint32_t Find(uint32_t cellIndex);
	bool Unite(uint32_t a, uint32_t b);

	uint32_t numHorizontal_ = 0;
	uint32_t numVertical_ = 0;

	std::vector<uint32_t> parent_;
	std::vector<uint32_t> rank_;
	std::vector<bool> isPassable_;

	uint32_t openedConnectionCount_ = 0;
};
