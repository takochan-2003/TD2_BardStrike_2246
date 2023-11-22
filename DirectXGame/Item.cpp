#include "Item.h"
#include <cassert>

void Item::Initialize(Model* model,Vector3 position) {
	// NULLポインタチェック
	assert(model);
	// 引数からデータを受け取る
	model_ = model;
	//ポジション
	worldTransform_.translation_ = position;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	 worldTransform_.UpdateMatrix();
}

void Item::Update() { worldTransform_.UpdateMatrix(); }

void Item::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }
