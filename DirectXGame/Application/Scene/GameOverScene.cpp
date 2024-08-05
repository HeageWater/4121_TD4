#include "GameOverScene.h"
#include "ChengeScene.h"
#include "ImGui.h"

void GameOverScene::Initialize()
{
	//描画用行列
	matView_.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//
	whiteTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/white1x1.png");

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
	screen_.obj_.scale_ = { 0,0,0.2f };

	//透過するかどうか
	normalSpriteCommon_->Inilialize(MyDirectX::GetInstance(), true);

	//基礎
	sprite_->Inilialize(normalSpriteCommon_, &matProjection_);

	//画像色
	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };

	overTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/scene/gameover.png");

	//ライフ英語
	over_->Inilialize(normalSpriteCommon_, &matProjection_);
	over_->position_ = { -480,-680,0 };
	over_->scale_ = { Window::window_width_ * 2,Window::window_height_ * 2,1 };
	over_->SetColor(color_);

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(shader_, pipeline_.get());

	//音読み込み
	hitSound_ = MyXAudio::GetInstance()->SoundLoadWave("Resources/sound/Click.wav");
}

void GameOverScene::Update()
{
	//
	Debug();

	if (input_->GetTrigger(DIK_SPACE))
	{
		ChengeScene::GetInstance()->SetPlayFlag("TITLE");

		//BGMを鳴らす
		MyXAudio::GetInstance()->SoundPlayWave(hitSound_);
	}

	over_->Update();

	//カメラ更新
	matView_.MatUpdate();

	//天球更新
	skydome_->Update(matView_.mat_, matProjection_);

	//スクリーン更新
	screen_.MatUpdate(matView_.mat_, matProjection_, 0);

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();
}

void GameOverScene::Draw()
{
	//Draw
	MyDirectX::GetInstance()->PrevDrawScreen();

	//// 描画コマンド
	MyDirectX::GetInstance()->PostDrawScreen();

	//UIDraw
	MyDirectX::GetInstance()->PrevDraw();

	//スクリーン描画
	screen_.Draw(whiteTex_);

	//天球
	skydome_->Draw();

	//
	over_->Draw(overTex_);

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

#ifdef _DEBUG

	//ImGui描画
	ImguiManager::GetInstance()->Draw();

#endif _DEBUG

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void GameOverScene::Finalize()
{
}

void GameOverScene::Debug()
{
#ifdef _DEBUG

	//ImGui受付開始
	ImguiManager::GetInstance()->Begin();
	float test1 = 0.5f;

	ImGui::Text("test");
	ImGui::SliderFloat("Test", &test1, 0.01f, 0.99f);

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
