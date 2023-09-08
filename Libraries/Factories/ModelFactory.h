/*
 *	@File	ModelFactory.h
 *	@Brief	モデルを作成するファクトリークラス。
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MODELFACTORY
#define MODELFACTORY

class ModelFactory
{
public:
	ModelFactory();
	~ModelFactory();

	// モデルを作成する関数
	static std::unique_ptr<DirectX::DX11::Model> GetCreateModel(ID3D11Device1* device, const wchar_t* filename);

	// モデルを開放する関数
	static void DeleteModel(std::unique_ptr<DirectX::Model>& model);
};

#endif // MODELFACTORY