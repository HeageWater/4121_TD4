#include "BattleSystem.h"
#include "MyMath.h"

void BattleSystem::Update()
{

	size_t damage = 0;
	size_t min = 0;
	size_t max = 5;
	size_t enemyMax = 2;

	//
	switch (nowKind)
	{
	case 0:

		//バトル
		if (input_->GetTrigger(DIK_SPACE))
		{
			nowKind = 1;

			damage = player.power_ + MyMath::GetRandom(min, max);

			if (enemy.hp_ >= damage)
			{
				enemy.hp_ -= damage;
			}
			else
			{
				enemy.hp_ = 0;

				nowKind = 2;

				player.hp_ += MyMath::GetRandom(min, max);
				player.power_ += MyMath::GetRandom(min, max);
			}
		}

		break;

	case 1:

		//
		enemy_->Attack();

		
		nowKind = 0;

		damage = enemy.power_ + MyMath::GetRandom(min, enemyMax);

		if (player.hp_ >= damage)
		{
			player.hp_ -= damage;
		}
		else
		{
			player.hp_ = 0;

			nowKind = 2;
		}

		break;

	case 2:

		break;

	default:
		break;
	}

	//
	enemy_->SetCamera(matView_, matProjection_);
	enemy_->Update();

	//座標Update
	//model_->MatUpdate(matView_, matProjection_);
}

void BattleSystem::Initalize(Shader shader, GPipeline* pipeline)
{
	//パイプライン代入
	pipeline_ = pipeline;

	shader;

	//モデル読み込み
	//model_->Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\enemy\\enemy.obj", pipeline_);

	//タイム
	//time_ = 0;

	//パイプライン初期化
	pipeline_ = nullptr;

	//画像初期化
	//tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/enemy/enemy.png");

	//
	enemy.Initialize();
	enemy.SetHP(5);
	enemy.SetPower(1);

	nowKind = 0;

	//
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(shader, pipeline);
	enemy_->SetPos(Vector3D{ 30,10,-10 });
	enemy_->SetScale(Vector3D{ 30,30,30 });
	enemy_->SetRot(Vector3D{ 0.8f, 1.0f, 0.8f });
}

void BattleSystem::Draw()
{
	//描画s
	//model_->Draw(tex_);

	//
	enemy_->Draw();
}

void BattleSystem::Reaet()
{
	//
	enemy.Initialize();
	enemy.SetHP(10);
	enemy.SetPower(1);

	nowKind = 0;
}

BattleSystem* BattleSystem::GetInstance()
{
	static BattleSystem troutManager;
	return &troutManager;
}

void BattleSystem::SetCamera(Matrix matView, Matrix matProjection)
{
	//カメラ代入
	matView_ = matView;
	matProjection_ = matProjection;
}
