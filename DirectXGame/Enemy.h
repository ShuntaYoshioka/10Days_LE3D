#pragma once
#include "KamataEngine.h"
#include "MyMath.h"
#include "MapChipField.h"


class Player;

class PlayerAttack;

class MapChipField;

enum class EnemyState {
	kMoving, 
	kCircling 
};

class Enemy {
public:
	/// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();

	// 歩行スピード
	static inline const float kWalkSpeed = 0.03f;

	 static inline const float kGravity = 0.05f;  // 重力加速度
	static inline const float kGroundY = 0.0f;   // 接地高さ（地面のy座標）

	float walkTimer_ = 0.0f;

	KamataEngine::Vector3 velocity_ = {};
	// AABBを取得
	AABB GetAABB();
	KamataEngine::Vector3 GetWorldPosition();

	//衝突応答
	void OnCollision(const Player* player);

	// 衝突応答
	void OnCollision(const PlayerAttack* playerAttack);

	  void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

		// げった
	bool isDead() const { return isDead_; }


private:
	KamataEngine::WorldTransform worldTransform_; // ワールドトランスふぉーむ
	KamataEngine::Model* model_ = nullptr;        // モデル
	KamataEngine::Camera* camera_ = nullptr;      // カメラ
	MapChipField* mapChipField_ = nullptr;

	EnemyState state_ = EnemyState::kMoving;

	float circleAngle_ = 0.0f; 

	
	// ですフラグ
	bool isDead_ = false;
};
