/*
 *	@File	Transition.cpp
 *	@Brief	��ʐ؂�ւ��̃g�����W�V�����B
 *	@Date	2023-10-28
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ParticleUtility.h"

// �t�@�C���ǂݍ���
#include "../ReadData.h"

#include "Transition.h"

/// <summary>
/// �C���v�b�g���C�A�E�g�̐ݒ�
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Transition::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
Transition::Transition()
	: m_pDR{nullptr}
	, m_vertice{}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Transition::~Transition()
{
}

/// <summary>
/// ���\�[�X�̍쐬
/// </summary>
/// <param name="pDR">�f�o�C�X���\�[�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void Transition::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	// �摜�̓ǂݍ��݁i�ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/CloudRule.png");

	// �V�F�[�_�[�̍쐬
	CreateShader();

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_pDR->GetD3DDeviceContext());

	// �R�����X�e�[�g�̍쐬
	m_states = std::make_unique<CommonStates>(m_pDR->GetD3DDevice());
}

/// <summary>
/// �摜�ǂݍ���
/// </summary>
/// <param name="path">�摜�p�X</param>
/// <returns>�Ȃ�</returns>
void Transition::LoadTexture(const wchar_t* path)
{
	// �f�o�C�X
	auto _device = m_pDR->GetD3DDevice();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture = texture;
}

/// <summary>
/// �V�F�[�_�[�̍쐬
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Transition::CreateShader()
{
	// �f�o�C�X
	auto _device = m_pDR->GetD3DDevice();
	auto _context = m_pDR->GetD3DDeviceContext();

	//-------------------------------------------------------------------------------------//
	// �V�F�[�_�[�t�@�C���̓ǂݍ���

	// �o�[�e�b�N�X�V�F�[�_�[
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
	// �V�F�[�_�[�̍쐬

	// �C���v�b�g���C�A�E�g
	DX::ThrowIfFailed(
		_device->CreateInputLayout(
			&INPUT_LAYOUT[0],
			static_cast<UINT>(INPUT_LAYOUT.size()),
			_VSData.data(),
			_VSData.size(),
			m_inputLayout.GetAddressOf())
	);

	// �o�[�e�b�N�X�V�F�[�_�[
	DX::ThrowIfFailed(
		_device->CreateVertexShader(
			_VSData.data(),
			_VSData.size(),
			nullptr,
			m_verShader.ReleaseAndGetAddressOf()
		)
	);
	// �W�I���g���V�F�[�_�[
	DX::ThrowIfFailed(
		_device->CreateGeometryShader(
			_GSData.data(),
			_GSData.size(),
			nullptr,
			m_geoShader.ReleaseAndGetAddressOf()
		)
	);
	// �s�N�Z���V�F�[�_�[
	DX::ThrowIfFailed(
		_device->CreatePixelShader(
			_PSData.data(),
			_PSData.size(),
			nullptr,
			m_pixShader.ReleaseAndGetAddressOf()
		)
	);
	// �R���X�^���g�o�b�t�@�쐬
	CreateConstBuffer(_device);
}

/// <summary>
/// �R���X�^���g�o�b�t�@�쐬
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Transition::CreateConstBuffer(ID3D11Device1*& device)
{
	// �R���X�^���g�o�b�t�@��`
	D3D11_BUFFER_DESC _buffer = {};

	// ���g����ɂ���
	ZeroMemory(&_buffer, sizeof(_buffer));

	// �ǂݏ����̃��[�h���f�t�H���g�ɂ���
	_buffer.Usage = D3D11_USAGE_DEFAULT;

	// �V�F�[�_�[�Ŏg���f�[�^�\���̂̃T�C�Y���i�[
	_buffer.ByteWidth = sizeof(ConstBuffer);

	// �o�b�t�@�[��萔�o�b�t�@�[�Ƃ��ĕR�Â���
	_buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// CPU���珑���������ł��Ȃ�����
	_buffer.CPUAccessFlags = NULL;

	// �쐬�����o�b�t�@���i�[
	device->CreateBuffer(&_buffer, nullptr, &m_constBuffer);
}

/// <summary>
/// �`��
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <returns>�Ȃ�</returns>
void Transition::Render(const float& timer)
{
	auto context = m_pDR->GetD3DDeviceContext();

	// ���_���(�|���S���̂S���_�̍��W���j
	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(
		SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		SimpleMath::Vector4::One,
		SimpleMath::Vector2(0.0f, 0.0f)),
	};

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = SimpleMath::Matrix::Identity;
	cbuff.matProj = SimpleMath::Matrix::Identity;
	cbuff.matWorld = SimpleMath::Matrix::Identity;
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);
	cbuff.Time = SimpleMath::Vector4(timer, 1, 1, 1);

	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_constBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_constBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// �J�����O�͍�����
	context->RSSetState(m_states->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_verShader.Get(), nullptr, 0);
	context->GSSetShader(m_geoShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}