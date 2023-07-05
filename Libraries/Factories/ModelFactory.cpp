/*
 *	@File	ModelFactory.cpp
 *	@Brief	モデルを作成するファクトリークラス。
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ModelFactory.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
ModelFactory::ModelFactory()
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
ModelFactory::~ModelFactory()
{
}

//--------------------------------------------------------//
//モデルを作成するファクトリー                            //
//--------------------------------------------------------//
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

//--------------------------------------------------------//
//空だった場合、nullを設定して仮引数のある方の処理をする  //
//--------------------------------------------------------//
void ModelFactory::DeleteModel()
{
	// 空の引数を渡す(例外スロー防止用オーバーロード)
	std::unique_ptr<Model> nullModel = nullptr;
	DeleteModel(nullModel);
}

//--------------------------------------------------------//
//モデルを破棄する                                        //
//--------------------------------------------------------//
void ModelFactory::DeleteModel(std::unique_ptr<Model>& model)
{
	model.reset();
}
