#pragma once
#include <KamataEngine.h>

#include "Fade.h"
class TitleScene {
public:

	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	void Initialize();
	void Update();
	void Draw();
	~TitleScene();

	KamataEngine::Model* model_;

	KamataEngine::Camera camera_;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::WorldTransform worldTransformPlayer_;

	bool isFinished() const { return finished_; }

private:
	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;

	uint32_t titleTextureHandle_ = 0;

	KamataEngine::Sprite* titleSprite_ = nullptr;

	uint32_t startTextureHandle_ = 0;

	KamataEngine::Sprite* startSprite_ = nullptr;

	// SPACE TO START点滅用カウンター
	float startBlinkTimer_ = 0.0f;
};
