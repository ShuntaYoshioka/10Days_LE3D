#include "PlayerAttack.h"
#include "Player.h"
#include <cmath>
#include <numbers>

using namespace KamataEngine;

void PlayerAttack::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, Player* player) {
	model_ = model;
	camera_ = camera;
	player_ = player;

	worldTransform_.Initialize();
	worldTransform_.scale_ = {kRadius, kRadius, kRadius};                                 
}

void PlayerAttack::StartAttack() { isActive_ = true; }

void PlayerAttack::Update() {
	if (!isActive_ || !player_)
		return;

	// プレイヤーの位置と回転情報を取得
	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 playerRot = player_->GetWorldRotation(); 

	Vector3 forward;
	forward.x = std::sin(playerRot.y);
	forward.y = 0.0f;
	forward.z = std::cos(playerRot.y);

	//攻撃の位置 
	worldTransform_.translation_.x = playerPos.x + forward.x;
	worldTransform_.translation_.y = playerPos.y;
	worldTransform_.translation_.z = playerPos.z + forward.z;

	//回転をプレイヤーに合わせる
	worldTransform_.rotation_ = playerRot;

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void PlayerAttack::Draw() {
	if (isActive_ && model_ && camera_) {
		model_->Draw(worldTransform_, *camera_);
	}
}

void PlayerAttack::OnCollision() {
	// 常に攻撃を出し続ける場合は削除、当たり判定時消すなら isActive_ = false;
	isActive_ = false;
}

KamataEngine::Vector3 PlayerAttack::GetWorldPosition() { return worldTransform_.translation_; }

AABB PlayerAttack::GetAABB() {
	AABB aabb;
	aabb.min.x = worldTransform_.translation_.x - kRadius;
	aabb.min.y = worldTransform_.translation_.y - kRadius;
	aabb.min.z = worldTransform_.translation_.z - kRadius;

	aabb.max.x = worldTransform_.translation_.x + kRadius;
	aabb.max.y = worldTransform_.translation_.y + kRadius;
	aabb.max.z = worldTransform_.translation_.z + kRadius;
	return aabb;
}