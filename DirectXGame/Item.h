#pragma once

#include "WorldTransform.h"
#include "MT.h"

class Item {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:

	WorldTransform worldTrasform_;


};
