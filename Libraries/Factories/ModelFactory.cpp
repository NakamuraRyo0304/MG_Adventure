/*
 *	@File	ModelFactory.cpp
 *	@Brief	モデルを作成するファクトリークラス。
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ModelFactory.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
ModelFactory::ModelFactory()
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
ModelFactory::~ModelFactory()
{
}

/// <summary>
/// モデルを作成し、返却する(std::unique_ptr<Model>で格納可)
/// </summary>
/// <param name="device">ID3D11Device1ポインタ</param>
/// <param name="filename">ファイルパス　拡張子は「.cmo」</param>
// 第１引数：デバイス 第２引数：ファイルの名前
std::unique_ptr<DX11::Model> ModelFactory::GetModel(ID3D11Device1* device, const wchar_t* filename)
{
	// モデルの作成(作成時のみの使い捨て)
	std::unique_ptr<EffectFactory> fx =
		std::make_unique<EffectFactory>(device);

	// ディレクトリの設定 (ここにモデルがあるパスを設定)
	fx->SetDirectory(L"Resources/Models");

	// モデルデータの読み込み
	std::unique_ptr<DX11::Model> model = Model::CreateFromCMO(
		device,
		filename,
		*fx
	);

	// 作成したモデルを返却
	return model;
}

/// <summary>
/// モデルの解放
/// </summary>
/// <param name="引数無し"></param>
void ModelFactory::DeleteModel()
{
	// 空の引数を渡す(例外スロー防止用オーバーロード)
	std::unique_ptr<Model> nullModel = nullptr;
	DeleteModel(nullModel);
}

/// <summary>
/// モデルの解放
/// </summary>
/// <param name="model">解放したいモデルデータ</param>
void ModelFactory::DeleteModel(std::unique_ptr<Model>& model)
{
	model.reset();
}
