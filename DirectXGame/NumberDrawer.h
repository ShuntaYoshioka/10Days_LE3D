#pragma once
#include "KamataEngine.h"
#include <cstdint>

///<summary>
/// 数値をResources/number/0.png~9.pngの画像で描画するクラス
///</summary>
class NumberDrawer {
public:
	void Initialize(const KamataEngine::Vector2& position, float digitSize = 32.0f);

	void SetPosition(const KamataEngine::Vector2& position);

	void SetValue(uint32_t value);

	void Draw();

	~NumberDrawer();

private:
	static const uint32_t kMaxDigits = 5;

	uint32_t textureHandles_[10] = {};
	KamataEngine::Sprite* digitSprites_[kMaxDigits] = {};

	KamataEngine::Vector2 position_{};
	float digitSize_ = 32.0f;
	uint32_t digitCount_ = 1;
};
