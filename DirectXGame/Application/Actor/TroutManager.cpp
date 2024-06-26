#include "TroutManager.h"

void TroutManager::Initialize()
{
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/ene/enemy.png");
}

void TroutManager::Update()
{
	//ステージ更新
	for (size_t i = 0; i < trouts_.size(); i++)
	{
		//trouts_[i]->Update(view_, prodaction_);
		trouts_[i]->Update();
	}
}

void TroutManager::Draw()
{
	//ステージ描画
	for (size_t i = 0; i < trouts_.size(); i++)
	{
		trouts_[i]->Draw();
		//trouts_[i]->Draw(tex_);
	}
}

void TroutManager::Finalize()
{
	//保存したposを削除
	for (size_t i = 0; i < trouts_.size(); i++)
	{
		trouts_.erase(trouts_.begin());
	}
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

void TroutManager::CreateTrout(size_t kind)
{
	//モデルを指定して3Dオブジェクトを生成
	BaseTrout* newTrout_ = nullptr;

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
	
	newTrout_->SetPos(Vector3D(0, 0, 10));
	newTrout_->SetScale(Vector3D(5, 5, 5));

	//格納
	trouts_.push_back(newTrout_);
}

std::vector<BaseTrout*> TroutManager::GetTrout()
{
	return trouts_;
}
