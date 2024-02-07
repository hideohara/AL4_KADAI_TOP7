#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Ground::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Ground::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
