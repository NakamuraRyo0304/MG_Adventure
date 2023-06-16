/*
 *	@File	ModelFactory.cpp
 *	@Brief	モデルを作成するファクトリークラス。
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ModelFactory.h"

 //--------------------------------------------------------//
 //モデルを作成するファクトリー                            //
 //--------------------------------------------------------//
 // 第１引数：デバイス 第２引数：ファイルの名前
std::unique_ptr<DirectX::DX11::Model> ModelFactory::GetModel(ID3D11Device1* device, const wchar_t* filename)
{
	// モデルの作成(作成時のみの使い捨て)
	std::unique_ptr<DirectX::EffectFactory> fx =
		std::make_unique<DirectX::EffectFactory>(device);

	// ディレクトリの設定
	fx->SetDirectory(L"Resources/Models");

	// モデルデータの読み込み
	std::unique_ptr<DirectX::DX11::Model> model = DirectX::Model::CreateFromCMO(
		device,
		filename,
		*fx
	);

	// fx インスタンスは使用後に解放する
	fx.reset();

	// 作成したモデルを返却
	return model;
}