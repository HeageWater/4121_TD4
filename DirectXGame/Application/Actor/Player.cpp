#include "Player.h"
#include "Easing.h"
#include "CollisionManager.h"
#include "ModelManager.h"

Player::Player()
{
	//modelを制作
	model_ = std::make_unique<Model>();

	//コリジョン用タグ
	tag_ = "player";

	//コリジョンに追加
	CollisionManager::GetInstance()->AddCollision(this);

	//初期化
	model_->mat_.Initialize();
	model_->mat_.scale_ = { 1,1,1 };

	//サウンド
	sound_ = nullptr;

	//タイム
	time_ = 0;

	//パイプライン初期化
	pipeline_ = nullptr;

	//画像初期化
	tex_ = 0;
}

Player::~Player()
{
	//消去
	Destroy();
}

void Player::Initialize(Shader shader, GPipeline* pipeline)
{
	//パイプライン代入
	pipeline_ = pipeline;

	//モデル読み込み
	model_->Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\Player\\Player.obj", pipeline_);

	//初期化
	model_->mat_.Initialize();
	model_->mat_.scale_ = { 10,10,10 };
	model_->mat_.rotAngle_ = { 0,0,0 };
	model_->mat_.trans_.x_ = 0;// 950;
	model_->mat_.trans_.y_ = 0;
	model_->mat_.trans_.z_ = 0;

	//コントローラー
	controller_ = Controller::GetInstance();
	
	//タイム
	time_ = 0;

	//音鳴らす用
	sound_ = MyXAudio::GetInstance();

	//音読み込み
	///jumpSE_ = sound_->SoundLoadWave("Resources/sound/SE_jump.wav");

	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/skydome/skyBG.png");
	//tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Player/Player.png");
}

void Player::Draw(size_t tex)
{
	//描画
	model_->Draw(tex);
}

void Player::Draw()
{
	//描画
	model_->Draw(tex_);
}

Vector2D Player::MoveCamera(Matrix matView, Matrix matProjection, Input* input)
{
	//コントローラー更新
	controller_->Update();

	//動く値
	Vector3D move = { 0 ,0 ,0 };

	//WASDで移動
	move.x_ += input->GetKey(DIK_D) - input->GetKey(DIK_A);
	move.z_ += input->GetKey(DIK_W) - input->GetKey(DIK_S);

	//transに加算
	model_->mat_.trans_ += move;

	//更新
	model_->MatUpdate(matView, matProjection);

	//
	return Vector2D(move.x_, move.z_);
}

void Player::Update(Matrix matView, Matrix matProjection)
{
	//カメラ代入
	matView_ = matView;
	matProjection_ = matProjection;

	//コントローラーUpdate
	controller_->Update();

	//座標Update
	model_->MatUpdate(matView, matProjection);
}

void Player::Reset()
{
	//リセット
	model_->mat_.Initialize();
	model_->mat_.scale_ = { 3,3,3 };
	model_->mat_.trans_.x_ = 0;// 950;
	model_->mat_.trans_.y_ = 11;
}

void Player::SetDeadAnimation()
{
	//死亡時のアニメーション用
	model_->mat_.scale_ *= 2;
	model_->mat_.rotAngle_.y_ = 3.14f;
}

void Player::OnCollision()
{

}

void Player::SetCamera(Matrix matView, Matrix matProjection)
{
	//座標Update
	model_->MatUpdate(matView, matProjection);
}