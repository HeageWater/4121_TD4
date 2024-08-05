#include "PlayScene.h"
#include "imgui.h"
#include "ChengeScene.h"
#include "Enum.h"
#include "Input.h"
#include "BattleSystem.h"

void PlayScene::Update()
{
	//デバッグ表示用
	Debug();

	///ここから更新処理追加


	//
	switch (mapSelect)
	{
	case 0:

		//マス選んでいるとき
		MoveMap();

		break;

	case 1:

		//マスの中身の処理
		SelectMap();

		break;

		break;

	default:
		break;
	}

	///ここまで

	//更新処理
	ModelUpdate();
}

void PlayScene::Initialize()
{
	//描画用行列
	matView_.Init(Vector3D(0.0f, 90.0f, -60.0f), Vector3D(0.0f, 10.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

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
	//screen_.obj_.scale_ = { Window::window_width_ * 2,Window::window_height_ / 2,0.2f };
	screen_.obj_.scale_ = { 0,0,0 };

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

	//player初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(shader_, pipeline_.get());

	//
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(shader_, pipeline_.get());

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(shader_, pipeline_.get());

	//test 
	TroutManager::GetInstance()->Initialize();
	TroutManager::GetInstance()->SetModel(shader_, pipeline_.get());

	//
	mapSelect = 99;

	//
	nowCount_ = 0;

	blackTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/black.png");

	//scale用
	float size = 15.5f;
	//ブラックアウト
	blackOut_->Inilialize(normalSpriteCommon_, &matProjection_);
	blackOut_->position_ = { -680,-Window::window_height_,0 };
	blackOut_->scale_ = { Window::window_width_ * size,Window::window_height_ * size,1 };
	blackOut_->SetColor(color_);

	//更新
	ModelUpdate();

	//リセット
	TroutManager::GetInstance()->Reset();

	//マップの階層
	size_t a = 4;

	//バトルマス生成
	TroutManager::GetInstance()->CreateMap(a);

	mapSelect = 0;

	nowStage = 0;
	nowStageF = true;

	nowKind = TroutManager::GetInstance()->GetTrout()[nowCount_]->kind_;

	//
	BattleSystem::GetInstance()->Initalize(shader_, pipeline_.get());
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

	//天球
	skydome_->Draw();

	//
	switch (mapSelect)
	{
	case 0:

		//player描画
		player_->Draw();

		//マス表示
		TroutManager::GetInstance()->Draw();

		break;

	case 1:

		//
		switch (nowKind)
		{
		case 0:

			//バトル

			//
			//enemy_->Draw();

			//
			BattleSystem::GetInstance()->Draw();

			break;

		case 1:

			break;

		case 2:

			break;

		default:
			break;
		}

		break;

	default:
		break;
	}

	//ブラックアウト
	blackOut_->SetColor(color_);
	blackOut_->Draw(blackTex_);

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
	TroutManager::GetInstance()->Finalize();
}

void PlayScene::ModelUpdate()
{
	//カメラ更新
	matView_.MatUpdate();

	//player更新
	player_->Update(matView_.mat_, matProjection_);

	//
	enemy_->SetCamera(matView_.mat_, matProjection_);
	enemy_->Update();

	//マス更新
	TroutManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	TroutManager::GetInstance()->Update();

	//スクリーン更新
	screen_.MatUpdate(matView_.mat_, matProjection_, ZERO);

	//パーティクル更新
	ParticleManager::GetInstance()->SetCamera(matView_.mat_, matProjection_);
	ParticleManager::GetInstance()->Update();

	//天球更新
	skydome_->Update(matView_.mat_, matProjection_);

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();

	//burakkuauto 
	blackOut_->Update();

	//暗転
	BlackOut();
}

void PlayScene::BlackOut()
{
	//ブラックアウト
	if (blackOutFlag_)
	{
		//ブラックアウト
		if (color_.x_ < 1.0f)
		{
			float zouka = 0.005f;

			color_.x_ += zouka;
			color_.y_ += zouka;
			color_.z_ += zouka;
			color_.w_ += zouka;

			if (color_.x_ >= 1.0f)
			{
				//
				blackOutFlag_ = false;

				//ここにフラグ関係
				if (mapSelect == 0)
				{
					mapSelect = 1;

					BattleSystem::GetInstance()->Reaet();
					BattleSystem::GetInstance()->SetPlayerStatus(player_->status_);
				}
				else
				{
					mapSelect = 0;
					matView_.target_.x_ = player_->GetPos().x_;
					matView_.eye_.x_ = player_->GetPos().x_;
				}
			}
		}
	}
	else
	{
		//ブラックアウト
		if (color_.x_ > 0.0f)
		{
			float zouka = 0.005f;

			color_.x_ -= zouka;
			color_.y_ -= zouka;
			color_.z_ -= zouka;
			color_.w_ -= zouka;
		}
	}
}

void PlayScene::MoveMap()
{
	//マス数
	//size_t count = TroutManager::GetInstance()->GetTrout().size();

	//階数
	size_t dan = TroutManager::GetInstance()->GetCount();

	//いるべきマスの場所を格納
	Vector3D troutPos = TroutManager::GetInstance()->GetTrout()[nowCount_]->GetPos();

	//マスの位置にプレイヤーを持ってくる
	player_->SetPos(troutPos);

	//次の選択
	size_t next = nowdan_ * 2;

	if (nowdan_ != dan - 1 && !nowStageF)
	{
		//進行方向決め
		if (input_->GetTrigger(DIK_RIGHT))
		{
			nowCount_ += next + 1;
			nowdan_++;
			nowStage = 0;
			nowStageF = true;
			nowKind = TroutManager::GetInstance()->GetTrout()[nowCount_]->kind_;
		}
		if (input_->GetTrigger(DIK_UP))
		{
			nowCount_ += next + 2;
			nowdan_++;
			nowStage = 0;
			nowStageF = true;
			nowKind = TroutManager::GetInstance()->GetTrout()[nowCount_]->kind_;
		}
		if (input_->GetTrigger(DIK_LEFT))
		{
			nowCount_ += next + 3;
			nowdan_++;
			nowStage = 0;
			nowStageF = true;
			nowKind = TroutManager::GetInstance()->GetTrout()[nowCount_]->kind_;
		}
	}

	if (nowStageF)
	{
		nowStage++;

		//if (nowStage >= 30)
		if (input_->GetTrigger(DIK_SPACE))
		{
			nowStageF = false;

			//ブラックアウト
			blackOutFlag_ = true;
		}
	}
}

void PlayScene::SelectMap()
{
	size_t plus = 0;

	Vector3D pos = { 10,0,-10 };

	//階数
	size_t dan = TroutManager::GetInstance()->GetCount();

	//
	switch (nowKind)
	{
	case 0:

		//バトル

		BattleSystem::GetInstance()->SetCamera(matView_.mat_, matProjection_);
		BattleSystem::GetInstance()->Update();

		if (BattleSystem::GetInstance()->Nextbattle())
		{

			player_->status_ = BattleSystem::GetInstance()->GetPlayerStatus();

			if (!blackOutFlag_)
			{
				if (nowdan_ != dan - 1)
				{
					//ブラックアウト
					blackOutFlag_ = true;
				}
				else
				{
					//
					ChengeScene::GetInstance()->SetPlayFlag("GAMECLEAR");
				}
			}
		}

		if (BattleSystem::GetInstance()->GameOver())
		{
			//
			ChengeScene::GetInstance()->SetPlayFlag("GAMEOVER");
		}

		break;

	case 1:

		//space押して生成
		if (input_->GetTrigger(DIK_SPACE) && !blackOutFlag_)
		{
			//パワーアップ
			plus = player_->GetStatus().power_ + MyMath::GetRandom(3, 7);

			player_->status_.power_ = plus;

			if (nowdan_ != dan - 1)
			{
				//ブラックアウト
				blackOutFlag_ = true;
			}
			else
			{
				//
				ChengeScene::GetInstance()->SetPlayFlag("GAMECLEAR");
			}
		}

		break;

	case 2:

		//space押して生成
		if (input_->GetTrigger(DIK_SPACE) && !blackOutFlag_)
		{
			//回復
			plus = player_->GetStatus().hp_ + MyMath::GetRandom(5, 10);

			player_->status_.hp_ = plus;

			if (nowdan_ != dan - 1)
			{
				//ブラックアウト
				blackOutFlag_ = true;
			}
			else
			{
				//
 				ChengeScene::GetInstance()->SetPlayFlag("GAMECLEAR");
			}
		}

		break;

	default:
		break;
	}
}

void PlayScene::ReturnMap()
{
	//space押して生成
	if (input_->GetTrigger(DIK_SPACE))
	{
		//ブラックアウト
		blackOutFlag_ = true;

		matView_.target_ = player_->GetPos();

		mapSelect = 0;
	}
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

	//マスの数
	float sizeT = (float)TroutManager::GetInstance()->GetTrout().size();
	ImGui::SliderFloat("trout size", &sizeT, 0.01f, 50.99f);

	//カーソルの位置
	Vector2D mouse;
	mouse.x_ = (float)Input::GetInstance()->CursorPos().x;
	mouse.y_ = (float)Input::GetInstance()->CursorPos().y;
	ImGui::SliderFloat("mouseX", &mouse.x_, 0.01f, 0.99f);
	ImGui::SliderFloat("mouseY", &mouse.y_, 0.01f, 0.99f);

	//カーソルの位置
	float hp = (float)player_->GetStatus().hp_;
	float power = (float)player_->GetStatus().power_;
	ImGui::SliderFloat("hp", &hp, 0.01f, 0.99f);
	ImGui::SliderFloat("power", &power, 0.01f, 0.99f);

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
