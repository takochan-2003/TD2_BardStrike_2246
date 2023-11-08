#include "Player.h"
#include "MT.h"

void Player::Initialize(Model* model) {
	assert(model);
	input_ = Input::GetInstance();

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() { 
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// 速さ
		const float speed = 0.3f;
		Matrix4x4 rotation;
		
		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, // Lスティックの横成分
		    0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed, // Lスティックの縦成分
		};

		// 移動量に速さを反映
		move = Multiply(speed, Normalize(move));
		
		// 移動量に速さを反映(θ度の移動ベクトル)
		rotation = MakeRotateYmatrix(viewProjection_->rotation_.y);

		move = Transform(move, rotation);

		if (move.y != 0) {
			worldTransform_.rotation_.y = std::atan2(move.z, move.x);
		}

		// 移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}
}

void Player::Draw(const ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

const WorldTransform& Player::GetWorldTransform() {
	// TODO: return ステートメントをここに挿入します
	return worldTransform_;
}