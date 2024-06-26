#include "PlayScene.h"
#include "imgui.h"
#include "ChengeScene.h"
#include "Enum.h"

void PlayScene::Update()
{
	//デバッグ表示用
	Debug();

	///ここから更新処理追加




	///ここまで

	//カメラ更新
	matView_.MatUpdate();

	//スクリーン更新
	screen_.MatUpdate(matView_.mat_, matProjection_, ZERO);

	//パーティクル更新
	ParticleManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	ParticleManager::GetInstance()->Update();

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();
}

void PlayScene::Initialize()
{
	//描画用行列
	matView_.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//shader
	shader_.Initizlize(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl");
	bilShader_.Initizlize(L"Resources/shader/VShader.hlsl", L"Resources/shader/PShader.hlsl");

	//pipeline
	pipeline_ = std::make_unique<GPipeline>();
	pipeline_->Initialize(MyDirectX::GetInstance()->GetDev(), shader_);

	//描画初期化
	multipathPipeline_ = std::make_unique<GPipeline>();
	multipathPipeline_->Initialize(MyDirectX::GetInstance()->GetDev(), bilShader_);

	//背景のスクリーン(これが必要なので依存しないようにしたい)
	screen_.Initialize(multipathPipeline_.get(), bilShader_);
	screen_.obj_.trans_.z_ = 100.1f;
	screen_.obj_.scale_ = { Window::window_width_ * 2,Window::window_height_ / 2,0.2f };

	//カメラ初期化
	gameCamera_ = std::make_unique<GameCamera>();

	//パーティクルマネージャー
	ParticleManager::GetInstance()->Initalize();
	ParticleManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	ParticleManager::GetInstance()->SetDraw(shader_, pipeline_.get());

	//白画像
	whiteTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/white1x1.png");

	////透過するかどうか
	normalSpriteCommon_->Inilialize(MyDirectX::GetInstance(), true);

	//画像色
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	//基礎
	sprite_->Inilialize(normalSpriteCommon_, &matProjection_);
}

void PlayScene::Draw()
{
	//Draw
	MyDirectX::GetInstance()->PrevDrawScreen();

	//// 描画コマンド
	MyDirectX::GetInstance()->PostDrawScreen();

	//UIDraw
	MyDirectX::GetInstance()->PrevDraw();

	//スクリーン描画
	screen_.Draw(whiteTex_);

	///ここから描画処理追加




	///ここまで

	//ボックスパーティクル
	ParticleManager::GetInstance()->Draw();

	//jsonファイルから読み込んだものの描画
	LoadObjectData::GetInstance()->Draw();

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

#ifdef _DEBUG

	//ImGui描画
	ImguiManager::GetInstance()->Draw();

#endif _DEBUG

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void PlayScene::Finalize()
{
	//パーティクル削除
	ParticleManager::GetInstance()->Finalize();
}

void PlayScene::BlackOut()
{
	////ブラックアウト
	//if (blackOutFlag_)
	//{
	//	//ブラックアウト
	//	if (color_.x_ < 1.0f)
	//	{
	//		color_.x_ += 0.01f;
	//		color_.y_ += 0.01f;
	//		color_.z_ += 0.01f;
	//		color_.w_ += 0.01f;

	//		if (color_.x_ >= 1.0f)
	//		{
	//			//ステージ読み込み
	//			if (stageCount_ == ONE)
	//			{
	//				LoadObjectData::GetInstance()->StageLoad("stage2");
	//			}
	//			else if (stageCount_ == TWO)
	//			{
	//				LoadObjectData::GetInstance()->StageLoad("stage3");
	//			}
	//			else if (stageCount_ == THREE)
	//			{
	//				LoadObjectData::GetInstance()->StageLoad("stage4");
	//			}
	//			else if (stageCount_ == FOUR)
	//			{
	//				LoadObjectData::GetInstance()->StageLoad("stageLIBLADE");
	//			}

	//			//開始地点をセット
	//			player_->SetPos(LoadObjectData::GetInstance()->GetStartPos());

	//			//ゴール初期化
	//			goal_->SetPos(LoadObjectData::GetInstance()->GetEndPos());

	//			blackOutFlag_ = false;
	//		}
	//	}
	//}
	//else
	//{
	//	//ブラックアウト
	//	if (color_.x_ > 0.0f)
	//	{
	//		color_.x_ -= 0.01f;
	//		color_.y_ -= 0.01f;
	//		color_.z_ -= 0.01f;
	//		color_.w_ -= 0.01f;
	//	}
	//}
}

void PlayScene::Debug()
{
#ifdef _DEBUG

	//ImGui受付開始
	ImguiManager::GetInstance()->Begin();
	float test1 = 0.5f;

	Vector2D con = controller_->GetInstance()->GetRightStickVec();

	ImGui::Text("test");
	ImGui::SliderFloat("Test", &test1, 0.01f, 0.99f);
	ImGui::SliderFloat("RstickX", &con.x_, 0.01f, 0.99f);
	ImGui::SliderFloat("RstickY", &con.y_, 0.01f, 0.99f);

	//titleSceneheへ
	if (ImGui::Button("TITLE"))
	{
		ChengeScene::GetInstance()->SetPlayFlag("TITLE");
	}

	//playSceneheへ
	if (ImGui::Button("Play"))
	{
		ChengeScene::GetInstance()->SetPlayFlag("PLAY");
	}

	//clearSceneheへ
	if (ImGui::Button("GAMECLEAR"))
	{
		ChengeScene::GetInstance()->SetPlayFlag("GAMECLEAR");
	}

	//goalSceneheへ
	if (ImGui::Button("GAMEOVER"))
	{
		ChengeScene::GetInstance()->SetPlayFlag("GAMEOVER");
	}

	//ImGui受付終了
	ImguiManager::GetInstance()->End();

#endif _DEBUG
}
