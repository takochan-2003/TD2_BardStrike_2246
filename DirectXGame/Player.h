#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MT.h"

class Player {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	//Player();

public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	//カメラの向きをセット
	void SetParent(const WorldTransform* parent);

	void SetViewProjection(const Vector3 parent) { worldTransform_.rotation_; }


	// 移動処理
	void KeyMove();
	void JoyMove();

	void RailMove();

	// ワールドトランスフォーム取得
	const WorldTransform& GetWorldTransform();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;
};