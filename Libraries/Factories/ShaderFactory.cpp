/*
 *	@File	ShaderFactory.cpp
 *	@Brief	�V�F�[�_�[���쐬����t�@�N�g���[�B
 *	@Date	2023-11-04
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "ShaderFactory.h"

// �R���X�g���N�^
ShaderFactory::ShaderFactory()
	: m_vertexSize{}
{
	m_vertexSize.clear();
}

// �f�X�g���N�^
ShaderFactory::~ShaderFactory()
{
	m_vertexSize.clear();
}

// �C���v�b�g���C�A�E�g���쐬
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

// �R���X�^���g�o�b�t�@���쐬
template<typename T>
void ShaderFactory::CreateConstBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& constBuffer)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// �R���X�^���g�o�b�t�@��`
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	// �ǂݏ����̃��[�h���f�t�H���g�ɂ���
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	// �V�F�[�_�[�Ŏg���f�[�^�\���̂̃T�C�Y���i�[
	bufferDesc.ByteWidth = sizeof(T);

	// �o�b�t�@�[��萔�o�b�t�@�[�Ƃ��ĕR�Â���
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// CPU���珑���������ł��Ȃ�����
	bufferDesc.CPUAccessFlags = 0;

	// �쐬�����o�b�t�@���i�[
	 _device->CreateBuffer(&bufferDesc, nullptr, constBuffer.GetAddressOf());

}
// ���_�V�F�[�_�[���쐬
void ShaderFactory::CreateVertexShader(const wchar_t* shaderPath,
	Microsoft::WRL::ComPtr<ID3D11VertexShader>* shader)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// �p�X��ǂݍ���
	std::vector<uint8_t> _Data = DX::ReadData(shaderPath);

	DX::ThrowIfFailed(_device->CreateVertexShader(
		_Data.data(), _Data.size(), nullptr,
		(*shader).ReleaseAndGetAddressOf()));
	m_vertexSize = _Data;
}
// �s�N�Z���V�F�[�_�[���쐬
void ShaderFactory::CreatePixelShader(const wchar_t* shaderPath,
	Microsoft::WRL::ComPtr<ID3D11PixelShader>* shader)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// �p�X��ǂݍ���
	std::vector<uint8_t> _Data = DX::ReadData(shaderPath);

	DX::ThrowIfFailed(_device->CreatePixelShader(
		_Data.data(), _Data.size(), nullptr,
		(*shader).ReleaseAndGetAddressOf()));
}
// �W�I���g���V�F�[�_�[���쐬
void ShaderFactory::CreateGeometryShader(const wchar_t* shaderPath,
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>* shader)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// �p�X��ǂݍ���
	std::vector<uint8_t> _Data = DX::ReadData(shaderPath);

	DX::ThrowIfFailed(_device->CreateGeometryShader(
		_Data.data(), _Data.size(), nullptr,
		(*shader).ReleaseAndGetAddressOf()));
}

