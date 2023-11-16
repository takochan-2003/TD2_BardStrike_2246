#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

class Item {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,Vector3 position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:

	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

};
