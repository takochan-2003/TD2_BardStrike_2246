#pragma once

#include "WorldTransform.h"
#include "MT.h"

class Item {
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize();

	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw();

private:

	WorldTransform worldTrasform_;


};
