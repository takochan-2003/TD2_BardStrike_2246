﻿#include "FollowCamera.h"
#include "MT.h"

FollowCamera::FollowCamera() {}

FollowCamera::~FollowCamera() {}

void FollowCamera::Initialize() {
	input_ = Input::GetInstance();
	viewProjection_.Initialize();
}

void FollowCamera::Update() { // ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// 速さ
		const float rotation = 0.01f;

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotation;
		viewProjection_.rotation_.x -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * rotation;

	}

	// 追従対象がいれば
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		Matrix4x4 rotateXMatrix = MakeRotateXmatrix(viewProjection_.rotation_.x);
		Matrix4x4 rotateYMatrix = MakeRotateYmatrix(viewProjection_.rotation_.y);
		Matrix4x4 rotateZMatrix = MakeRotateZmatrix(viewProjection_.rotation_.z);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		offset = TransformNormal(offset, rotateXYZMatrix);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}

	viewProjection_.UpdateMatrix();
}
