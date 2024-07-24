#pragma once

class TexManager
{

public:




	//マス関係

	//バトルマス
	size_t battlePng;

	//
	size_t enhancePng;

	//
	size_t itemPng_;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	//シングルトン
	static TexManager* GetInstance()
	{
		static TexManager gameCamera;
		return &gameCamera;
	}

private:

	//
	TexManager() = default;
	~TexManager() = default;

	//
	TexManager& operator=(const TexManager&) = delete;
	TexManager(const TexManager&) = delete;
};