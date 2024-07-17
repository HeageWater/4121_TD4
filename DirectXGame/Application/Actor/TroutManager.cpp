#include "TroutManager.h"

void TroutManager::Initialize()
{
	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/battle/battle.png");

	//マスの数を0に
	count_ = 0;
}

void TroutManager::Update()
{
	//ステージ更新
	for (size_t i = 0; i < trouts_.size(); i++)
	{
		trouts_[i]->Update();
	}
}

void TroutManager::Draw()
{
	//ステージ描画
	for (size_t i = 0; i < trouts_.size(); i++)
	{
		trouts_[i]->Draw();
	}
}

void TroutManager::Finalize()
{
	//消去
	trouts_.clear();
}

void TroutManager::Reset()
{
	//消去
	trouts_.clear();
}

TroutManager* TroutManager::GetInstance()
{
	static TroutManager troutManager;
	return &troutManager;
}

void TroutManager::SetCamera(Matrix view, Matrix prodaction)
{
	view_ = view;
	prodaction_ = prodaction;
}

void TroutManager::SetModel(Shader shader, GPipeline* pipeline)
{
	shader_ = shader;
	pipeline_ = pipeline;
}

void TroutManager::CreateTrout(size_t kind, Vector3D pos)
{
	//モデルを指定して3Dオブジェクトを生成
	BaseTrout* newTrout_ = nullptr;

	//ランダム
	if (kind == Random)
	{
		//最小値
		size_t minRange = 0;

		//最大値
		size_t maxRange = Size - 1;

		//ランダム
		kind = MyMath::GetRandom(minRange, maxRange);
	}

	//引数がBattleなら
	if (kind == Battle)
	{
		//バトルマスに
		newTrout_ = new BattleTrout();
	}
	//引数がPowerUpなら
	else if (kind == PowerUp)
	{
		//バトルマスに
		newTrout_ = new PowerUpTrout();
	}
	//引数がItemなら
	else if (kind == Item)
	{
		//バトルマスに
		newTrout_ = new ItemTrout();
	}

	//初期化に必要な変数代入
	newTrout_->SetCamera(view_, prodaction_);
	newTrout_->SetModel(shader_, pipeline_);

	//改めて初期化
	newTrout_->Initialize();

	//位置と大きさ
	newTrout_->SetPos(pos);
	newTrout_->SetScale(Vector3D(5, 5, 5));
	newTrout_->SetVec(Vector3D(1, 1, 1));

	//格納
	trouts_.push_back(newTrout_);
}

void TroutManager::CreateMap(size_t size)
{
	//マップ生成
	for (size_t i = 0; i < size; i++)
	{
		//2個ずつ増やす
		size_t count = (i * 2) + 1;

		//半分超えたら減らす
		if (i + 1 > size / 2)
		{
			count = ((size - i - 1) * 2) + 1;
		}

		//マスの数
		for (size_t j = 0; j < count; j++)
		{
			//マス生成
			CreateTrout(Random, { (float)i * 10 , 0, (float)((count / 2) - j) * 10 });
		}
	}
}

std::vector<BaseTrout*> TroutManager::GetTrout()
{
	return trouts_;
}
