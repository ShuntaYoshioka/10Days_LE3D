#pragma once
#include "Player.h"
#include <KamataEngine.h>

class PlayerAttack {
public:
	// 初期化・更新・描画
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, Player* player);
	void StartAttack();
	void Update();
	void Draw();

	// 衝突・ゲッター処理
	void OnCollision();
	KamataEngine::Vector3 GetWorldPosition();
	AABB GetAABB();

	// フラグ操作
	bool IsActive() const { return isActive_; }
	void SetIsActive(bool isActive) { isActive_ = isActive; }

private:
	// 定数設定
	static inline const float kRadius = 0.8f; // 攻撃の判定半径

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	Player* player_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	bool isActive_ = false;
};