#include "GraphBar.h"
#include <algorithm>

using namespace KamataEngine;

GraphBar::~GraphBar() {
}

void GraphBar::Initialize(uint32_t textureHandle) {
	textureHandle_ = textureHandle;
	// 画面左上（X=50, Y=20）に配置
}

void GraphBar::Update(float currentFuel, float maxFuel) {
	// 燃料の割合（0.0 〜 1.0）を計算
	float fuelRate = 0.0f;
	if (maxFuel > 0.0f) {
		fuelRate = std::clamp(currentFuel / maxFuel, 0.0f, 1.0f);
	}

}

void GraphBar::Draw() {
}