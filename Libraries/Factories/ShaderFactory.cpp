/*
 *	@File	ShaderFactory.cpp
 *	@Brief	シェーダーを作成するファクトリー。
 *	@Date	2023-11-04
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "ShaderFactory.h"

// コンストラクタ
ShaderFactory::ShaderFactory()
	: m_vertexSize{}
{
	m_vertexSize.clear();
}

// デストラクタ
ShaderFactory::~ShaderFactory()
{
	m_vertexSize.clear();
}

// インプットレイアウトを作成
void ShaderFactory::CreateInputLayout(Microsoft::WRL::ComPtr<ID3D11InputLayout>* inputLayout)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();
	DX::ThrowIfFailed(
		_device->CreateInputLayout(
			&INPUT_LAYOUT[0],
			static_cast<UINT>(INPUT_LAYOUT.size()),
			m_vertexSize.data(),
			m_vertexSize.size(),
			(*inputLayout).GetAddressOf())
	);
}

// コンスタントバッファを作成
template<typename T>
void ShaderFactory::CreateConstBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& constBuffer)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// コンスタントバッファ定義
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	// 読み書きのモードをデフォルトにする
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	// シェーダーで使うデータ構造体のサイズを格納
	bufferDesc.ByteWidth = sizeof(T);

	// バッファーを定数バッファーとして紐づける
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// CPUから書き換えをできなくする
	bufferDesc.CPUAccessFlags = 0;

	// 作成したバッファを格納
	 _device->CreateBuffer(&bufferDesc, nullptr, constBuffer.GetAddressOf());

}
// 頂点シェーダーを作成
void ShaderFactory::CreateVertexShader(const wchar_t* shaderPath,
	Microsoft::WRL::ComPtr<ID3D11VertexShader>* shader)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// パスを読み込む
	std::vector<uint8_t> _Data = DX::ReadData(shaderPath);

	DX::ThrowIfFailed(_device->CreateVertexShader(
		_Data.data(), _Data.size(), nullptr,
		(*shader).ReleaseAndGetAddressOf()));
	m_vertexSize = _Data;
}
// ピクセルシェーダーを作成
void ShaderFactory::CreatePixelShader(const wchar_t* shaderPath,
	Microsoft::WRL::ComPtr<ID3D11PixelShader>* shader)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// パスを読み込む
	std::vector<uint8_t> _Data = DX::ReadData(shaderPath);

	DX::ThrowIfFailed(_device->CreatePixelShader(
		_Data.data(), _Data.size(), nullptr,
		(*shader).ReleaseAndGetAddressOf()));
}
// ジオメトリシェーダーを作成
void ShaderFactory::CreateGeometryShader(const wchar_t* shaderPath,
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>* shader)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// パスを読み込む
	std::vector<uint8_t> _Data = DX::ReadData(shaderPath);

	DX::ThrowIfFailed(_device->CreateGeometryShader(
		_Data.data(), _Data.size(), nullptr,
		(*shader).ReleaseAndGetAddressOf()));
}

