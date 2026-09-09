#include "ClearScene.h"
#include <cmath>

using namespace KamataEngine;

ClearScene::~ClearScene() {
	delete endSprite_;
	delete stclearSprite_;
	delete panelSprite_;
	delete fade_;
}

void ClearScene::Initialize(uint32_t openedConnectionCount) {
	openedConnectionCount_ = openedConnectionCount;

	// 開通数が多いほど減点(0点未満にはしない)
	score_ = RoomConnectivity::ComputeScore(openedConnectionCount_);

	panelTextureHandle_ = TextureManager::Load("panel/score.png");
	panelSprite_ = Sprite::Create(panelTextureHandle_, {0, 0});
	panelSprite_->SetSize({1280.0f, 720.0f});

	stclearTextureHandle_ = TextureManager::Load("panel/stclear.png");
	stclearSprite_ = Sprite::Create(stclearTextureHandle_, {368.0f, 100.0f});
	stclearSprite_->SetSize({550.0f, 92.0f});

	// 拡縮パルスのため中心アンカーで配置(中心座標 = 元の矩形の中心)
	endTextureHandle_ = TextureManager::Load("panel/end.png");
	endSprite_ = Sprite::Create(endTextureHandle_, {639.5f, 597.0f});
	endSprite_->SetAnchorPoint({0.5f, 0.5f});
	endSprite_->SetSize({683.0f, 48.0f});

	// SCORE : (8文字) を1.5倍スケールで表示した幅の分だけ右にずらす
	scoreNumber_.Initialize({578.0f, 320.0f}, 27.0f);
	scoreNumber_.SetValue(score_);

	// OPENED CONNECTIONS : (21文字) を1.5倍スケールで表示した幅の分だけ右にずらす
	openedNumber_.Initialize({754.0f, 350.0f}, 27.0f);
	openedNumber_.SetValue(openedConnectionCount_);

	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void ClearScene::Update() {

	// SPACE TO TITLEの拡縮パルス(サイン波でサイズが100%~110%を往復)
	const float kPulseSpeed = 3.0f;
	endPulseTimer_ += 1.0f / 60.0f;
	float endScale = 1.0f + 0.1f * (0.5f + 0.5f * std::sin(endPulseTimer_ * kPulseSpeed));
	endSprite_->SetSize({683.0f * endScale, 48.0f * endScale});

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->isFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			nextScene_ = NextScene::kTitle;
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		} else if (Input::GetInstance()->PushKey(DIK_RETURN)) {
			nextScene_ = NextScene::kStageSelect;
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->isFinished()) {
			finished_ = true;
		}
		break;
	}
}

void ClearScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	DebugText* debugText = DebugText::GetInstance();

	debugText->SetScale(1.5f);
	debugText->SetPos(470, 320);
	debugText->Printf("SCORE : ");

	debugText->SetPos(470, 350);
	debugText->Printf("OPENED CONNECTIONS : ");

	Sprite::PreDraw(dxCommon->GetCommandList());

	panelSprite_->Draw();
	stclearSprite_->Draw();
	debugText->DrawAll();
	scoreNumber_.Draw();
	openedNumber_.Draw();
	endSprite_->Draw();

	Sprite::PostDraw();

	fade_->Draw();
}
