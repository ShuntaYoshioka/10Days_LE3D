#include "NumberDrawer.h"
#include <cstdio>

using namespace KamataEngine;

void NumberDrawer::Initialize(const Vector2& position, float digitSize) {
	position_ = position;
	digitSize_ = digitSize;

	for (uint32_t i = 0; i < 10; ++i) {
		char path[32];
		snprintf(path, sizeof(path), "number/%u.png", i);
		textureHandles_[i] = TextureManager::Load(path);
	}

	for (uint32_t i = 0; i < kMaxDigits; ++i) {
		Vector2 pos = {position_.x + digitSize_ * static_cast<float>(i), position_.y};
		digitSprites_[i] = Sprite::Create(textureHandles_[0], pos);
		digitSprites_[i]->SetSize({digitSize_, digitSize_});
	}

	SetValue(0);
}

void NumberDrawer::SetPosition(const Vector2& position) {
	position_ = position;
	for (uint32_t i = 0; i < kMaxDigits; ++i) {
		digitSprites_[i]->SetPosition({position_.x + digitSize_ * static_cast<float>(i), position_.y});
	}
}

void NumberDrawer::SetValue(uint32_t value) {
	char buf[16];
	int len = snprintf(buf, sizeof(buf), "%u", value);

	digitCount_ = (len > 0) ? static_cast<uint32_t>(len) : 1;
	if (digitCount_ > kMaxDigits) {
		digitCount_ = kMaxDigits;
	}

	for (uint32_t i = 0; i < digitCount_; ++i) {
		int digit = buf[i] - '0';
		digitSprites_[i]->SetTextureHandle(textureHandles_[digit]);
	}
}

void NumberDrawer::Draw() {
	for (uint32_t i = 0; i < digitCount_; ++i) {
		digitSprites_[i]->Draw();
	}
}

NumberDrawer::~NumberDrawer() {
	for (uint32_t i = 0; i < kMaxDigits; ++i) {
		delete digitSprites_[i];
	}
}
