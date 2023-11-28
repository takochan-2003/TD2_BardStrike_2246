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

	// CSVファイル読み込み
	LoadPointPopData();

	// 自キャラの生成と初期化処理
	player_ = std::make_unique<Player>();
	// 3Dモデルの生成
	modelPlayer_.reset(Model::CreateFromOBJ("Player", true));
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

	// カメラの向きと自機の向きをそろえる
	player_->SetViewRotate(followCamera_->GetViewRotate());

	// 自キャラの更新
	player_->Update();
	// スカイドーム
	skydome_->Update();

	// CSVファイルの更新処理
	UpdataPointPopCommands();

	// ビュープロジェクション行列の転送
	viewProjection_.TransferMatrix();

	CheckAllCollision();

	// デスフラグの立ったアイテムを削除
	items_.remove_if([](const std::unique_ptr<Item>& item) {
		if (item->IsDead()) {
			//delete item;
			return true;
		}
		return false;
	});

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

	// std::list<std::unique_ptr<Item>> items_;
	// std::unique_ptr<Item> items_[];
	for (const std::unique_ptr<Item>& item : items_) {
		item->Draw(viewProjection_);
	}

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

void GameScene::CheckAllCollision() { 
	Vector3 posA, posB;

	for (const std::unique_ptr<Item>& item : items_) {
		posA = player_->GetWorldPosition();

		posB = item->GetWorldPosition();

		float Hit = (posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) +
		            (posA.z - posB.z) * (posA.z - posB.z);

		float Radius =
		    (player_->GetRadius() + item->GetRadius()) * (player_->GetRadius() + item->GetRadius()); 

		if (Hit <= Radius) {
			item->OnCollision();
		}
	}
}

void GameScene::LoadPointPopData() {
	std::ifstream file;
	file.open("Resources/ItemPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	pointPopCommnds << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdataPointPopCommands() {

	// 待機処理
	if (standFlag) {
		standTime--;
		if (standTime <= 0) {
			// 待機完了
			standFlag = false;
		}
		return;
	}

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
		} // WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機時間
			standFlag = true;
			standTime = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::PointGenerate(Vector3 position) {

	// アイテムの生成と初期化処理
	Item* item = new Item();
	item->Initialize(modelItem_.get(), position);

	items_.push_back(static_cast<std::unique_ptr<Item>>(item));
}
