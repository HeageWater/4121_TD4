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
	//player_->Update(matView_.mat_, matProjection_);

	if (input_->GetTrigger(DIK_SPACE))
	{
		//
		ChengeScene::GetInstance()->SetPlayFlag("PLAY");

		//BGMを鳴らす
		MyXAudio::GetInstance()->SoundPlayWave(hitSound_);
	}

	///ここまで

	title_->Update();
	exit_->Update();
	start_->Update();

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
	matView_.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

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
	exitTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/scene/exit.png");
	startTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/scene/start.png");
	titleTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/scene/title.png");

	//player初期化
	//player_ = std::make_unique<Player>();
	//player_->Initialize(shader_, pipeline_.get());

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(shader_, pipeline_.get());

	//ライフ英語
	title_->Inilialize(spriteCommon_, &matProjection_);
	title_->position_ = { -220,-200,0 };
	title_->scale_ = { Window::window_width_,Window::window_height_,1 };
	title_->SetColor(color_);

	start_->Inilialize(spriteCommon_, &matProjection_);
	start_->position_ = { -220,-280,0 };
	start_->scale_ = { Window::window_width_,Window::window_height_,1 };
	start_->SetColor(color_);

	exit_->Inilialize(spriteCommon_, &matProjection_);
	exit_->position_ = { 0,0,0 };
	exit_->scale_ = { Window::window_width_,Window::window_height_,1 };
	exit_->SetColor(color_);

	//音読み込み
	hitSound_ = MyXAudio::GetInstance()->SoundLoadWave("Resources/sound/Click.wav");
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
	//player_->Draw();

	//Ui表示
	UIDraw();

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

}

void TitleScene::UIDraw()
{
	title_->Draw(titleTex_);
	//exit_->Draw(exitTex_);
	start_->Draw(startTex_);
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

	//カーソルの位置
	Vector2D mouse;
	mouse.x_ = (float)Input::GetInstance()->CursorPos().x;
	mouse.y_ = (float)Input::GetInstance()->CursorPos().y;
	ImGui::SliderFloat("mouseX", &mouse.x_, 0.01f, 0.99f);
	ImGui::SliderFloat("mouseY", &mouse.y_, 0.01f, 0.99f);

	//左クリック
	bool Ltrigger = Input::GetInstance()->ClickTrriger(Input::LeftClick);
	bool Rtrigger = Input::GetInstance()->ClickTrriger(Input::RightClick);
	ImGui::Checkbox("leftClick", &Ltrigger);
	ImGui::Checkbox("rightClick", &Rtrigger);

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
