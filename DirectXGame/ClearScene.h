#pragma once
#include <KamataEngine.h>

#include "Fade.h"
#include "NumberDrawer.h"
#include "RoomConnectivity.h"

///<summary>
/// クリア画面(スコア表示、タイトル/ステージ選択への遷移選択)
///</summary>
class ClearScene {
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	enum class NextScene {
		kTitle,
		kStageSelect,
	};

	void Initialize(uint32_t openedConnectionCount);
	void Update();
	void Draw();
	~ClearScene();

	bool isFinished() const { return finished_; }

	NextScene GetNextScene() const { return nextScene_; }

private:
	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;

	NextScene nextScene_ = NextScene::kTitle;

	uint32_t openedConnectionCount_ = 0;

	uint32_t score_ = 0;

	NumberDrawer scoreNumber_;
	NumberDrawer openedNumber_;

	uint32_t panelTextureHandle_ = 0;

	KamataEngine::Sprite* panelSprite_ = nullptr;

	uint32_t stclearTextureHandle_ = 0;

	KamataEngine::Sprite* stclearSprite_ = nullptr;

	uint32_t endTextureHandle_ = 0;

	KamataEngine::Sprite* endSprite_ = nullptr;

	// SPACE TO TITLEの拡縮パルス用カウンター
	float endPulseTimer_ = 0.0f;
};
