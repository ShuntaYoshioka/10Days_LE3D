#include "GameScene.h"
#include "CameraController.h"
#include "MyMath.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	phase_ = Phase::kFadeIn;
	textureHandle_ = TextureManager::Load("./Resources./uvChecker.png");

	// 3Dモデルの生成
	modelBlock_ = Model::CreateFromOBJ("block");
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	modelPlayer_ = Model::CreateFromOBJ("P_rinker", true);
	modelGoal_ = Model::CreateFromOBJ("goal", true);
	modelAttack_ = Model::CreateFromOBJ("hammer", true);
	modelParticle_ = Model::CreateFromOBJ("deathParticle", true);

	textureHandleGraph_ = TextureManager::Load("white1x1.png");

	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	mapChipField_->ResetMapChipData();

	mapChipField_->LoadMapchipCsv("Resources/blocks.csv", 0);
	mapChipField_->LoadMapchipCsv("Resources/blocks2.csv", 1);

	numVertical_ = mapChipField_->GetNumBlockVirtical();
	numHorizontal_ = mapChipField_->GetNumBlockHorizontal();

	// 自キャラ生成
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);

	worldTransform_.Initialize();
	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	skydome_ = new Skydome();
	player_->SetMapChipField(mapChipField_);

	playerAttack_ = new PlayerAttack();
	playerAttack_->Initialize(modelAttack_, &camera_, player_);

	camera_.Initialize();

	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->SetMapChipField(mapChipField_);

	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(18, 3);
	Vector3 goalSize = {1.0f, 1.0f, 1.0f};
	goal_.Initialize(goalPosition, goalSize, modelGoal_);

	skydome_->Initialize(modelSkydome_, &camera_);

	GenerateBlocks();

	debugCamera_ = new DebugCamera(1280, 720);

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameScene::GenerateBlocks() {
	worldTransformBlocks_.resize(numVertical_);
	worldTransformGrounds_.resize(numVertical_);
	blockHp_.resize(numVertical_);
	blockInterval_.resize(numVertical_);

	for (uint32_t i = 0; i < numVertical_; ++i) {
		worldTransformBlocks_[i].resize(numHorizontal_, nullptr);
		worldTransformGrounds_[i].resize(numHorizontal_, nullptr);
		blockHp_[i].resize(numHorizontal_, 0);
		blockInterval_[i].resize(numHorizontal_, 0);

		for (uint32_t j = 0; j < numHorizontal_; ++j) {
			// レイヤー0地面
			MapChipType groundType = mapChipField_->GetMapChipTypeByIndex(j, i, 0);
			if (groundType == MapChipType::kBlank || groundType == MapChipType::kBlock) {
				WorldTransform* groundTransform = new WorldTransform();
				groundTransform->Initialize();
				Vector3 pos = mapChipField_->GetMapChipPositionByIndex(j, i);
				pos.y = 0.0f; // 床の高さ
				groundTransform->translation_ = pos;

				worldTransformGrounds_[i][j] = groundTransform;
			}

			// レイヤー1 壁ブロック
			MapChipType wallType = mapChipField_->GetMapChipTypeByIndex(j, i, 1);

			if (wallType == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				Vector3 pos = mapChipField_->GetMapChipPositionByIndex(j, i);
				pos.y = 1.0f;
				worldTransform->translation_ = pos;

				worldTransformBlocks_[i][j] = worldTransform;
				blockHp_[i][j] = -1; // 壊れない壁
			} else if (wallType == MapChipType::kBreakableBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				Vector3 pos = mapChipField_->GetMapChipPositionByIndex(j, i);
				pos.y = 1.0f;
				worldTransform->translation_ = pos;

				worldTransformBlocks_[i][j] = worldTransform;
				blockHp_[i][j] = 3; // 耐久値 3
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
#pragma region 自キャラとブロックの当たり判定

	AABB playerAABB = player_->GetAABB();
	AABB attackAABB = playerAttack_->GetAABB();

	for (uint32_t i = 0; i < numVertical_; ++i) {
		for (uint32_t j = 0; j < numHorizontal_; ++j) {
			// ブロックが存在しないか壊れている場合はスキップ
			if (!worldTransformBlocks_[i][j] || blockHp_[i][j] <= 0) {
				continue;
			}

			// ブロックのAABB
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(j, i);
			AABB blockAABB;
			blockAABB.min = {rect.left, -0.5f, rect.bottom};
			blockAABB.max = {rect.right, 0.5f, rect.top};

			// 1. 攻撃とブロックの当たり判定
			if (playerAttack_->IsActive() && IsCollision(attackAABB, blockAABB)) {
				if (blockInterval_[i][j] <= 0) {
					blockHp_[i][j]--;
					blockInterval_[i][j] = 30;

					if (blockHp_[i][j] <= 0) {

						Vector3 blockPos = worldTransformBlocks_[i][j]->translation_;

						// 新しいパーティクルを生成＆初期化
						particle_ = new Particle();
						particle_->Initialize(modelParticle_, &camera_, blockPos);

						delete worldTransformBlocks_[i][j];
						worldTransformBlocks_[i][j] = nullptr;


					}

					playerAttack_->OnCollision();

					if (!worldTransformBlocks_[i][j]) {
						continue;
					}
				}
			}

			// 壁とPlayerの当たり判定
			if (worldTransformBlocks_[i][j] && IsCollision(playerAABB, blockAABB)) {
				// 衝突
				player_->OnCollision(blockAABB);

				//AABB更新
				playerAABB = player_->GetAABB();
			}
		}
	}

	if (IsCollision(playerAABB, goal_.GetAABB())) {
		finished_ = true;
		isclear_ = true;
	}
#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		break;
	case Phase::kDeath:
		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);
		break;
	case Phase::kFadeIn:
		if (fade_->isFinished()) {
			phase_ = Phase::kPlay;
		}
		break;
	case Phase::kFadeOut:
		if (fade_->isFinished()) {
			finished_ = true;
		}
		break;
	};
}

void GameScene::Update() {

	fade_->Update();

	switch (phase_) {
	case Phase::kPlay:
		CheckAllCollisions();
		break;
	case Phase::kDeath:
		break;
	case Phase::kFadeIn:
		fade_->Update();
		break;
	case Phase::kFadeOut:
		fade_->Update();
		CheckAllCollisions();
		break;
	}

	if (phase_ != Phase::kFadeOut) {
		player_->Update();
	}

	// 壊せるインターバル
	for (uint32_t i = 0; i < numVertical_; ++i) {
		for (uint32_t j = 0; j < numHorizontal_; ++j) {
			if (blockInterval_[i][j] > 0) {
				blockInterval_[i][j]--;
			}
		}
	}

		goal_.Update();


	skydome_->Update();
	playerAttack_->StartAttack();
	playerAttack_->Update();

	if (particle_) {
		particle_->Update();
		if (particle_->IsFinished()) {
			delete particle_;
			particle_ = nullptr;
		}
	}

	cameraController_->Update();
	debugCamera_->Update();

	for (auto& groundLine : worldTransformGrounds_) {
		for (WorldTransform* ground : groundLine) {
			if (!ground)
				continue;
			ground->matWorld_ = MakeAffineMatrix(ground->scale_, ground->rotation_, ground->translation_);
			ground->TransferMatrix();
		}
	}

	for (auto& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();
		}
	}

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	ChangePhase();

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		camera_.TransferMatrix();
	}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	if (skydome_) {
		skydome_->Draw();
	}

	// 地面の描画
	for (auto& groundLine : worldTransformGrounds_) {
		for (WorldTransform* ground : groundLine) {
			if (!ground)
				continue;
			modelBlock_->Draw(*ground, camera_);
		}
	}

	// 壁 ブロックの描画
	for (auto& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	if (player_) {
		player_->Draw();
	}

	if (particle_) {
		particle_->Draw();
	}

	if (playerAttack_->IsActive()) {
		playerAttack_->Draw();
	}

		goal_.Draw(&camera_);
	

	Model::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());
	Sprite::PostDraw();

	fade_->Draw();
}

GameScene::~GameScene() {
	delete modelBlock_;
	delete debugCamera_;
	delete modelPlayer_;
	delete fade_;
	delete modelSkydome_;
	delete mapChipField_;
	delete particle_;
	delete modelParticle_;

	for (auto& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	for (auto& groundLine : worldTransformGrounds_) {
		for (WorldTransform* ground : groundLine) {
			delete ground;
		}
	}
	worldTransformGrounds_.clear();
}