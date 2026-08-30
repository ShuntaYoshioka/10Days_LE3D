#pragma once
#include "MyMath.h"
#include <KamataEngine.h>

// 粒1つの構造体
struct ParticleData {
	KamataEngine::WorldTransform transform;
	KamataEngine::Vector3 velocity;
	float timer = 0.0f;
	float maxLifeTime = 0.0f;
	bool isDead = false;

};

class Particle {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Update();
	void Draw();

	// 爆発用
	void SpawnExplosion(const KamataEngine::Vector3& position, int count = 20);

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	std::list<ParticleData> particles_;
};