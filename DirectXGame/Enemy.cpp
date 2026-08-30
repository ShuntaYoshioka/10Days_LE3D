#include "Enemy.h"
#include "MyMath.h"
#include <numbers>

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {

	model_ = model;
	camera_ = camera;
	walkTimer_ = 0.0f;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;
}

void Enemy::Update() {
	// ゴール中心
	KamataEngine::Vector3 centerPos = mapChipField_->GetMapChipPositionByIndex(27, 7);

	if (state_ == EnemyState::kMoving) {
		// ゴールに向かう
		KamataEngine::Vector3 diff;
		diff.x = centerPos.x - worldTransform_.translation_.x;
		diff.y = centerPos.y - worldTransform_.translation_.y;
		diff.z = centerPos.z - worldTransform_.translation_.z; 

		// 差が小さくなったら周回に切り替え
		if (std::abs(diff.x) < 0.2f && std::abs(diff.y) < 0.2f) {
			state_ = EnemyState::kCircling;
			// ゴールとの相対角度を計算
			circleAngle_ = std::atan2(worldTransform_.translation_.y - centerPos.y, worldTransform_.translation_.x - centerPos.x);
			return;
		}

		// 差が大きい方の方向だけに移動
		velocity_ = {0.0f, 0.0f, 0.0f};
		if (std::abs(diff.x) >= std::abs(diff.y)) {
			velocity_.x = (diff.x > 0) ? kWalkSpeed : -kWalkSpeed;
		} else {
			velocity_.y = (diff.y > 0) ? kWalkSpeed : -kWalkSpeed;
		}

		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.y += velocity_.y;
		worldTransform_.translation_.z += velocity_.z;

	} else if (state_ == EnemyState::kCircling) {
		// 周囲を円運動
		const float radius = 1.5f;       // 中心からの半径
		const float rotateSpeed = 0.05f; // 回転速度

		circleAngle_ += rotateSpeed;

		worldTransform_.translation_.x = centerPos.x + std::cos(circleAngle_) * radius;
		worldTransform_.translation_.y = centerPos.y + std::sin(circleAngle_) * radius;
	}

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}




void Enemy::OnCollision(const Player* player) { (void)player; }

void Enemy::OnCollision(const PlayerAttack* playerAttack) { 
	(void)playerAttack;
	isDead_ = true;

}

KamataEngine::Vector3 Enemy::GetWorldPosition() {
	return worldTransform_.translation_;
}


AABB Enemy::GetAABB() {
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	   if (isDead_) {
		return {
		    {9999,  9999,  9999 },
            {-9999, -9999, -9999}
        };
	}

	aabb.min = {
	    worldPos.x - 1.0f / 2.0f,
	    worldPos.y - 1.0f / 2.0f,
	    worldPos.z - 1.0f / 2.0f,
	};

	aabb.max = {
	    worldPos.x + 1.0f / 2.0f,
	    worldPos.y + 1.0f / 2.0f,
	    worldPos.z + 1.0f / 2.0f,
	};
	return aabb;
}

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }