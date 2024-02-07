#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"

class FollowCamera {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

    void SetTarget(const WorldTransform* target) { target_ = target; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;

    // 追従対象
	const WorldTransform* target_ = nullptr;
};
