/*
 *	@File	ModelFactory.cpp
 *	@Brief	モデルを作成するファクトリークラス。
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ModelFactory.h"

// コンストラクタ
ModelFactory::ModelFactory()
{
}

// デストラクタ
ModelFactory::~ModelFactory()
{
}

// モデル作成関数
std::unique_ptr<Model> ModelFactory::GetCreateModel(const wchar_t* filename)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// モデルの作成(作成時のみの使い捨て)
	std::unique_ptr<EffectFactory> _fx =
		std::make_unique<EffectFactory>(_device);

	// ディレクトリの設定 (ここにモデルがあるパスを設定)
	_fx->SetDirectory(L"Resources/Models");

	// モデルデータの読み込み
	std::unique_ptr<Model> _model = Model::CreateFromCMO(
		_device,
		filename,
		*_fx
	);

	// 作成したモデルを返却
	return _model;
}

// モデル破棄関数
void ModelFactory::DeleteModel(std::unique_ptr<Model>& model)
{
	model.reset();
}
