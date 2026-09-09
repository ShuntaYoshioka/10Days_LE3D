#pragma once
#include <KamataEngine.h>

#include "Fade.h"

///<summary>
/// 遊び方説明画面
///</summary>
class TutorialScene {
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	void Initialize();
	void Update();
	void Draw();
	~TutorialScene();

	bool isFinished() const { return finished_; }

private:
	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;

	uint32_t tutorialTextureHandle_ = 0;

	KamataEngine::Sprite* tutorialSprite_ = nullptr;

	// BGM(タイトルと同じ曲を継続使用)
	uint32_t bgmHandle_ = 0;
	uint32_t bgmVoiceHandle_ = 0;
};
