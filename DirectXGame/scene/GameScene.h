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
#include "Scene.h"

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
	void CheckAllCollision();

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

	bool IsSceneEnd() { return isSceneEnd; }

	void StopBGM();

	Scene NextScene() { return Scene::RESULT; }

	/// <summary>
	/// スコアの描画処理
	/// </summary>
	void GamePlayDraw2DNear();

	/// <summary>
	/// スコアの計算と描画処理
	/// </summary>
	void DrawScore();

	// getter
	int GetScore() { return gameScore_; }

	void Reset();

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

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

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

	int SceneEndTitle = 0;

	Vector3 PlayerPosition;

		// シーンを終わらせるフラグ
	bool isSceneEnd = false;

	// ナンバー
	uint32_t textureHandleNumber = 0;
	Sprite* spriteNumber_[4] = {};

	// スコア
	uint32_t textureHandleSCORE = 0;
	Sprite* spriteScore = {};
	int gameScore_ = 0;

	//BGM
	uint32_t BGM_ = 0;
	uint32_t Sound_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
