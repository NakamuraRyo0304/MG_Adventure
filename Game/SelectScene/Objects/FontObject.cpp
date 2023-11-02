/*
 *	@File	FontObject.cpp
 *	@Brief	選択中のフォントを表示するクラス。
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

// モデルファクトリー
#include "Libraries/Factories/ModelFactory.h"

#include "FontObject.h"

// コンストラクタ
FontObject::FontObject(const int& safeStage, const int& maxStage)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	for (int i = 0; i < maxStage - safeStage; ++i)
	{
		// 0番目はエディタの文字
		if (i == 0)
		{
			m_fonts[0] = std::move(ModelFactory::GetCreateModel(_device, L"Resources/Models/StageEdit.cmo"));
		}
		else
		{
			std::wstring _path = L"Resources/Models/Stage" + std::to_wstring(i) + L".cmo";
			m_fonts[i] = std::move(ModelFactory::GetCreateModel(_device, _path.c_str()));
		}
	}
}

// デストラクタ
FontObject::~FontObject()
{
	for (int i = 0; i < 6; ++i)
	{
		m_fonts[i].reset();
	}
}

// 描画処理
void FontObject::Render(CommonStates& states, const int& selectNum, const float& rotate,
	const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj)
{
	// コンテキストの取得
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	// テキストの移動アニメーション
	SimpleMath::Matrix stageMat = CreateTextMatrix(rotate);

	// ステージ番号表示
	m_fonts[selectNum]->Draw(_context, states, stageMat, view, proj);
}

// テキストの回転
SimpleMath::Matrix FontObject::CreateTextMatrix(const float& rotValue)
{
	SimpleMath::Matrix _mat = SimpleMath::Matrix::Identity;
	_mat *= SimpleMath::Matrix::CreateRotationY(rotValue);
	_mat *= SimpleMath::Matrix::CreateScale(10.0f);
	_mat *= SimpleMath::Matrix::CreateTranslation(sinf(rotValue), 10.0f, cosf(rotValue));

	return _mat;
}
