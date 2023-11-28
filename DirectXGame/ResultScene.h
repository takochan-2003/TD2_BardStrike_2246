﻿#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Scene.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
class ResultScene {
public:
	void Initialize();

	void Updata();

	void Draw();

	bool IsSceneEnd() { return isSceneEnd; }

	Scene NextScene() { return Scene::TITLE; }

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	// シーンを終わらせるフラグ
	bool isSceneEnd = false;
};
