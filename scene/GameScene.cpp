#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

const float PI = 3.141592f;

GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

#pragma region スケーリング
	//X,Y,Z 方向のスケーリングを設定
	//worldTransform_.scale_ = { 1.0f,5.0f,1.0f };
	////スケーリング行列を宣言
	//Matrix4 matScale;

	//matScale = {
	//	worldTransform_.scale_.x,0.0f,0.0f,0.0f,
	//	0.0f,worldTransform_.scale_.y,0.0f,0.0f,
	//	0.0f,0.0f,worldTransform_.scale_.z,0.0f,
	//	0.0f,0.0f,0.0f,1.0f
	//};

	//Matrix4 matScale2;

	//matScale2 = {
	//	1.0f,0.0f,0.0f,0.0f,
	//	0.0f,1.0f,0.0f,0.0f,
	//	0.0f,0.0f,1.0f,0.0f,
	//	0.0f,0.0f,0.0f,1.0f
	//};

	worldTransform_.matWorld_ = affin->Initialize();
	worldTransform_.matWorld_ *= affin->Scale(vector3(1.0f, 4.0f, 1.0f));

	//行列の転送
	worldTransform_.TransferMatrix();
#pragma endregion

#pragma region Rotation_X
	////X,Y,Z軸周りの回転角を設定
	//worldTransform_.rotation_ = { PI / 4,0.0f,0.0f };
	////Z軸回転行列を宣言
	//Matrix4 matRotX;
	//matRotX = {
	//	1.0f,0.0f,0.0f,0.0f,
	//	0.0f,cos(worldTransform_.rotation_.x),  sin(worldTransform_.rotation_.x),0.0f,
	//	0.0f,-sin(worldTransform_.rotation_.x), cos(worldTransform_.rotation_.x),0.0f,
	//	0.0f,0.0f,0.0f,1.0f
	//};

	//Matrix4 matRotX2;
	//matRotX2 = {
	//	1.0f,0.0f,0.0f,0.0f,
	//	0.0f,1.0f,0.0f,0.0f,
	//	0.0f,0.0f,1.0f,0.0f,
	//	0.0f,0.0f,0.0f,1.0f
	//};

	worldTransform_.matWorld_ = affin->Initialize();
	worldTransform_.matWorld_ *= affin->Rotation(vector3(PI / 4, 0.0f, 0.0f), 1);

	//行列の転送
	worldTransform_.TransferMatrix();
#pragma endregion

#pragma region Rotation_Y
	//	//X,Y,Z軸周りの回転角を設定
	//	worldTransform_.rotation_ = { 0.0f,PI / 4,0.0f };
	//	//Y軸回転行列を宣言
	//	Matrix4 matRotY;
	//	matRotY = {
	//		cos(worldTransform_.rotation_.y),0.0f,-sin(worldTransform_.rotation_.y),0.0f,
	//		0.0f,1.0f,0.0f,0.0f,
	//		sin(worldTransform_.rotation_.y),0.0f, cos(worldTransform_.rotation_.y),0.0f,
	//		0.0f,0.0f,0.0f,1.0f
	//	};
	//
	//	Matrix4 matRotY2;
	//	matRotY2 = {
	//		1.0f,0.0f,0.0f,0.0f,
	//		0.0f,1.0f,0.0f,0.0f,
	//		0.0f,0.0f,1.0f,0.0f,
	//		0.0f,0.0f,0.0f,1.0f
	//	};
	//
	worldTransform_.matWorld_ = affin->Initialize();
	worldTransform_.matWorld_ *= affin->Rotation(vector3(0.0f, PI / 4, 0.0f), 2);

	//行列の転送
	worldTransform_.TransferMatrix();
#pragma endregion

#pragma region Rotation_Z
	//	//X,Y,Z軸周りの回転角を設定
	//	worldTransform_.rotation_ = { 0.0f,0.0f,PI / 4 };
	//	//Z軸回転行列を宣言
	//	Matrix4 matRotZ;
	//	matRotZ = {
	//		cos(worldTransform_.rotation_.z),sin(worldTransform_.rotation_.z),0.0f,0.0f,
	//		-sin(worldTransform_.rotation_.z),cos(worldTransform_.rotation_.z), 0.0f,0.0f,
	//		0.0f,0.0f, 1.0f,0.0f,
	//		0.0f,0.0f,0.0f,1.0f
	//	};
	//
	//	Matrix4 matRotZ2;
	//	matRotZ2 = {
	//		1.0f,0.0f,0.0f,0.0f,
	//		0.0f,1.0f,0.0f,0.0f,
	//		0.0f,0.0f,1.0f,0.0f,
	//		0.0f,0.0f,0.0f,1.0f
	//	};
	//
	worldTransform_.matWorld_ = affin->Initialize();
	worldTransform_.matWorld_ *= affin->Rotation(vector3(0.0f, 0.0f, PI / 4), 3);

	//行列の転送
	worldTransform_.TransferMatrix();
#pragma endregion

#pragma region Rotation_X_Y_Z
	//	//X,Y,Z軸周りの回転角を設定
	//	worldTransform_.rotation_ = { PI / 4,PI / 4,PI / 4 };
	//	Matrix4 matRotXYZ;
	//	Matrix4 matRot_X, matRot_Y, matRot_Z;
	//	matRot_X = {
	//		1.0f,0.0f,0.0f,0.0f,
	//		0.0f,cos(worldTransform_.rotation_.x),  sin(worldTransform_.rotation_.x),0.0f,
	//		0.0f,-sin(worldTransform_.rotation_.x), cos(worldTransform_.rotation_.x),0.0f,
	//		0.0f,0.0f,0.0f,1.0f
	//	};
	//	matRot_Y = {
	//		cos(worldTransform_.rotation_.y),0.0f,-sin(worldTransform_.rotation_.y),0.0f,
	//		0.0f,1.0f,0.0f,0.0f,
	//		sin(worldTransform_.rotation_.y),0.0f, cos(worldTransform_.rotation_.y),0.0f,
	//		0.0f,0.0f,0.0f,1.0f
	//	};
	//	matRot_Z = {
	//		cos(worldTransform_.rotation_.z),sin(worldTransform_.rotation_.z),0.0f,0.0f,
	//		-sin(worldTransform_.rotation_.z),cos(worldTransform_.rotation_.z), 0.0f,0.0f,
	//		0.0f,0.0f, 1.0f,0.0f,
	//		0.0f,0.0f,0.0f,1.0f
	//	};
	//	Matrix4 matRot;
	//	matRot = {
	//		1.0f,0.0f,0.0f,0.0f,
	//		0.0f,1.0f,0.0f,0.0f,
	//		0.0f,0.0f,1.0f,0.0f,
	//		0.0f,0.0f,0.0f,1.0f
	//	};
	//	//各軸の回転行列を合成
	//	matRotXYZ = matRot;
	//	matRotXYZ *= matRot_X;
	//	matRotXYZ *= matRot_Y;
	//	matRotXYZ *= matRot_Z;
	//
	worldTransform_.matWorld_ = affin->Initialize();
	worldTransform_.matWorld_ *= affin->Rotation(vector3(PI / 4, PI / 4, PI / 4), 6);

	//行列の転送
	worldTransform_.TransferMatrix();
#pragma endregion

#pragma region move
	//
	//	//X,Y,Z軸回りの平行移動を設定
	//	worldTransform_.translation_ = { 10,0,0 };
	//	//平行移動を宣言
	//	Matrix4 matTrans = MathUtility::Matrix4Identity();
	//
	//	Matrix4 matMove;
	//	matMove = {
	//		1.0f,0.0f,0.0f,0.0f,
	//		0.0f,1.0f,0.0f,0.0f,
	//		0.0f,0.0f,1.0f,0.0f,
	//		worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z,1.0f
	//	};
	//
	worldTransform_.matWorld_ = affin->Initialize();
	worldTransform_.matWorld_ *= affin->Move(vector3(10.0f, 0.0f, 0.0f));

	//行列の転送
	worldTransform_.TransferMatrix();
#pragma endregion

}

void GameScene::Update() {
	debugCamera_->Update();
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
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

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
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->DrawLine3d(vector3(0, 0, 0), vector3(200, 0, 200), vector4(255, 255, 255, 100));
}

Vector3 GameScene::vector3(float x, float y, float z)
{
	return Vector3(x, y, z);
}

Vector4 GameScene::vector4(int x, int y, int z, int w)
{
	return Vector4(x, y, z, w);
}

