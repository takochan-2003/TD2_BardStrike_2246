﻿#include "Player.h"
#include <cassert>
#include"ImGuiManager.h"

void Player::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	// 引数からデータを受け取る
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}
void Player::Update() {

	// 移動処理
	// KeyMove();
	JoyMove();


}
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void Player::KeyMove() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Player::RailMove() {
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	// Vector3 move = {0, 0, 0};
	//  ゲームパッド状態取得変数
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 移動速度
		const float kCharacterSpeed = 0.5f;
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed,
		    0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed,
		};

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


	worldTransform_.UpdateMatrix();
}

void Player::JoyMove() {
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	// Vector3 move = {0, 0, 0};
	//  ゲームパッド状態取得変数
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 移動速度
		const float kCharacterSpeed = 0.5f;
		//↓変更点
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed,
		    0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed,
		};

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotateXMatrix = MakeRotateXmatrix(viewProjection_->rotation_.x);
		Matrix4x4 rotateYMatrix = MakeRotateYmatrix(viewProjection_->rotation_.y);
		Matrix4x4 rotateZMatrix = MakeRotateZmatrix(viewProjection_->rotation_.z);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		// 移動量に速さを反映
		move = Multiply(kCharacterSpeed,Normalize(move));

		move = TransformNormal(move, rotateXYZMatrix);

		if (move.z != 0 || move.y != 0) {
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}

		//移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}

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
