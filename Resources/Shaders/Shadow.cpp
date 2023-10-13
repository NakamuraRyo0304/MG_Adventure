/*
 *	@File	Shadow.cpp
 *	@Brief	影のシェーダー。
 *	@Date	2023-10-10
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ファイル読み込み
#include "Libraries/ReadData.h"

#include "Shadow.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
Shadow::Shadow()
	: m_pixelShader{}		// ピクセルシェーダー
	, m_world{}				// ワールド行列
	, m_view{}				// ビュー行列
	, m_proj{}				// プロジェクション行列
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
Shadow::~Shadow()
{
	m_pixelShader.Reset();
}

/// <summary>
/// 影シェーダー生成関数
/// </summary>
/// <param name="device">デバイスポインタ</param>
/// <returns>なし</returns>
void Shadow::CreateShadow(ID3D11Device1* device)
{
	// ピクセルシェーダーの作成
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);
}

/// <summary>
/// モデルをセットする
/// </summary>
/// <param name="model">モデルのパス</param>
/// <param name="key">登録キー</param>
/// <returns>なし</returns>
void Shadow::SetModel(const wchar_t* key, std::unique_ptr<Model> model)
{
	m_shadowModel.emplace(key, std::move(model));
}

/// <summary>
/// 描画関数
/// </summary>
/// <param name="key">登録キー</param>
/// <param name="context">コンテキストポインタ</param>
/// <param name="states">コモンステートポインタ</param>
/// <returns>なし</returns>
void Shadow::Draw(const wchar_t* key, ID3D11DeviceContext1* context, CommonStates* states)
{
	auto vol = [&]()
	{
		// ブレンドステートの設定
		context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xffffffff);
		// カリングしない
		context->RSSetState(states->CullNone());
		// ピクセルシェーダーの設定
		context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	};

	m_world *= SimpleMath::Matrix::CreateShadow(
			   SimpleMath::Vector3(0.f, 1.f, 0.f),
			   SimpleMath::Plane(0.f, 1.f, 0.f, 0.f)
	);

	// 影を描画する
 	std::map<const wchar_t*, std::unique_ptr<Model>>::const_iterator it = m_shadowModel.find(key);
	it->second->Draw(context, *states, m_world, m_view, m_proj, &vol);
}
