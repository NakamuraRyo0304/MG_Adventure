/*
 *	@File	PlayerBill.cpp
 *	@Brief	プレイヤーの位置情報を描画するクラス。
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Libraries/SystemDatas/ParticleUtility.h"
#include "Libraries/SystemDatas/LoadFile.h"

#include "PlayerBill.h"

/// <summary>
/// インプットレイアウトの設定
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerBill::INPUT_LAYOUT =
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
PlayerBill::PlayerBill():
	m_pDR{nullptr},
	m_defaultPos{SimpleMath::Vector3::Zero}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
PlayerBill::~PlayerBill()
{
}

/// <summary>
/// 画像読み込み
/// </summary>
/// <param name="path">画像パス</param>
/// <returns>なし</returns>
void PlayerBill::LoadTexture(const wchar_t* path)
{
	// デバイス
	auto device = m_pDR->GetD3DDevice();
	// シェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex;

	// ファイル読み込み
	DirectX::CreateWICTextureFromFile(
		device,
		path,
		nullptr, 
		tex.ReleaseAndGetAddressOf()
	);

	// 画像を追加
	m_textures.push_back(tex);
}

/// <summary>
/// リソースの作成
/// </summary>
/// <param name="pDR">デバイスリソースポインタ</param>
/// <returns>なし</returns>
void PlayerBill::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	// デバイスの取得
	auto device = pDR->GetD3DDevice();

	// シェーダーの作成
	CreateShader();

	// 画像の読み込み（読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/PlayerPoint.png");

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	// コモンステートの作成
	m_common = std::make_unique<CommonStates>(device);

	// 座標の初期化
	m_defaultPos = SimpleMath::Vector3{ 0.0f,0.0f,0.0f };
}

/// <summary>
/// 描画
/// </summary>
/// <param name="playerPos">プレイヤの座標</param>
/// <param name="timer">タイマーSinfで使う</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <returns>なし</returns>
void PlayerBill::Render(DirectX::SimpleMath::Vector3 playerPos, float timer, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_pDR->GetD3DDeviceContext();

	// 頂点情報(板ポリゴンの１頂点の座標情報）
	VertexPositionColorTexture vertex = VertexPositionColorTexture(
		m_defaultPos,					// 座標
		SimpleMath::Vector4::One,		// 色情報
		SimpleMath::Vector2::Zero		// テクスチャ座標
	);
	
	// ビルボード位置を更新
	vertex.position = DirectX::SimpleMath::Vector3(playerPos.x, playerPos.y + (2.0f + sinf(timer) * 0.2f), playerPos.z);

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			// カメラ正面の距離でソート
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});

	// 登録されている頂点をリセット
	m_vertices.clear();

	for (auto& li : m_particleUtility)
	{
		// カメラの後ろにいたら処理しない
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f)continue;

		// 頂点情報の初期化
		VertexPositionColorTexture vPCT;										// 宣言
		vPCT.position = XMFLOAT3(li.GetPosition());								// 座標
		vPCT.color = XMFLOAT4(li.GetNowColor());								// 色
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);			// 画像

		m_vertices.push_back(vPCT);
	}

	// シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose();
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_constBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_constBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// 画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_common->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// 半透明描画指定
	ID3D11BlendState* blendstate = m_common->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// ブロックの奥でもビルボードを貫通させる
	context->OMSetDepthStencilState(m_common->DepthNone(), 0);

	// カリングは左周り
	context->RSSetState(m_common->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_verShader.Get(), nullptr, 0);
	context->GSSetShader(m_geoShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する
	for (int i = 0; i < m_textures.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_textures[i].GetAddressOf());
	}

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex, 1);
	m_batch->End();

	//シェーダの解放
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

/// <summary>
/// シェーダーの作成
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayerBill::CreateShader()
{
	// デバイス
	auto device = m_pDR->GetD3DDevice();

	// シェーダーファイルの読み込み
	// バーテックスシェーダー
	LoadFile VSData = LoadFile::LoadBinaryFile(L"Resources/Shaders/PlayerPointVS.cso");
	// ジオメトリシェーダー
	LoadFile GSData = LoadFile::LoadBinaryFile(L"Resources/Shaders/PlayerPointGS.cso");
	// ピクセルシェーダー
	LoadFile PSData = LoadFile::LoadBinaryFile(L"Resources/Shaders/PlayerPointPS.cso");

	// インプットレイアウトの作成
	device->CreateInputLayout(
		&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(),
		VSData.GetSize(),
		m_inputLayout.GetAddressOf()
	);

	// バーテックスシェーダーの作成
	device->CreateVertexShader(
		VSData.GetData(),
		VSData.GetSize(), 
		nullptr,
		m_verShader.ReleaseAndGetAddressOf()
	);

	// ジオメトリシェーダーの作成
	device->CreateGeometryShader(
		GSData.GetData(),
		GSData.GetSize(),
		nullptr,
		m_geoShader.ReleaseAndGetAddressOf()
	);

	// ピクセルシェーダーの作成
	device->CreatePixelShader(
		PSData.GetData(), 
		PSData.GetSize(), 
		nullptr,
		m_pixShader.ReleaseAndGetAddressOf()
	);

	// コンスタントバッファ作成
	CreateConstBuffer(device);
}

/// <summary>
/// コンスタントバッファ作成
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayerBill::CreateConstBuffer(ID3D11Device1*& device)
{
	// コンスタントバッファ定義
	D3D11_BUFFER_DESC buffer;

	// 中身を空にする
	ZeroMemory(&buffer, sizeof(buffer));

	buffer.Usage = D3D11_USAGE_DEFAULT;

	// シェーダーで使うデータ構造体のサイズを格納
	buffer.ByteWidth = sizeof(ConstBuffer);
	buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	buffer.CPUAccessFlags = NULL;

	// 作成したバッファを格納
	device->CreateBuffer(&buffer, nullptr, &m_constBuffer);
}

/// <summary>
/// ビルボード作成関数
/// </summary>
/// <param name="target">カメラターゲット（注視点）</param>
/// <param name="eye">カメラアイ（カメラ座標）</param>
/// <param name="up">上向きベクトル（基本はYのみ１のベクトル）</param>
/// <returns>なし</returns>
void PlayerBill::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
{
	m_billboard = SimpleMath::Matrix::CreateBillboard(SimpleMath::Vector3::Zero, eye - target, up);

	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;

	// ビルボードのテクスチャを回転
	m_billboard = rot * m_billboard;
}