#pragma once
#include "ImGuiManager.h"
#include "GameModel.h"
#include "Input.h"
#include "Controller.h"
#include "Sound.h"

class BaseTrout
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

private:

	//モデル
	std::unique_ptr<Model> model_;

	Matrix view_;
	Matrix prodaction_;

	Shader shader_;
	GPipeline* pipeline_;

	//画像
	size_t tex_;

};
