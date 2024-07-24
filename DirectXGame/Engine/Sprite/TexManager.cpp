#include "TexManager.h"
#include "DirectX.h"

void TexManager::Initialize()
{
	//画像読み込み
	battlePng = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/battle/battle.png");

	//画像読み込み
	enhancePng = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/enhance/enhance.png");

	//画像読み込み
	itemPng_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/heal/heal.png");





}
