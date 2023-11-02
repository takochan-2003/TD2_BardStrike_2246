#include "Player.h"

void Player::Initialize() {}

void Player::Update() { // ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	}
}

void Player::Draw() {}
