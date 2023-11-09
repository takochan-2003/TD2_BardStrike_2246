#include "Skydome.h"

void Skydome::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Skydome::Update() {
	const float kSpeed = 0.15f;
	Vector3 move = {0.0f, 0.0f, kSpeed};

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
