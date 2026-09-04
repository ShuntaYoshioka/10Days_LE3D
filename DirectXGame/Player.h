#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class MapChipField;
class Enemy;

class Player {
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	// ゲッター セッター
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	KamataEngine::Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }
	void SetVelocity(const KamataEngine::Vector3& velocity) { velocity_ = velocity; }

	float GetRadius() const { return kRadius; }

	KamataEngine::Vector3 GetWorldRotation() const { return worldTransform_.rotation_; }

	bool IsDead() const { return isDead_; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }

	KamataEngine::Vector3 GetWorldPosition();

	AABB GetAABB();

	void RestoreFuel(float amount);

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void OnCollision(const AABB);

private:
	// ワールド変換データ & モデル & カメラ
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	MapChipField* mapChipField_ = nullptr;
	// 物理計算パラメータ
	KamataEngine::Vector3 velocity_ = {0.0f, 0.0f, 0.0f};

	static inline const float kRadius = 0.6f; // 当たり判定の半径
	// 回転速度
	float kSpinSpeed = 0.1f;
	// 前進速度
	static inline const float kForwardSpeed = 0.1f;

	KamataEngine::Vector3 positionPrev_; // 前の位置

	bool isDead_ = false;
};