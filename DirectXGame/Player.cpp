#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(Model* model,Vector3 position) {
	// NULLポインタチェック
	assert(model);
	// 引数からデータを受け取る
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = position;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {

	// 移動処理
	worldTransform_.translation_.z += 0.1f;

	worldTransform_.UpdateMatrix();


	ImGui::Begin("Player");
	ImGui::Text(
	    " posX % f,posY %f,posZ %f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);

	ImGui::End();

}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}



const WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
