#include "ResultScene.h"

void ResultScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	// スコア文字テクスチャ
	textureHandleSCORE = TextureManager::Load("score.png");
	// スコアの数字テクスチャ
	textureHandleNumber = TextureManager::Load("number.png");
	// スコアのスプライト描画
	for (int i = 0; i < 4; i++) {
		spriteNumber_[i] = Sprite::Create(textureHandleNumber, {130.0f + i * 26, 10});
	}
	// スコアのスプライト描画
	spriteScore = Sprite::Create(textureHandleSCORE, {0.0f, 10});

	// ビューポートプロジェクションの初期化
	viewProjection_.Initialize();

	// 背景のスプライト
	textureHandle_ = TextureManager::Load("Result.png");
	sprite_ = Sprite::Create(textureHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void ResultScene::Updata() {
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
			Sleep(1 * 300);
			isSceneEnd = true;
		}
	}

}

void ResultScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	sprite_->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	GamePlayDraw2DNear();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void ResultScene::GamePlayDraw2DNear() {
	spriteScore->Draw();
	DrawScore();
}

void ResultScene::DrawScore() {
	int eachNumber[4] = {};
	int number = gameScore_;

	int keta = 1000;
	for (int i = 0; i < 4; i++) {
		eachNumber[i] = number / keta;
		number = number % keta;
		keta = keta / 10;
	}
	for (int i = 0; i < 4; i++) {
		spriteNumber_[i]->SetSize({32, 64});
		spriteNumber_[i]->SetTextureRect({32.0f * eachNumber[i], 0}, {32, 64});
		spriteNumber_[i]->Draw();
	}
}

void ResultScene::Reset() { isSceneEnd = false; }
