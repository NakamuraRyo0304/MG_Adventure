/*
 *	@File	Transition.cpp
 *	@Brief	場面切り替えのトランジション。
 *	@Date	2023-10-28
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ParticleUtility.h"

// ファイル読み込み
#include "../ReadData.h"

#include "Transition.h"

/// <summary>
/// インプットレイアウトの設定
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Transition::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
Transition::Transition()
	: m_pDR{nullptr}
	, m_vertice{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
Transition::~Transition()
{
}

/// <summary>
/// リソースの作成
/// </summary>
/// <param name="pDR">デバイスリソースポインタ</param>
/// <returns>なし</returns>
void Transition::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	// 画像の読み込み（読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/CloudRule.png");

	// シェーダーの作成
	CreateShader();

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_pDR->GetD3DDeviceContext());

	// コモンステートの作成
	m_states = std::make_unique<CommonStates>(m_pDR->GetD3DDevice());
}

/// <summary>
/// 画像読み込み
/// </summary>
/// <param name="path">画像パス</param>
/// <returns>なし</returns>
void Transition::LoadTexture(const wchar_t* path)
{
	// デバイス
	auto _device = m_pDR->GetD3DDevice();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture = texture;
}

/// <summary>
/// シェーダーの作成
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Transition::CreateShader()
{
	// デバイス
	auto _device = m_pDR->GetD3DDevice();
	auto _context = m_pDR->GetD3DDeviceContext();

	//-------------------------------------------------------------------------------------//
	// シェーダーファイルの読み込み

	// バーテックスシェーダー
	std::vector<uint8_t> _VSData = DX::ReadData(L"Resources/Shaders/VS_Transition.cso");
	DX::ThrowIfFailed(_device->CreateVertexShader(_VSData.data(), _VSData.size(), nullptr,
			m_verShader.ReleaseAndGetAddressOf())
	);
	std::vector<uint8_t> _GSData = DX::ReadData(L"Resources/Shaders/GS_Transition.cso");
	DX::ThrowIfFailed(_device->CreateGeometryShader(_GSData.data(), _GSData.size(), nullptr,
			m_geoShader.ReleaseAndGetAddressOf())
	);

	std::vector<uint8_t> _PSData = DX::ReadData(L"Resources/Shaders/PS_Transition.cso");
	DX::ThrowIfFailed(_device->CreatePixelShader(_PSData.data(), _PSData.size(), nullptr,
			m_pixShader.ReleaseAndGetAddressOf())
	);

	//-------------------------------------------------------------------------------------//
	// シェーダーの作成

	// インプットレイアウト
	DX::ThrowIfFailed(
		_device->CreateInputLayout(
			&INPUT_LAYOUT[0],
			static_cast<UINT>(INPUT_LAYOUT.size()),
			_VSData.data(),
			_VSData.size(),
			m_inputLayout.GetAddressOf())
	);

	// バーテックスシェーダー
	DX::ThrowIfFailed(
		_device->CreateVertexShader(
			_VSData.data(),
			_VSData.size(),
			nullptr,
			m_verShader.ReleaseAndGetAddressOf()
		)
	);
	// ジオメトリシェーダー
	DX::ThrowIfFailed(
		_device->CreateGeometryShader(
			_GSData.data(),
			_GSData.size(),
			nullptr,
			m_geoShader.ReleaseAndGetAddressOf()
		)
	);
	// ピクセルシェーダー
	DX::ThrowIfFailed(
		_device->CreatePixelShader(
			_PSData.data(),
			_PSData.size(),
			nullptr,
			m_pixShader.ReleaseAndGetAddressOf()
		)
	);
	// コンスタントバッファ作成
	CreateConstBuffer(_device);
}

/// <summary>
/// コンスタントバッファ作成
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Transition::CreateConstBuffer(ID3D11Device1*& device)
{
	// コンスタントバッファ定義
	D3D11_BUFFER_DESC _buffer = {};

	// 中身を空にする
	ZeroMemory(&_buffer, sizeof(_buffer));

	// 読み書きのモードをデフォルトにする
	_buffer.Usage = D3D11_USAGE_DEFAULT;

	// シェーダーで使うデータ構造体のサイズを格納
	_buffer.ByteWidth = sizeof(ConstBuffer);

	// バッファーを定数バッファーとして紐づける
	_buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// CPUから書き換えをできなくする
	_buffer.CPUAccessFlags = NULL;

	// 作成したバッファを格納
	device->CreateBuffer(&_buffer, nullptr, &m_constBuffer);
}

/// <summary>
/// 描画
/// </summary>
/// <param name="timer">タイマー</param>
/// <returns>なし</returns>
void Transition::Render(const float& timer)
{
	auto context = m_pDR->GetD3DDeviceContext();

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(
		SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		SimpleMath::Vector4::One,
		SimpleMath::Vector2(0.0f, 0.0f)),
	};

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = SimpleMath::Matrix::Identity;
	cbuff.matProj = SimpleMath::Matrix::Identity;
	cbuff.matWorld = SimpleMath::Matrix::Identity;
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);
	cbuff.Time = SimpleMath::Vector4(timer, 1, 1, 1);

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_constBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_constBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// カリングは左周り
	context->RSSetState(m_states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_verShader.Get(), nullptr, 0);
	context->GSSetShader(m_geoShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}