#include "FollowCamera.h"

void FollowCamera::Initialize() { 
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	// 追従対象がいれば
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	
	}
	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
}
