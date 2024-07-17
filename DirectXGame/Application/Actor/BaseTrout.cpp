#include "BaseTrout.h"

void BaseTrout::Initialize()
{
	//modelを制作
	model_ = std::make_unique<Model>();
	model_->Initialize(MyDirectX::GetInstance(), shader_, "Resources\\Model\\battle\\battle.obj", pipeline_);

	//mat初期化
	model_->mat_.Initialize();
	model_->mat_.trans_ = { 0,0,0 };
	model_->mat_.scale_ = { 5,5,5 };

	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/battle/battle.png");
}

void BaseTrout::Update()
{
	//カメラセット
	model_->SetCamera(view_, prodaction_);

	//更新
	model_->Update();
}

void BaseTrout::Draw()
{
	//描画
	model_->Draw(tex_);
}

void BaseTrout::SetCamera(Matrix view, Matrix prodaction)
{
	//カメラ関係
	view_ = view;
	prodaction_ = prodaction;
}

void BaseTrout::SetModel(Shader shader, GPipeline* pipeline)
{
	//モデル関係
	shader_ = shader;
	pipeline_ = pipeline;
}
