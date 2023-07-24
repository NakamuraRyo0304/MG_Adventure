/*
 *	@File	PlayerShadow.cpp
 *	@Brief	プレイヤーの影。
 *	@Date	2023-07-24
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "DeviceResources.h"
#include "Libraries/SystemDatas/LoadFile.h"
#include <vector>

#include "PlayerShadow.h"

 /// <summary>
 /// インプットレイアウト
 /// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerShadow::INPUT_LAYOUT =
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
PlayerShadow::PlayerShadow(DX::DeviceResources* pDR) :
	m_deviceResources{ pDR },
	m_position{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
PlayerShadow::~PlayerShadow()
{
}

/// <summary>
/// 生成関数
/// </summary>
/// <param name="">なし</param>
/// <returns>なし</returns>
void PlayerShadow::Create()
{
	// デバイスの作成
	auto device = m_deviceResources->GetD3DDevice();

	//シェーダーの作成
	CreateShader();

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_deviceResources->GetD3DDeviceContext());

	// コモンステートの作成
	m_states = std::make_unique<CommonStates>(device);

	// 座標のリセット
	m_position = SimpleMath::Vector3::Zero;

	// 画像読み込み
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/PlayerShadow.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">プレイヤーの座標</param>
/// <returns>なし</returns>
void PlayerShadow::Update(const DirectX::SimpleMath::Vector3& position)
{
	m_position = position;
	m_position.y += 0.5f;
}

/// <summary>
/// シェーダー作成関数
/// </summary>
/// <param name="">なし</param>
/// <returns>なし</returns>
void PlayerShadow::CreateShader()
{
	auto device = m_deviceResources->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	LoadFile VSData = std::move(LoadFile::LoadBinaryFile(L"Resources/Shaders/PlayerShadowVS.cso"));
	LoadFile GSData = std::move(LoadFile::LoadBinaryFile(L"Resources/Shaders/PlayerShadowGS.cso"));
	LoadFile PSData = std::move(LoadFile::LoadBinaryFile(L"Resources/Shaders/PlayerShadowPS.cso"));

	//インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// コンスタントバッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_constBuffer);
}

/// <summary>
/// 描画関数
/// </summary>
/// <param name="context">デバイスコンテキストのポインタ</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <returns>なし</returns>
void PlayerShadow::Render(ID3D11DeviceContext* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	// 頂点情報
	VertexPositionColorTexture vertex[1] =
	{
		VertexPositionColorTexture(m_position,SimpleMath::Vector4::One,SimpleMath::Vector2(0.0f, 0.0f))
	};

	// シェーダーに渡す追加のバッファを作成する。
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_world.Transpose();
	cbuff.Diffuse = SimpleMath::Vector4::One;

	// 受け渡し用バッファの内容更新
	context->UpdateSubresource(m_constBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_constBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// 画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// 半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// カリングは左周り
	context->RSSetState(m_states->CullNone());

	// 画像をセット
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	// シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	// シェーダの登録を解除する
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
