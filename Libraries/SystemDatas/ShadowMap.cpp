/*
 *	@File	ShadowMap.cpp
 *	@Brief	シャドウマップの適用クラス
 *	@Date	2023-12-09
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "ReadData.h"
#include "Libraries/Factories/ModelFactory.h"
#include "ShadowMap.h"

//==============================================================================
// シャドウマップ
//==============================================================================
const int ShadowMap::SIZE = 256;
const float ShadowMap::AMBIENT_INTENSITY = 0.3f;

//==============================================================================
// コンストラクタ
//==============================================================================
ShadowMap::ShadowMap(ID3D11Device1* device)
	: m_lightRadius{}
{

	RECT _rect = { 0, 0, SIZE, SIZE };

	// レンダーテクスチャの作成
	auto _rt = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	SetRenderTexture(_rt);
	GetRenderTexture()->SetDevice(device);
	GetRenderTexture()->SetWindow(_rect);

	// デプスステンシルの作成
	auto _dp = std::make_unique<DepthStencil>(DXGI_FORMAT_D32_FLOAT);
	SetDepthStencil(_dp);
	GetDepthStencil()->SetDevice(device);
	GetDepthStencil()->SetWindow(_rect);

	// デプスシェーダーを作成
	std::vector<uint8_t> _vs_depth = DX::ReadData(L"Resources/Shaders/ShadowMap/SM_VS_Depth.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(_vs_depth.data(), _vs_depth.size(), nullptr, GetVS().Depth.ReleaseAndGetAddressOf()));

	std::vector<uint8_t> _ps_depth = DX::ReadData(L"Resources/Shaders/ShadowMap/SM_PS_Depth.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(_ps_depth.data(), _ps_depth.size(), nullptr, GetPS().Depth.ReleaseAndGetAddressOf()));

	// 定数バッファの作成
	D3D11_BUFFER_DESC _normalDesc = {};
	_normalDesc.ByteWidth = static_cast<UINT>(sizeof(NormalBuffer));
	_normalDesc.Usage = D3D11_USAGE_DYNAMIC;
	_normalDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	_normalDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DX::ThrowIfFailed(
		device->CreateBuffer(&_normalDesc, nullptr, GetNormalBuffer().ReleaseAndGetAddressOf()));

	// テクスチャシェーダーを作成
	std::vector<uint8_t> _vs = DX::ReadData(L"Resources/Shaders/ShadowMap/SM_VS.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(_vs.data(), _vs.size(), nullptr, GetVS().Normal.ReleaseAndGetAddressOf()));

	std::vector<uint8_t> _ps = DX::ReadData(L"Resources/Shaders/ShadowMap/SM_PS.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(_ps.data(), _ps.size(), nullptr, GetPS().Normal.ReleaseAndGetAddressOf()));

	// サンプラーの作成
	D3D11_SAMPLER_DESC _samDesc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	_samDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	_samDesc.AddressU = _samDesc.AddressV = _samDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	_samDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	device->CreateSamplerState(&_samDesc, m_sampler.ReleaseAndGetAddressOf());
}

//==============================================================================
// デストラクタ
//==============================================================================
ShadowMap::~ShadowMap()
{
	m_renderTexture.reset();
	m_depthStencil.reset();
	m_sampler.Reset();
	m_normalBuffer.Reset();
}

//==============================================================================
// 影の開始処理(パス①～④を実行する)
//==============================================================================
void ShadowMap::Begin(ID3D11DeviceContext1* context, SimpleMath::Matrix& proj)
{
	// パス①影変換を開始
	Path1StartTransport();

	// パス②ライトビューを計算
	Path2CalcViewMatrix();

	// パス③バッファを更新
	Path3UpdateBuffer(context, proj);
}

//==============================================================================
// 影の終了処理(パス⑤を実行する)
//==============================================================================
void ShadowMap::End(ID3D11DeviceContext1* context)
{

	// パス④シャドウマップを作成
	Path4CreateShadowOption(context);
	// パス⑤影変換を終了
	Path5EndTransport();
}

//==============================================================================
// 影の解放処理(パス⑥を実行する)
//==============================================================================
void ShadowMap::Exit(ID3D11DeviceContext1* context)
{
	// パス⑥リソースを解放
	Path6ReleaseResources(context);
}

//==============================================================================
// ①影変換を開始
//==============================================================================
void ShadowMap::Path1StartTransport()
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	auto _rtvSM = GetRenderTexture()->GetRenderTargetView();
	auto _dsvSM = GetDepthStencil()->GetDepthStencilView();

	// ビューポートを黒く塗りつぶす
	_context->ClearRenderTargetView(_rtvSM, Colors::White);
	_context->ClearDepthStencilView(_dsvSM, D3D11_CLEAR_DEPTH, 1.f, 0);
	_context->OMSetRenderTargets(1, &_rtvSM, _dsvSM);

	// ビューポートを反映
	D3D11_VIEWPORT _vpSM = { 0.f, 0.f,static_cast<float>(SIZE), static_cast<float>(SIZE), 0.f, 1.f };
	_context->RSSetViewports(1, &_vpSM);
}

//==============================================================================
// ②ライトビューを計算
//==============================================================================
void ShadowMap::Path2CalcViewMatrix()
{
	// 方向を計算
	SimpleMath::Vector3 _lightDir = SimpleMath::Vector3::Transform(
		SimpleMath::Vector3::UnitZ,		// 軸
		GetLightRotate()				// 回転
	);

	// ビュー行列を計算
	SimpleMath::Matrix _lightView = SimpleMath::Matrix::CreateLookAt(
		GetLightPosition(),				// 座標
		GetLightPosition() + _lightDir,	// 注視点
		SimpleMath::Vector3::UnitY		// 上方向
	);

	// 方向をセット
	SetLightDirection(_lightDir);

	// 計算結果をセット
	SetViewMatrix(_lightView);
}

//==============================================================================
// ③バッファを更新
//==============================================================================
void ShadowMap::Path3UpdateBuffer(ID3D11DeviceContext1* context, SimpleMath::Matrix& proj)
{
	// ビュー行列を取得
	auto& _view = GetViewMatrix();

	// バッファへのアクセス制限
	D3D11_MAPPED_SUBRESOURCE _map;
	context->Map(GetNormalBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &_map);

	// バッファの更新
	NormalBuffer _normalBuff = {};
	_normalBuff.ViewProj = XMMatrixTranspose(_view * proj);
	_normalBuff.Position = GetLightPosition();
	_normalBuff.Ambient = Colors::White * AMBIENT_INTENSITY;

	// バッファを反映・制限解除
	*static_cast<NormalBuffer*>(_map.pData) = _normalBuff;
	context->Unmap(GetNormalBuffer().Get(), 0);
}

//==============================================================================
// ④影のラムダ式をセット
//==============================================================================
void ShadowMap::Path4CreateShadowOption(ID3D11DeviceContext1* context)
{
	SetShadowLambda([&]() {
		context->VSSetShader(GetVS().Depth.Get(), nullptr, 0);
		context->PSSetShader(GetPS().Depth.Get(), nullptr, 0); });
}

//==============================================================================
// ⑤影変換を終了
//==============================================================================
void ShadowMap::Path5EndTransport()
{
	auto _pDR = DX::DeviceResources::GetInstance();
	auto _context = _pDR->GetD3DDeviceContext();
	auto _rtvDef  = _pDR->GetRenderTargetView();
	auto _dsvDef  = _pDR->GetDepthStencilView();

	// ビューポートを戻す
	_context->ClearRenderTargetView(_rtvDef, Colors::White);
	_context->ClearDepthStencilView(_dsvDef, USER_DEPTH_MODE, 1.f, 0);
	_context->OMSetRenderTargets(1, &_rtvDef, _dsvDef);

	// ビューポートを反映
	auto const _vpDef = _pDR->GetScreenViewport();
	_context->RSSetViewports(1, &_vpDef);
}

//==============================================================================
// ⑥リソースの解放
//==============================================================================
void ShadowMap::Path6ReleaseResources(ID3D11DeviceContext1* context)
{
	ID3D11ShaderResourceView* _nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, _nullsrv);
}