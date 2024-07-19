#include "GameCamera.h"
#include "Shake.h"

void GameCamera::Initialize(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	matView_.eye_ = _eye;
	matView_.target_ = _target;
	matView_.up_ = _up;
}

void GameCamera::Initlalise()
{
	//描画用行列
	matView_.Init(Vector3D(0.0f, 60.0f, -100.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
}

void GameCamera::Update()
{
	//カメラ更新
	matView_.MatUpdate();
	//matView_ = MyMath::LookAtLH(eye_, target_, up_);
}

void GameCamera::Move(float camerapos)
{
	matView_.eye_.x_ = camerapos;
	matView_.target_.x_ = camerapos;

	Update();
}

void GameCamera::Reset()
{
	//描画用行列
	matView_.Init(Vector3D(0.0f, 60.0f, -100.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
}

void GameCamera::Sheik(size_t time)
{
	//意味なし
	time++;
}
