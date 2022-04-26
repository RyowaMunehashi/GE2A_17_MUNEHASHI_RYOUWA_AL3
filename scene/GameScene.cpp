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
	debugText_eye = DebugText::GetInstance();
	debugText_traget = DebugText::GetInstance();
	debugText_up = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	//乱数
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(座標)
	std::uniform_real_distribution<float> posDice(-10.0f, 10.0f);

	worldTransform_.Initialize();

	for (int i = 0; i < 3; i++) {
		//カメラ視点座標
		viewProjection_[i].eye = {posDice(engine), posDice(engine), posDice(engine)};

		//カメラ注視点座標
		viewProjection_[i].target = {0.0f, 0.0f, 0.0f};

		//カメラ上方向ベクトルを設定
		viewProjection_[i].up = {0.0f, 1.0f, 0.0f};

		//ビュープロジェクションの初期化
		viewProjection_[i].Initialize();
	}
}

void GameScene::Update() {
	//押した方向で移動ベクトルを変更
	if (input_->TriggerKey(DIK_SPACE)) {
		viewSecen++;
		if (viewSecen >= 3) {
			viewSecen = 0;
		}
	}
	debugText_eye->SetPos(50, 30);
	debugText_eye->Printf("Camera1");
	debugText_eye->SetPos(50, 50);
	debugText_eye->Printf(
	  "eye:(%f,%f,%f)", viewProjection_[0].eye.x, viewProjection_[0].eye.y,
	  viewProjection_[0].eye.z);
	debugText_eye->SetPos(50, 70 );
	debugText_eye->Printf(
	  "target:(%f,%f,%f)", viewProjection_[0].target.x, viewProjection_[0].target.y,
	  viewProjection_[0].target.z);
	debugText_eye->SetPos(50, 90);
	debugText_eye->Printf(
	  "up:(%f,%f,%f)", viewProjection_[0].up.x, viewProjection_[0].up.y, viewProjection_[0].up.z);
	debugText_traget->SetPos(50, 140);
	debugText_traget->Printf("Camera2");
	debugText_traget->SetPos(50, 160);
	debugText_traget->Printf(
	  "eye:(%f,%f,%f)", viewProjection_[1].eye.x, viewProjection_[1].eye.y,
	  viewProjection_[1].eye.z);
	debugText_traget->SetPos(50, 180);
	debugText_traget->Printf(
	  "target:(%f,%f,%f)", viewProjection_[1].target.x, viewProjection_[1].target.y,
	  viewProjection_[1].target.z);
	debugText_traget->SetPos(50, 200);
	debugText_traget->Printf(
	  "up:(%f,%f,%f)", viewProjection_[1].up.x, viewProjection_[1].up.y, viewProjection_[1].up.z);
	debugText_up->SetPos(50, 230);
	debugText_up->Printf("Camera3");
	debugText_up->SetPos(50, 250);
	debugText_up->Printf(
	  "eye:(%f,%f,%f)", viewProjection_[2].eye.x, viewProjection_[2].eye.y,
	  viewProjection_[2].eye.z);
	debugText_up->SetPos(50, 270);
	debugText_up->Printf(
	  "target:(%f,%f,%f)", viewProjection_[2].target.x, viewProjection_[2].target.y,
	  viewProjection_[2].target.z);
	debugText_up->SetPos(50, 290);
	debugText_up->Printf(
	  "up:(%f,%f,%f)", viewProjection_[2].up.x, viewProjection_[2].up.y, viewProjection_[2].up.z);
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
	model_->Draw(worldTransform_, viewProjection_[viewSecen], textureHandle_);

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
	debugText_eye->DrawAll(commandList);
	debugText_traget->DrawAll(commandList);
	debugText_up->DrawAll(commandList);
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
