#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	//// 自キャラの解放
	//	delete player_;
	//
	//
	//	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome"));
	modelGround_.reset(Model::CreateFromOBJ("ground"));
	modelEnemy_.reset(Model::CreateFromOBJ("needle_Body"));

	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body"));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head"));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm"));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm"));

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.translation_ = {0.0f, 2.0f, -10.0f};
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	player_->Initialize(
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get());
	// スカイドームの生成
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());
	// 地面の生成
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());
	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	// 敵の生成
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(modelEnemy_.get());
}

void GameScene::Update() {
	// 各クラスの更新
	player_->Update();
	skydome_->Update();
	ground_->Update();
	enemy_->Update();
	followCamera_->Update();

	// 追従カメラのビュー行列をゲームシーンのビュープロジェクションにコピー;
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	// 追従カメラのプロジェクション行列をゲームシーンのビュープロジェクションにコピー;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	// ゲームシーンのビュープロジェクション行列の転送処理
	viewProjection_.TransferMatrix();

	// 衝突判定
	if (enemy_->GetY() == 0) {
		float dx = abs(player_->GetX() - enemy_->GetX());
		float dz = abs(player_->GetZ() - enemy_->GetZ());
		if (dx < 2 && dz < 2) {
			enemy_->Hit();
		}
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 各クラスの描画
	player_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
