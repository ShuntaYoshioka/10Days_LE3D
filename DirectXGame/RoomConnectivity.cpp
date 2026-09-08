#include "RoomConnectivity.h"
#include <utility>

void RoomConnectivity::Initialize(MapChipField* mapChipField, uint32_t numHorizontal, uint32_t numVertical) {
	numHorizontal_ = numHorizontal;
	numVertical_ = numVertical;

	const uint32_t numCells = numHorizontal_ * numVertical_;
	parent_.resize(numCells);
	rank_.assign(numCells, 0);
	isPassable_.assign(numCells, false);
	openedConnectionCount_ = 0;

	for (uint32_t idx = 0; idx < numCells; ++idx) {
		parent_[idx] = idx;
	}

	// 壁レイヤー(layer=1)を読み取り、kBlankマスのみ通行可能にする
	for (uint32_t y = 0; y < numVertical_; ++y) {
		for (uint32_t x = 0; x < numHorizontal_; ++x) {
			MapChipType type = mapChipField->GetMapChipTypeByIndex(x, y, 1);
			isPassable_[ToCellIndex(x, y)] = (type == MapChipType::kBlank);
		}
	}

	// 初期状態の部屋分け(スコアには含めない)
	for (uint32_t y = 0; y < numVertical_; ++y) {
		for (uint32_t x = 0; x < numHorizontal_; ++x) {
			if (!isPassable_[ToCellIndex(x, y)]) {
				continue;
			}
			if (x + 1 < numHorizontal_ && isPassable_[ToCellIndex(x + 1, y)]) {
				Unite(ToCellIndex(x, y), ToCellIndex(x + 1, y));
			}
			if (y + 1 < numVertical_ && isPassable_[ToCellIndex(x, y + 1)]) {
				Unite(ToCellIndex(x, y), ToCellIndex(x, y + 1));
			}
		}
	}
}

bool RoomConnectivity::NotifyBlockBroken(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex >= numHorizontal_ || yIndex >= numVertical_) {
		return false;
	}

	uint32_t idx = ToCellIndex(xIndex, yIndex);
	if (isPassable_[idx]) {
		return false;
	}
	isPassable_[idx] = true;

	static const int kDx[4] = {1, -1, 0, 0};
	static const int kDy[4] = {0, 0, 1, -1};

	bool opened = false;
	for (int k = 0; k < 4; ++k) {
		int nx = static_cast<int>(xIndex) + kDx[k];
		int ny = static_cast<int>(yIndex) + kDy[k];
		if (nx < 0 || ny < 0 || nx >= static_cast<int>(numHorizontal_) || ny >= static_cast<int>(numVertical_)) {
			continue;
		}
		uint32_t nIdx = ToCellIndex(static_cast<uint32_t>(nx), static_cast<uint32_t>(ny));
		if (!isPassable_[nIdx]) {
			continue;
		}
		if (Unite(idx, nIdx)) {
			++openedConnectionCount_;
			opened = true;
		}
	}
	return opened;
}

uint32_t RoomConnectivity::ComputeScore(uint32_t openedConnectionCount) {
	const uint32_t deduction = openedConnectionCount * kScorePerConnection;
	return (deduction >= kBaseScore) ? 0 : kBaseScore - deduction;
}

uint32_t RoomConnectivity::Find(uint32_t cellIndex) {
	if (parent_[cellIndex] == cellIndex) {
		return cellIndex;
	}
	return parent_[cellIndex] = Find(parent_[cellIndex]);
}

bool RoomConnectivity::Unite(uint32_t a, uint32_t b) {
	uint32_t ra = Find(a);
	uint32_t rb = Find(b);
	if (ra == rb) {
		return false;
	}
	if (rank_[ra] < rank_[rb]) {
		std::swap(ra, rb);
	}
	parent_[rb] = ra;
	if (rank_[ra] == rank_[rb]) {
		++rank_[ra];
	}
	return true;
}
