#include "BattleTrout.h"
#include "TexManager.h"

BattleTrout::BattleTrout()
{
}

BattleTrout::~BattleTrout()
{
}

void BattleTrout::Initialize()
{
	//modelを制作
	model_ = std::make_unique<Model>();
	model_->Initialize(MyDirectX::GetInstance(), shader_, "Resources\\Model\\battle\\battle.obj", pipeline_);

	//mat初期化
	model_->mat_.Initialize();
	model_->mat_.trans_ = { 0,0,0 };
	model_->mat_.scale_ = { 5,5,5 };
}

void BattleTrout::Update()
{
	//カメラセット
	model_->SetCamera(view_, prodaction_);

	//更新
	model_->Update();
}

void BattleTrout::Draw()
{
	size_t tex_ = TexManager::GetInstance()->battlePng;

	//描画
	model_->Draw(tex_);
}
