#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialize(Model* model, Vector3 position) {
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
	AutoMove();
}
void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

void Player::JoyMove() {
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	// Vector3 move = {0, 0, 0};
	//  ゲームパッド状態取得変数
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 移動速度
		const float kCharacterSpeed = 0.5f;
		// ↓変更点
		// Vector3 move = {
		//     (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed,
		//     0.0f,
		//     (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed,
		// };

		Vector3 move = {0, 0, 1};

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotateXMatrix = MakeRotateXmatrix(viewProjection_->rotation_.x);
		Matrix4x4 rotateYMatrix = MakeRotateYmatrix(viewProjection_->rotation_.y);
		Matrix4x4 rotateZMatrix = MakeRotateZmatrix(viewProjection_->rotation_.z);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		// 移動量に速さを反映
		move = Multiply(kCharacterSpeed, Normalize(move));

		move = TransformNormal(move, rotateXYZMatrix);

		if (move.z != 0 || move.y != 0) {
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}

		// 移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}

	// 行列を更新
	worldTransform_.UpdateMatrix();
}

void Player::AutoMove() {

	XINPUT_STATE joyState;

	// 移動速度
	const float kCharacterSpeed = 1.0f;
	float moveSpeed = 0.0f;
	Vector3 move = {0, 0, 1};

	// 左スティックでの速度調整処理
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// 変動する移動速度
		moveSpeed = (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;

		// 速度が-にいかないようにする
		if (moveSpeed < -0.6f) {
			moveSpeed = -0.6f;
		}

		ImGui::Begin("player");
		// ImGui::Text("moveSpeed:%f", moveSpeed, 0.0f, 360.0f);

		float Position[3] = {
		    worldTransform_.translation_.x, worldTransform_.translation_.y,
		    worldTransform_.translation_.z};

		ImGui::SliderFloat3("Positon", Position, -300, 300);

		worldTransform_.translation_.x = Position[0];
		worldTransform_.translation_.y = Position[1];
		worldTransform_.translation_.z = Position[2];

		ImGui::End();
	}

	// カメラの角度から回転行列を計算する
	Matrix4x4 rotateXMatrix = MakeRotateXmatrix(viewProjection_->rotation_.x);
	Matrix4x4 rotateYMatrix = MakeRotateYmatrix(viewProjection_->rotation_.y);
	Matrix4x4 rotateZMatrix = MakeRotateZmatrix(viewProjection_->rotation_.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	// 移動量に速さを反映
	move = Multiply(kCharacterSpeed, Normalize(move));

	move.z += moveSpeed;

	move = TransformNormal(move, rotateXYZMatrix);

	if (move.z != 0 || move.y != 0) {
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	ImGui::Begin("Move");
	ImGui::Text("move:%f", move.z, 0.0f, 360.0f);
	ImGui::End();

	// 行列を更新
	worldTransform_.UpdateMatrix();
}

const WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::ResetPosition() {

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::SceneEndResetPosition() {
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Player::SkydomeLeave() { Player::ResetPosition(); }
