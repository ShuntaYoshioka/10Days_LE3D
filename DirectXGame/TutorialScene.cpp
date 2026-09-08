#include "TutorialScene.h"

using namespace KamataEngine;

TutorialScene::~TutorialScene() {
	delete tutorialSprite_;
	delete fade_;
}

void TutorialScene::Initialize() {
	tutorialTextureHandle_ = TextureManager::Load("panel/tutorial.png");
	tutorialSprite_ = Sprite::Create(tutorialTextureHandle_, {0, 0});
	tutorialSprite_->SetSize({1280.0f, 720.0f});

	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void TutorialScene::Update() {

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->isFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
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

void TutorialScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Sprite::PreDraw(dxCommon->GetCommandList());
	tutorialSprite_->Draw();
	Sprite::PostDraw();

	fade_->Draw();
}
