#include "Player.h"
#include <algorithm>
#include <cassert>
#include <cmath>

using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;

	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {kRadius, kRadius, kRadius};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

	// KamataEngineでの行列転送
	worldTransform_.TransferMatrix();

	velocity_ = {0.0f, 0.0f, 0.0f};
	isDead_ = false;
}

void Player::Update() {

	if (isDead_) {
		return;
	}

	positionPrev_ = worldTransform_.translation_;

	worldTransform_.rotation_.y += kSpinSpeed;

	//移動処理
	Input* input = Input::GetInstance();
	velocity_ = {0.0f, 0.0f, 0.0f};

	if (input->PushKey(DIK_W) || input->PushKey(DIK_UP)) {
		velocity_.z += kForwardSpeed;
	}
	if (input->PushKey(DIK_S) || input->PushKey(DIK_DOWN)) {
		velocity_.z -= kForwardSpeed;
	}
	if (input->PushKey(DIK_A) || input->PushKey(DIK_LEFT)) {
		velocity_.x -= kForwardSpeed;
	}
	if (input->PushKey(DIK_D) || input->PushKey(DIK_RIGHT)) {
		velocity_.x += kForwardSpeed;
	}

	// 座標更新
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.z += velocity_.z;
	worldTransform_.translation_.y = 1.0f;

	//とりあえず手動で移動範囲の制限
	worldTransform_.translation_.x = (std::clamp)(worldTransform_.translation_.x, 1.0f, 17.0f);
	worldTransform_.translation_.z = (std::clamp)(worldTransform_.translation_.z, 1.0f, 20.0f);

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	if (!isDead_) {
		if (model_ && camera_) {
			model_->Draw(worldTransform_, *camera_);
		}
	}
}

KamataEngine::Vector3 Player::GetWorldPosition() { return worldTransform_.translation_; }

AABB Player::GetAABB() {
	AABB aabb;
	// 球体の中心座標から、半径 kRadius 分の立方体AABBを作る
	aabb.min.x = worldTransform_.translation_.x - kRadius;
	aabb.min.y = worldTransform_.translation_.y - kRadius;
	aabb.min.z = worldTransform_.translation_.z - kRadius;

	aabb.max.x = worldTransform_.translation_.x + kRadius;
	aabb.max.y = worldTransform_.translation_.y + kRadius;
	aabb.max.z = worldTransform_.translation_.z + kRadius;
	return aabb;
}

void Player::OnCollision(const AABB) {
	// 壁に当たったら押し戻す 改善する
	worldTransform_.translation_ = positionPrev_;


}