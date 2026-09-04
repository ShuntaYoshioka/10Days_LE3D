#pragma once
#include "CameraController.h"
#include "Fade.h"
#include "Goal.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include "TitleScene.h"
#include "PlayerAttack.h"
#include "Particle.h"
#include <vector>

class GameScene {

private:
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	bool finished_ = false;

	Goal goal_;

public:
	enum class Phase {
		kFadeIn,
		kPlay,
		kDeath,
		kFadeOut,
	};

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// デストラクタ
	~GameScene();

	// 3Dモデル
	KamataEngine::Model* modelBlock_;

	KamataEngine::Model* modelSkydome_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::Model* modelGoal_ = nullptr;

	KamataEngine::Model* modelAttack_ = nullptr;

	KamataEngine::Model* modelParticle_ = nullptr;

	uint32_t textureHandleGraph_ = 0;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera camera_;

	// 自キャラ
	Player* player_ = nullptr;

	PlayerAttack* playerAttack_ = nullptr;
	
	// sky
	Skydome* skydome_ = nullptr;

	// パーティクル
	Particle* particle_ = nullptr; 

	// カメラコントローラ
	CameraController* cameraController_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;


	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	//地面
	std::vector<std::vector<WorldTransform*>> worldTransformGrounds_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	void GenerateBlocks();

	void CheckAllCollisions();

	void ChangePhase();

	//壊す壁について
	std::vector<std::vector<int>> blockHp_;
	std::vector<std::vector<int>> blockInterval_;

	uint32_t numVertical_ = 0;
	uint32_t numHorizontal_ = 0;

	// ゲームプレイから開始
	Phase phase_;

	bool isFinished() const { return finished_; }

	bool isclear_ = false;

	 bool isAllKill_ = false;

	Fade* fade_ = nullptr;

	bool wasExploding = false;
};
