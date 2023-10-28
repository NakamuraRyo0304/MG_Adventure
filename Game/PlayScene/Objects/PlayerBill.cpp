/*
 *	@File	PlayerBill.cpp
 *	@Brief	プレイヤーの位置情報を描画するクラス。
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Libraries/SystemDatas/ParticleUtility.h"

// ファイル読み込み
#include "Libraries/ReadData.h"

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
PlayerBill::PlayerBill()
	: m_pDR{nullptr}
	, m_defaultPos{SimpleMath::Vector3::Zero}
	, m_vertice{}
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
/// リソースの作成
/// </summary>
/// <param name="pDR">デバイスリソースポインタ</param>
/// <returns>なし</returns>
void PlayerBill::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	// デバイスの取得
	auto _device = pDR->GetD3DDevice();

	// シェーダーの作成
	CreateShader();

	// 画像の読み込み（読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/PLAY_COMMON/PlayerPoint.dds");

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	// コモンステートの作成
	m_states = std::make_unique<CommonStates>(_device);

	// 座標の初期化
	m_defaultPos = SimpleMath::Vector3{ 0.0f,0.0f,0.0f };
}

/// <summary>
/// 画像読み込み
/// </summary>
/// <param name="path">画像パス</param>
/// <returns>なし</returns>
void PlayerBill::LoadTexture(const wchar_t* path)
{
	// デバイス
	auto _device = m_pDR->GetD3DDevice();
	// シェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _tex;

	// ファイル読み込み
	CreateDDSTextureFromFile(
		_device,
		path,
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);
}

/// <summary>
/// シェーダーの作成
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayerBill::CreateShader()
{
	// デバイス
	auto _device = m_pDR->GetD3DDevice();

	//-------------------------------------------------------------------------------------//
	// シェーダーファイルの読み込み

	// バーテックスシェーダー
	std::vector<uint8_t> _VSData = DX::ReadData(L"Resources/Shaders/VS_PlayerPoint.cso");
	DX::ThrowIfFailed(
		_device->CreateVertexShader(_VSData.data(), _VSData.size(), nullptr,
			m_verShader.ReleaseAndGetAddressOf())
	);
	// ジオメトリシェーダー
	std::vector<uint8_t> _GSData = DX::ReadData(L"Resources/Shaders/GS_PlayerPoint.cso");
	DX::ThrowIfFailed(
		_device->CreateGeometryShader(_GSData.data(), _GSData.size(), nullptr,
			m_geoShader.ReleaseAndGetAddressOf())
	);
	// ピクセルシェーダー
	std::vector<uint8_t> _PSData = DX::ReadData(L"Resources/Shaders/PS_PlayerPoint.cso");
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
void PlayerBill::CreateConstBuffer(ID3D11Device1*& device)
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
/// ビルボード作成関数
/// </summary>
/// <param name="target">カメラターゲット（注視点）</param>
/// <param name="eye">カメラアイ（カメラ座標）</param>
/// <param name="up">上向きベクトル（基本はYのみ１のベクトル）</param>
/// <returns>なし</returns>
void PlayerBill::CreateBillboard(SimpleMath::Vector3 target, SimpleMath::Vector3 eye, SimpleMath::Vector3 up)
{
	m_world = SimpleMath::Matrix::CreateBillboard(SimpleMath::Vector3::Zero, eye - target, up);

	// 回転を止めて常に正面に向ける
	SimpleMath::Matrix _rot = SimpleMath::Matrix::Identity;
	_rot._11 = -1;
	_rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;

	// ビルボードのテクスチャを回転
	m_world = _rot * m_world;
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
	auto _context = m_pDR->GetD3DDeviceContext();

	// 頂点情報
	VertexPositionColorTexture _vertex = VertexPositionColorTexture(
		m_defaultPos,					// 座標
		SimpleMath::Vector4::One,		// 色情報
		SimpleMath::Vector2::Zero		// テクスチャ座標
	);

	// ビルボード位置を更新
	_vertex.position = SimpleMath::Vector3(playerPos.x, playerPos.y + (2.0f + sinf(timer) * 0.2f), playerPos.z);

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	SimpleMath::Vector3 _cameraDir = m_cameraTarget - m_cameraPosition;
	_cameraDir.Normalize();

	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			// カメラ正面の距離でソート
			return _cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > _cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		}
	);

	for (auto& i : m_particleUtility)
	{
		// 頂点情報の初期化
		VertexPositionColorTexture _vPCT = {};							// 宣言
		_vPCT.position = XMFLOAT3(i.GetPosition());						// 座標
		_vPCT.color = XMFLOAT4(i.GetNowColor());						// 色
		_vPCT.textureCoordinate = XMFLOAT2(i.GetNowScale().x, 0.0f);	// 画像

		m_vertice = _vPCT;
	}

	// シェーダーにコンスタントバッファを渡す
	ConstBuffer _buffer;
	_buffer.matView = view.Transpose();
	_buffer.matProj = proj.Transpose();
	_buffer.matWorld = m_world.Transpose();
	_buffer.Diffuse = SimpleMath::Vector4::One;

	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	_context->UpdateSubresource(m_constBuffer.Get(), 0, NULL, &_buffer, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* _cbuffer[1] = { m_constBuffer.Get() };
	_context->VSSetConstantBuffers(0, 1, _cbuffer);
	_context->GSSetConstantBuffers(0, 1, _cbuffer);
	_context->PSSetConstantBuffers(0, 1, _cbuffer);

	// 画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	_context->PSSetSamplers(0, 1, sampler);

	// 半透明描画指定
	ID3D11BlendState* _blendState = m_states->NonPremultiplied();

	// 透明判定処理
	_context->OMSetBlendState(_blendState, nullptr, 0xFFFFFFFF);

	// ブロックの奥でもビルボードを貫通させる
	_context->OMSetDepthStencilState(m_states->DepthNone(), 0);

	// カリングは左周り
	_context->RSSetState(m_states->CullNone());

	// シェーダをセットする
	_context->VSSetShader(m_verShader.Get(), nullptr, 0);
	_context->GSSetShader(m_geoShader.Get(), nullptr, 0);
	_context->PSSetShader(m_pixShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する
	_context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//インプットレイアウトの登録
	_context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &_vertex, 1);
	m_batch->End();

	//シェーダの解放
	_context->VSSetShader(nullptr, nullptr, 0);
	_context->GSSetShader(nullptr, nullptr, 0);
	_context->PSSetShader(nullptr, nullptr, 0);
}