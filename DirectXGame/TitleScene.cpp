#include "TitleScene.h"
#include <cmath>

using namespace KamataEngine;

TitleScene::~TitleScene() {

	Audio::GetInstance()->StopWave(bgmVoiceHandle_);

	delete startSprite_;
	delete titleSprite_;
	delete fade_;
 }

void TitleScene::Initialize() {
	// 3Dモデル
//	model_ = Model::CreateFromOBJ("titleFont");

	// カメラ初期化
	camera_.Initialize();

	worldTransform_.Initialize();
	worldTransformPlayer_.Initialize();

	titleTextureHandle_ = TextureManager::Load("panel/title.png");
	titleSprite_ = Sprite::Create(titleTextureHandle_, {0, 0});
	titleSprite_->SetSize({1280.0f, 720.0f});

	startTextureHandle_ = TextureManager::Load("panel/start.png");
	startSprite_ = Sprite::Create(startTextureHandle_, {298.0f, 573.0f});
	startSprite_->SetSize({683.0f, 48.0f});

	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);

	// SEの読み込み
	seConfirmHandle_ = Audio::GetInstance()->LoadWave("SE/ketei.mp3");

	// BGMの読み込み・再生(ループ)
	bgmHandle_ = Audio::GetInstance()->LoadWave("BGM/title.mp3");
	bgmVoiceHandle_ = Audio::GetInstance()->PlayWave(bgmHandle_, true, 0.5f);
}

void TitleScene::Update() {

	// SPACE TO STARTのフェード点滅(サイン波でなめらかに0.3~1.0を往復)
	const float kBlinkSpeed = 3.0f;
	startBlinkTimer_ += 1.0f / 60.0f;
	float startAlpha = 0.3f + 0.7f * (0.5f + 0.5f * std::sin(startBlinkTimer_ * kBlinkSpeed));
	startSprite_->SetColor(Vector4(1, 1, 1, startAlpha));

	switch (phase_) {
	case Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			Audio::GetInstance()->PlayWave(seConfirmHandle_);
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->isFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->isFinished()) {
			finished_ = true;
		}
	}

}

void TitleScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

Model::PostDraw();

Sprite::PreDraw(dxCommon->GetCommandList());
titleSprite_->Draw();
startSprite_->Draw();
Sprite::PostDraw();

fade_->Draw();
}
