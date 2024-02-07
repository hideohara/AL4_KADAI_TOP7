#include "Player.h"
#include <cassert>

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {
	// NULLポインタチェック
	assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);

	// 引数として受け取ったデータをメンバ変数に記録する
	modelFighterBody_ = modelBody;
	modelFighterHead_ = modelHead;
	modelFighterL_arm_ = modelL_arm;
	modelFighterR_arm_ = modelR_arm;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransform_L_arm_.Initialize();
	worldTransform_R_arm_.Initialize();

	// 親子構造
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransform_L_arm_.parent_ = &worldTransformBody_;
	worldTransform_R_arm_.parent_ = &worldTransformBody_;

	// 位置調整
	worldTransformHead_.translation_.y = 1.5038f;
	worldTransform_L_arm_.translation_.x = -0.5275f;
	worldTransform_L_arm_.translation_.y = 1.2619f;
	worldTransform_R_arm_.translation_.x = +0.5275f;
	worldTransform_R_arm_.translation_.y = 1.2619f;

	input_ = Input::GetInstance();
	InitializeFloatingGimmick();
}

void Player::Update() {
	const float speed = 0.3f;
	Vector3 move = {0.0f, 0.0f, 0.0f};
	if (input_->PushKey(DIK_RIGHT)) {
		move.x = 1.0f;
	}
	if (input_->PushKey(DIK_LEFT)) {
		move.x = -1.0f;
	}
	if (input_->PushKey(DIK_UP)) {
		move.z = 1.0f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.z = -1.0f;
	}

	move = Normalize(move) * speed;

	worldTransform_.translation_ += move;

	// 移動ベクトルのY軸周り角度
	worldTransform_.rotation_.y = std::atan2(move.x, move.z);

	UpdateFloatingGimmick();

	// 行列を更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransform_L_arm_.UpdateMatrix();
	worldTransform_R_arm_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	modelFighterBody_->Draw(worldTransformBody_, viewProjection);
	modelFighterHead_->Draw(worldTransformHead_, viewProjection);
	modelFighterL_arm_->Draw(worldTransform_L_arm_, viewProjection);
	modelFighterR_arm_->Draw(worldTransform_R_arm_, viewProjection);
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	// 最もシンプルなアニメーション処理
	floatingParameter_ += 0.1f;
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * 0.2f;
	worldTransform_L_arm_.rotation_.x = std::sin(floatingParameter_) * 0.5f;
	worldTransform_R_arm_.rotation_.x = std::sin(floatingParameter_) * 0.5f;
}
