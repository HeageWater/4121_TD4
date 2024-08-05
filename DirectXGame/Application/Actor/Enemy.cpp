#include "Enemy.h"

void Enemy::SetCamera(Matrix matView, Matrix matProjection)
{
	//カメラ代入
	matView_ = matView;
	matProjection_ = matProjection;
}

Enemy::Enemy()
{
	//modelを制作
	model_ = std::make_unique<Model>();

	//初期化
	model_->mat_.Initialize();
	model_->mat_.scale_ = { 4.0f,4.0f,4.0f };

	//サウンド
	//sound_ = nullptr;

	//タイム
	time_ = 0;

	//パイプライン初期化
	pipeline_ = nullptr;

	//画像初期化
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/enemy/enemy.png");

	//
	status_.Initialize();
	status_.SetHP(5);
	status_.SetPower(1);
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(Shader shader, GPipeline* pipeline)
{
	//パイプライン代入
	pipeline_ = pipeline;

	//モデル読み込み
	model_->Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\enemy\\enemy.obj", pipeline_);

	//初期化
	model_->mat_.Initialize();
	model_->mat_.scale_ = { 5,5,5 };
	model_->mat_.rotAngle_ = { 0,0,0 };
	model_->mat_.trans_.x_ = 0;// 950;
	model_->mat_.trans_.y_ = 0;
	model_->mat_.trans_.z_ = 0;

	//タイム
	time_ = 0;

	//音鳴らす用
	//sound_ = MyXAudio::GetInstance();

	//音読み込み
	///jumpSE_ = sound_->SoundLoadWave("Resources/sound/SE_jump.wav");

	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/enemy/enemy.png");
}

void Enemy::Draw(size_t tex)
{
	//描画
	model_->Draw(tex);
}

void Enemy::Draw()
{
	//描画
	model_->Draw(tex_);
}

void Enemy::Update()
{
	if (atackF)
	{
		time_++;

		if (time_ < 10)
		{
			model_->mat_.trans_.x_ -= 0.1f * time_;
		}
		else if (time_ >= 10 && time_ < 20)
		{
			model_->mat_.trans_.x_ += 0.1f * (time_ / 2);
		}
		else
		{
			atackF = false;
		}
	}

	//座標Update
	model_->MatUpdate(matView_, matProjection_);
}

void Enemy::Reset()
{
	//リセット
	model_->mat_.Initialize();
	model_->mat_.scale_ = { 3,3,3 };
	model_->mat_.trans_.x_ = 0;// 950;
	model_->mat_.trans_.y_ = 11;
}

void Enemy::Attack()
{
	atackF = true;
	time_ = 0;
}
