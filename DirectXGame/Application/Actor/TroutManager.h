#pragma once
#include "BaseTrout.h"
#include "BattleTrout.h"
#include "ItemTrout.h"
#include "PowerUpTrout.h"

enum Kind
{
	Battle,
	PowerUp,
	Item,
};

class TroutManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static TroutManager* GetInstance();

	/// <summary>
	/// カメラセット
	/// </summary>
	/// <param name="view"></param>
	/// <param name="prodaction"></param>
	void SetCamera(Matrix view, Matrix prodaction);

	/// <summary>
	/// シェーダーとパイプラインをセット
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline"></param>
	void SetModel(Shader shader, GPipeline* pipeline);

	/// <summary>
	/// マス制作
	/// </summary>
	void CreateTrout(size_t kind);

	/// <summary>
	/// エネミーの情報を渡す
	/// </summary>
	/// <returns></returns>
	//std::vector<Enemy*> GetEnemy();
	std::vector<BaseTrout*> GetTrout();

private:

	TroutManager() = default;
	~TroutManager() = default;

	//コピーコンストラクタ・代入演算子削除
	TroutManager& operator=(const TroutManager&) = delete;
	TroutManager(const TroutManager&) = delete;

private:

	//マス格納用
	std::vector<BaseTrout*> trouts_;

	Matrix view_;
	Matrix prodaction_;

	Shader shader_;
	GPipeline* pipeline_;

	//画像
	size_t tex_;
};