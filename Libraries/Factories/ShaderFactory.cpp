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
void ShaderFactory::CreateInputLayout(std::vector<uint8_t> size,Microsoft::WRL::ComPtr<ID3D11InputLayout>* inputLayout)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();
	DX::ThrowIfFailed(
		_device->CreateInputLayout(
			&INPUT_LAYOUT[0],
			static_cast<UINT>(INPUT_LAYOUT.size()),
			size.data(),
			size.size(),
			(*inputLayout).GetAddressOf())
	);
}

// コンスタントバッファを作成
template<typename T>
void ShaderFactory::CreateConstBuffer(T* constantBuffer)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// コンスタントバッファ定義
	D3D11_BUFFER_DESC _buffer = {};

	// 中身を空にする
	ZeroMemory(&_buffer, sizeof(_buffer));

	// 読み書きのモードをデフォルトにする
	_buffer.Usage = D3D11_USAGE_DEFAULT;

	// シェーダーで使うデータ構造体のサイズを格納
	_buffer.ByteWidth = sizeof(T);

	// バッファーを定数バッファーとして紐づける
	_buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// CPUから書き換えをできなくする
	_buffer.CPUAccessFlags = NULL;

	// 作成したバッファを格納
	_device->CreateBuffer(&_buffer, nullptr, &constantBuffer);
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

