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

	// 値が減った瞬間などに呼ぶと、一瞬赤くフラッシュしてから白へ戻る
	void Flash();

	void Update();

	void Draw();

	~NumberDrawer();

private:
	static const uint32_t kMaxDigits = 5;

	uint32_t textureHandles_[10] = {};
	KamataEngine::Sprite* digitSprites_[kMaxDigits] = {};

	KamataEngine::Vector2 position_{};
	float digitSize_ = 32.0f;
	uint32_t digitCount_ = 1;

	static constexpr float kFlashDuration = 0.3f;
	float flashTimer_ = 0.0f;
};
