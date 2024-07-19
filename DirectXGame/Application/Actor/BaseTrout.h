#pragma once
#include "ImGuiManager.h"
#include "GameModel.h"
#include "Input.h"
#include "Controller.h"
#include "Sound.h"

class BaseTrout : public GameModel
{
public:

	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

	/// <summary>
	/// カメラセット
	/// </summary>
	/// <param name="view"></param>
	/// <param name="prodaction"></param>
	virtual void SetCamera(Matrix view, Matrix prodaction);

	/// <summary>
	/// シェーダーとパイプラインをセット
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline"></param>
	virtual void SetModel(Shader shader, GPipeline* pipeline);

	/// <summary>
	///posをセットする
	/// </summary>
	/// <param name="pos"></param>
	virtual void SetPos(Vector3D pos) { model_->mat_.trans_ = pos; };

	/// <summary>
	/// scaleをセットする
	/// </summary>
	/// <param name="pos"></param>
	virtual void SetScale(Vector3D scale) { model_->mat_.scale_ = scale; };

	/// <summary>
	/// posを返す
	/// </summary>
	/// <returns></returns>
	virtual Vector3D GetPos() { return model_->mat_.trans_; };

	/// <summary>
	/// rotaionを返す
	/// </summary>
	/// <returns></returns>
	virtual Vector3D GetRot() { return model_->mat_.rotAngle_; };

	/// <summary>
	/// scaleを返す
	/// </summary>
	/// <returns></returns>
	virtual Vector3D GetScale() { return model_->mat_.scale_; };

public:

	//
	Matrix view_;
	Matrix prodaction_;

	//
	Shader shader_;
	GPipeline* pipeline_;

	//画像
	size_t tex_;
};
