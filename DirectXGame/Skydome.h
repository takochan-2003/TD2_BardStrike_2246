#pragma once
#include "Model.h"
#include "MT.h"
#include "WorldTransform.h"

class Skydome {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);
	// ワールド座標を取得
	Vector3 GetWorldPosition();
	// getter
	float GetRadius() { return radius_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	// 半径
	float radius_ = 320;
};