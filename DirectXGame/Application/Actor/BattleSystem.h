#pragma once
#include "GameModel.h"
#include "Status.h"
#include "Sound.h"
#include "Input.h"
#include "Enemy.h"

class BattleSystem
{
public:

	void Update();

	void Initalize(Shader shader, GPipeline* pipeline_);

	void Draw();

	void Reaet();

	void SetPlayerStatus(Status status) { player = status; };

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static BattleSystem* GetInstance();

	/// <summary>
	/// matview,matProdactionをセット
	/// </summary>
	void SetCamera(Matrix matView, Matrix matProjection);

	Status GetPlayerStatus() { return player; };

	bool Nextbattle() { return enemy.hp_ <= 0; };
	bool GameOver() { return player.hp_ <= 0; };

private:

	BattleSystem() = default;
	~BattleSystem() = default;

	//コピーコンストラクタ・代入演算子削除
	BattleSystem& operator=(const BattleSystem&) = delete;
	BattleSystem(const BattleSystem&) = delete;

private:

	//キーボード
	Input* input_ = Input::GetInstance();

	//
	Status player;
	Status enemy;

	//パイプライン
	GPipeline* pipeline_;

	//
	size_t tex_ = 0;

	//
	Matrix matView_;
	Matrix matProjection_;

	size_t nowKind = 0;


	std::unique_ptr<Enemy> enemy_;

};