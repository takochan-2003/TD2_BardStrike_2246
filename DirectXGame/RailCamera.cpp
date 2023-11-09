#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 position, Vector3 rotation) {

	//// X,Y,Z方向の平行移動を設定
	worldTransform_.translation_ = position;

	// X,Y,Z方向の回転を設定
	worldTransform_.rotation_ = rotation;

	// ワールドトランスフォームの初期化
	viewProjection_.Initialize();

}

void RailCamera::Update() {
	worldTransform_.translation_.z += 0.1f;
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    {1.0f, 1.0f, 1.0f}, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	ImGui::Begin("Camera");
	
	ImGui::Text(
	    " posX % f,posY %f,posZ %f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);

	ImGui::Text(
	    "rotation.x %f,rotation.y %f,rotation.z %f", worldTransform_.rotation_.x,
	    worldTransform_.rotation_.y, worldTransform_.rotation_.z);

	ImGui::End();

}
