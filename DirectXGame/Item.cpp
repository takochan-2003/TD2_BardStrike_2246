#include "Item.h"
#include <cassert>

void Item::Initialize(Model* model, Vector3 position) {
	// NULLポインタチェック
	assert(model);
	// 引数からデータを受け取る
	model_ = model;
	// ポジション
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {9, 9, 9};

	audio_ = Audio::GetInstance();
	deathBird_ = audio_->LoadWave("get.wav");

	// ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.UpdateMatrix();
}

void Item::Update() { worldTransform_.UpdateMatrix(); }

void Item::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

Vector3 Item::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Item::OnCollision() { 
	isDead_ = true; 
	audio_->PlayWave(deathBird_);
}
