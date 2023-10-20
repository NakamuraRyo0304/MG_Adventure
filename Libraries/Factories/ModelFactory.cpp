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
 /// <returns>なし</returns>
ModelFactory::ModelFactory()
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
ModelFactory::~ModelFactory()
{
}

/// <summary>
/// モデルを作成し、返却する(std::unique_ptr<Model>で格納可)
/// </summary>
/// <param name="device">ID3D11Device1ポインタ</param>
/// <param name="filename">ファイルパス　拡張子は「.cmo」</param>
/// <returns>作成したモデルデータ</returns>
std::unique_ptr<DX11::Model> ModelFactory::GetCreateModel(ID3D11Device1* device, const wchar_t* filename)
{
	// モデルの作成(作成時のみの使い捨て)
	std::unique_ptr<EffectFactory> _fx =
		std::make_unique<EffectFactory>(device);

	// ディレクトリの設定 (ここにモデルがあるパスを設定)
	_fx->SetDirectory(L"Resources/Models");

	// モデルデータの読み込み
	std::unique_ptr<DX11::Model> _model = Model::CreateFromCMO(
		device,
		filename,
		*_fx
	);

	// 作成したモデルを返却
	return _model;
}

/// <summary>
/// モデルの解放
/// </summary>
/// <param name="model">解放したいモデルデータ</param>
/// <returns>なし</returns>
void ModelFactory::DeleteModel(std::unique_ptr<Model>& model)
{
	model.reset();
}
