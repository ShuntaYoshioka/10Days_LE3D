#include "Particle.h"
#include <cstdlib> // std::rand 用

void Particle::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
	model_ = model;
	camera_ = camera;
	particles_.clear();
}

// 指定した範囲 (min_val ～ max_val) の float 乱数を返すヘルパー関数
static float GetRandomFloat(float min_val, float max_val) {
	float rate = static_cast<float>(std::rand()) / RAND_MAX; // 0.0 ～ 1.0
	return min_val + rate * (max_val - min_val);
}

void Particle::SpawnExplosion(const KamataEngine::Vector3& position, int count) {
	for (int i = 0; i < count; ++i) {
		ParticleData p;
		p.transform.Initialize();
		p.transform.translation_ = position;

		// rand() を使って全方向に散らばる速度ベクトルを生成
		p.velocity = {
		    GetRandomFloat(-0.3f, 0.3f), GetRandomFloat(0.0f, 0.4f), // 上方向に少し跳ね上がる
		    GetRandomFloat(-0.3f, 0.3f)};

		// ランダムな初期サイズと寿命
		float scale = GetRandomFloat(0.2f, 0.5f);
		p.transform.scale_ = {scale, scale, scale};
		p.maxLifeTime = GetRandomFloat(0.3f, 0.6f);
		p.timer = 0.0f;
		p.isDead = false;

		particles_.push_back(p);
	}
}

void Particle::Update() {
	// 1. 各パーティクルの移動・回転・拡大処理
	for (ParticleData& p : particles_) {
		if (p.isDead) {
			continue;
		}

		p.timer += 1.0f / 60.0f;

		// 寿命チェック
		if (p.timer >= p.maxLifeTime) {
			p.isDead = true;
			continue;
		}

		// 位置の更新
		p.transform.translation_.x += p.velocity.x;
		p.transform.translation_.y += p.velocity.y;
		p.transform.translation_.z += p.velocity.z;

		// 回転処理
		p.transform.rotation_.y = 3.14f;
		p.transform.rotation_.z += 0.1f;

		// 拡大処理（一気に広がる爆発感）
		p.transform.scale_.x *= 1.05f;
		p.transform.scale_.y *= 1.07f;
		p.transform.scale_.z *= 1.05f;

		// 行列更新
		p.transform.matWorld_ = MakeAffineMatrix(p.transform.scale_, p.transform.rotation_, p.transform.translation_);
		p.transform.TransferMatrix();
	}

	// 2. 寿命が切れた（isDead == true）パーティクルをまとめて削除
	particles_.remove_if([](const ParticleData& p) { return p.isDead; });
}

void Particle::Draw() {
	if (!model_ || !camera_)
		return;

	// イテレータを使わず、直接参照で描画
	for (ParticleData& p : particles_) {
		if (!p.isDead) {
			model_->Draw(p.transform, *camera_);
		}
	}
}