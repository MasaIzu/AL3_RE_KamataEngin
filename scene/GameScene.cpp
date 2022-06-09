#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include"AffinTransformation.h"
#include <random>


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

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float value = dist(engine);

	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, MathUtility::PI);

	for (WorldTransform& worldTransform : worldTransforms_) {

		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		
		// X,Y,Z 軸回りの回転角を設定
		worldTransform.rotation_ = { rotDist(engine), rotDist(engine), rotDist(engine) };
		// X,Y,Z軸の周りの平行移動を設定
		worldTransform.translation_ = { dist(engine), dist(engine), dist(engine) };
		//合成の計算
		worldTransform.matWorld_.m[0][0] = 1.0f;
		worldTransform.matWorld_.m[1][1] = 1.0f;
		worldTransform.matWorld_.m[2][2] = 1.0f;
		worldTransform.matWorld_.m[3][3] = 1.0f;

#pragma region スケーリング

		//Matrix4 mat = MathUtility::Matrix4Identity();

		worldTransform.matWorld_ = AffinTrans::Initialize();
		worldTransform.matWorld_ *= AffinTrans::Scale(vector3(worldTransform.scale_.x, worldTransform.scale_.y, worldTransform.scale_.z));

#pragma endregion

		// Z軸回転
		worldTransform.matWorld_ *= AffinTrans::Rotation(vector3(0,0,worldTransform.rotation_.z),3);

		// X軸回転
		worldTransform.matWorld_ *= AffinTrans::Rotation(vector3(worldTransform.rotation_.x, 0, 0), 1);

		// Y軸回転
		worldTransform.matWorld_ *= AffinTrans::Rotation(vector3(0, worldTransform.rotation_.y, 0), 2);

		//平行移動
		worldTransform.matWorld_ *= AffinTrans::Move(vector3(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z));

		//行列の転送
		worldTransform.TransferMatrix();
	}

	//カメラの視点座標
	viewProjection_.eye = { 0, 0, -10 };

	//カメラ注視視点座標を設定
	viewProjection_.target = { 10,0,0 };

	//カメラ上方向ベクトルを設定(右上45度指定)
	viewProjection_.up = { cosf(PI / 4.0f),sinf(PI / 4.0f),0.0f };

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

#pragma region スケーリング

	//Matrix4 mat = MathUtility::Matrix4Identity();

	//worldTransform_.matWorld_ = AffinTrans::Initialize();
	//worldTransform_.matWorld_ *= AffinTrans::Scale(vector3(1.0f, 4.0f, 1.0f));

	////行列の転送
	//worldTransform_.TransferMatrix();
#pragma endregion

#pragma region Rotation_X

	//worldTransform_.matWorld_ = AffinTrans::Initialize();
	//worldTransform_.matWorld_ *= AffinTrans::Rotation(vector3(PI / 4, 0.0f, 0.0f), 1);

	////行列の転送
	//worldTransform_.TransferMatrix();
#pragma endregion

#pragma region Rotation_Y
	
	//worldTransform_.matWorld_ = AffinTrans::Initialize();
	//worldTransform_.matWorld_ *= AffinTrans::Rotation(vector3(0.0f, PI / 4, 0.0f), 2);

	////行列の転送
	//worldTransform_.TransferMatrix();
#pragma endregion

#pragma region Rotation_Z
	//
	//worldTransform_.matWorld_ = AffinTrans::Initialize();
	//worldTransform_.matWorld_ *= AffinTrans::Rotation(vector3(0.0f, 0.0f, PI / 4), 3);

	////行列の転送
	//worldTransform_.TransferMatrix();
#pragma endregion

#pragma region Rotation_X_Y_Z
	
	//worldTransform_.matWorld_ = AffinTrans::Initialize();
	//worldTransform_.matWorld_ *= AffinTrans::Rotation(vector3(PI / 4, PI / 4, PI / 4), 6);

	////行列の転送
	//worldTransform_.TransferMatrix();
#pragma endregion

#pragma region move
	
	//worldTransform_.matWorld_ = AffinTrans::Initialize();
	//worldTransform_.matWorld_ *= AffinTrans::Move(vector3(10.0f, 0.0f, 0.0f));

	////行列の転送
	//worldTransform_.TransferMatrix();
#pragma endregion

}

void GameScene::Update() {
	debugCamera_->Update();

	Vector3 move = vector3(0,0,0);

	//視点移動の速さ
	const float kEyeSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		move.z += kEyeSpeed;
	}
	else if (input_->PushKey(DIK_S)) {
		move.z -= kEyeSpeed;
	}
	//視点移動(ベクトルの加算)
	viewProjection_.eye += move;

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバック用表現
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move.x += kEyeSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x -= kEyeSpeed;
	}
	//視点移動(ベクトルの加算)
	viewProjection_.target += move;

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバック用表現
	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	//上方向の回転速さ[ラジアン/frame]
	const float kUpRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if(input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed;
		//2πを超えたら0に戻す
		viewAngle = fmod(viewAngle, PI * 2.0f);
	}

	//上方向ベクトルを計算(半径1の円周上の座標)
	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };
	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバック用表示
	debugText_->SetPos(50, 90);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

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
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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

