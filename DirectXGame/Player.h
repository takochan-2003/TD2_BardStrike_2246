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

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	// 移動処理
	void JoyMove();

	void AutoMove();

	void RailMove();

	// ワールドトランスフォーム取得
	const WorldTransform& GetWorldTransform();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	//カメラの向きと自機の向きを合わせる
	void SetViewRotate(const Vector3 parent) { worldTransform_.rotation_ = parent; }

	// getter
	float GetRadius() { return radius_; }

	//スカイドームから出たときの判定
	void SkydomeLeave();

	//元の位置に戻す処理
	void ResetPosition();

private:
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

	//半径
	float radius_ = 32;

};