#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 自機の3Dモデルの生成
	modelPlayer_.reset(Model::CreateFromOBJ("cube", true));
	// スカイドームの3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	// アイテムの3Dモデルの生成
	modelItem_.reset(Model::CreateFromOBJ("cube", true));

	// ビューポートプロジェクションの初期化
	viewProjection_.Initialize();

	//// ワールドトランスフォームの初期化
	// worldTransform_.Initialize();

	// 自キャラの生成と初期化処理
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_.get());
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	// スカイドームの生成と初期化処理
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	// 追従カメラの生成と初期化処理
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// 自キャラのワールドトランスフォームを追従カメラのセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	// 自キャラに追従カメラをアドレス渡し
	player_->SetViewProjection(&followCamera_->GetViewProjection());
}

void GameScene::Update() {

	// 追従カメラの更新
	followCamera_->Update();

	// ビュープロジェクションの反映
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	// 自キャラの更新
	player_->Update();
	// スカイドーム
	skydome_->Update();

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

void GameScene::LoadPointPopData() {
	std::ifstream file;
	file.open("");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	pointPopCommnds << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdataPointPopCommands() {

	// 1行分の文字列を入れる変数
	 std::string line;

	// コマンド実行ループ
	 while (getline(pointPopCommnds, line)) {
		std::istringstream line_stream(line);

		std::string word;
		// 　,区切りで行の先頭文字列を所得

		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			PointGenerate({x, y, z});
		}
	}
}

void GameScene::PointGenerate(Vector3 position) {

	// アイテムの生成と初期化処理
	item_ = std::make_unique<Item>();
	item_->Initialize(modelItem_.get(),position);

}
