/*
 *	@File	ModelFactory.h
 *	@Brief	モデルを作成するファクトリークラス。
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MODELFACTORY
#define MODELFACTORY

#include "../Game/GameMain.h"

class ModelFactory
{
private:

public:
	// モデルの作成をするファクトリー
	static std::unique_ptr<DirectX::DX11::Model> GetModel(ID3D11Device1* device, const wchar_t* filename);

};

#endif // MODELFACTORY