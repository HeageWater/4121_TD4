#pragma once
#include "BaseTrout.h"
#include "BattleTrout.h"
#include "ItemTrout.h"
#include "PowerUpTrout.h"

enum Kind
{
	Battle,
	PowerUp,
	Item,//ここから下に追加
	Size,//これを下から2番目に置くことでマスの数がわかる
	Random = 99,//ランダム生成用
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
	/// リセット
	/// </summary>
	void Reset();

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
	/// マス生成
	/// </summary>
	/// <param name="kind">何も入れないとランダムで生成</param>
	void CreateTrout(size_t kind = 99, Vector3D pos = { 0,0,0 }, size_t time = 0);

	/// <summary>
	/// サイズ
	/// </summary>
	/// <param name="size"></param>
	void CreateMap(size_t size);

	/// <summary>
	/// エネミーの情報を渡す
	/// </summary>
	/// <returns></returns>
	std::vector<BaseTrout*> GetTrout();
	
	/// <summary>
	/// 階数を返す
	/// </summary>
	/// <returns></returns>
	size_t GetCount() { return count_; };

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

	//マスの数
	size_t count_;
};