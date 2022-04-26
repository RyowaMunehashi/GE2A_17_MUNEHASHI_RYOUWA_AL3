#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	//平行移動の追加
	worldTransform_[0].translation_ = {-4.5, -2.5, 0};
	worldTransform_[1].translation_ = {0, 5, 0};
	worldTransform_[2].translation_ = {4.5, -2.5, 0};

	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		//スケーリング設定
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		//回転角の設定
		worldTransform_[i].rotation_ = {0, 0, 0};

		worldTransform_[i].Initialize();
	}

	//カメラ視点座標
	viewProjection_.eye = {0, 0, -50};

	//カメラ注視点座標
	viewProjection_.target = {10, 0, 0};

	//カメラ上方向ベクトルを設定
	viewProjection_.up = {0, 1, 0.0f};

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	//視点移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	if (input_->TriggerKey(DIK_SPACE)) {
		viewScene++;
		if (viewScene >= 3) {
			viewScene = 0;
		}
	}

	move = {
	  worldTransform_[viewScene].translation_.x, worldTransform_[viewScene].translation_.y,
	  worldTransform_[viewScene].translation_.z};

	//注視点移動
	viewProjection_.target.x = move.x;
	viewProjection_.target.y = move.y;
	viewProjection_.target.z = move.z;
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
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
	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
