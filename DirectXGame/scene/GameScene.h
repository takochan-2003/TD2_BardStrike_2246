#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "FollowCamera.h"
#include "Input.h"
#include "Item.h"
#include "Model.h"
#include "Player.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// ポイントアイテム発生データを読み込み
	/// </summary>
	void LoadPointPopData();

	/// <summary>
	/// ポイントアイテム発生コマンドの更新
	/// </summary>
	void UpdataPointPopCommands();

	/// <summary>
	/// ポイントアイテムの生成
	/// </summary>
	/// <param name="position"></param>
	void PointGenerate(Vector3 position);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// 自キャラの3Dモデル
	std::unique_ptr<Model> modelPlayer_;
	// スカイドームの3Dモデル
	std::unique_ptr<Model> modelSkydome_;
	// アイテムの3Dモデル
	std::unique_ptr<Model> modelItem_;

	// ビュープロダクション
	ViewProjection viewProjection_;

	// 自キャラ
	std::unique_ptr<Player> player_;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;

	////ポイントアイテム
	// std::unique_ptr<Item> item_;

	// 　ポイントアイテム発生コマンド
	std::stringstream pointPopCommnds;
	// アイテム
	std::list<std::unique_ptr<Item>> items_;

private:
	// 待機時間
	bool standFlag = false;
	int standTime = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
