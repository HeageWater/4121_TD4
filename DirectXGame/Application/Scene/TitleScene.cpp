#include "TitleScene.h"
#include "imgui.h"
#include "ChengeScene.h"
#include "Enum.h"

void TitleScene::Update()
{
	//デバッグ表示用
	Debug();

	///ここから更新処理追加

	//player更新
	player_->Update(matView_.mat_, matProjection_);

	TroutManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	TroutManager::GetInstance()->SetModel(shader_, pipeline_.get());
	TroutManager::GetInstance()->Update();

	//space押して生成
	if (input_->GetTrigger(DIK_SPACE))
	{
		//リセット
		TroutManager::GetInstance()->Reset();

		//マップの階層
		size_t a = 5;

		//バトルマス生成
		TroutManager::GetInstance()->CreateMap(a);
	}

	///ここまで

	matView_.eye_.x_ += input_->GetKey(DIK_D) - input_->GetKey(DIK_A);
	matView_.eye_.z_ += input_->GetKey(DIK_W) - input_->GetKey(DIK_S);

	//カメラ更新
	matView_.MatUpdate();

	//スクリーン更新
	screen_.MatUpdate(matView_.mat_, matProjection_, ZERO);

	//天球更新
	skydome_->Update(matView_.mat_, matProjection_);

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();
}

void TitleScene::Initialize()
{
	//描画用行列
	matView_.Init(Vector3D(0.0f, 60.0f, -100.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

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
	screen_.Initialize(pipeline_.get(), bilShader_);
	screen_.obj_.trans_.z_ = 100.1f;
	screen_.obj_.scale_ = { Window::window_width_ * 2,Window::window_height_ / 2,0.2f };
	screen_.obj_.scale_ = { 0,0,0 };

	//画像色
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	//透過するかどうか
	spriteCommon_->Inilialize(MyDirectX::GetInstance(), true);

	//tex
	whiteTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/white1x1.png");

	//player初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(shader_, pipeline_.get());

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(shader_, pipeline_.get());

	//test 
	TroutManager::GetInstance()->Initialize();

}

void TitleScene::Draw()
{
	//Draw
	MyDirectX::GetInstance()->PrevDrawScreen();

	// 描画コマンド
	MyDirectX::GetInstance()->PostDrawScreen();

	//UIDraw
	MyDirectX::GetInstance()->PrevDraw();

	//スクリーン描画
	screen_.Draw(whiteTex_);

	///ここから描画処理追加

	//天球
	skydome_->Draw();

	//player描画
	player_->Draw();

	//マス表示
	TroutManager::GetInstance()->Draw();

	///ここまで

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

#ifdef _DEBUG

	//ImGui描画
	ImguiManager::GetInstance()->Draw();

#endif _DEBUG

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void TitleScene::Finalize()
{
	TroutManager::GetInstance()->Finalize();
}

void TitleScene::Debug()
{
#ifdef _DEBUG

	//ImGui受付開始
	ImguiManager::GetInstance()->Begin();
	float test1 = 0.5f;

	ImGui::Text("test");
	ImGui::SliderFloat("Test", &test1, 0.01f, 0.99f);
	ImGui::SliderFloat("eyeX", &matView_.eye_.x_, 0.01f, 50.99f);
	ImGui::SliderFloat("eyeZ", &matView_.eye_.z_, 0.01f, 50.99f);

	//マスの数
	float sizeT = (float)TroutManager::GetInstance()->GetTrout().size();
	ImGui::SliderFloat("trout size", &sizeT, 0.01f, 50.99f);

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
