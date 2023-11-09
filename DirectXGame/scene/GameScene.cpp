#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルの生成
	modelPlayer_.reset(Model::CreateFromOBJ("cube", true));

	// ビューポートプロジェクションの初期化
	viewProjection_.Initialize();

	//// ワールドトランスフォームの初期化
	//worldTransform_.Initialize();

	// 自キャラの生成と初期化処理
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());

	//スカイドームの生成と初期化処理
	skydome_ = new Skydome();
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(skydomeModel_);


}

void GameScene::Update() {

		// 自キャラの更新
	player_->Update();

	// ビュープロジェクション行列の転送
	viewProjection_.TransferMatrix();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

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

	// プレイヤーの描画
	player_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
