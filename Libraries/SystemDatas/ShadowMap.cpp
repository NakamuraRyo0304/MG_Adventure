/*
 *	@File	ShadowMap.cpp
 *	@Brief	�V���h�E�}�b�v�̓K�p�N���X
 *	@Date	2023-12-09
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "ReadData.h"
#include "Libraries/Factories/ModelFactory.h"
#include "ShadowMap.h"

//==============================================================================
// �V���h�E�}�b�v
//==============================================================================
const int ShadowMap::SIZE = 256;
const float ShadowMap::AMBIENT_INTENSITY = 0.3f;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
ShadowMap::ShadowMap(ID3D11Device1* device)
	: m_lightRadius{}
{

	RECT _rect = { 0, 0, SIZE, SIZE };

	// �����_�[�e�N�X�`���̍쐬
	auto _rt = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	SetRenderTexture(_rt);
	GetRenderTexture()->SetDevice(device);
	GetRenderTexture()->SetWindow(_rect);

	// �f�v�X�X�e���V���̍쐬
	auto _dp = std::make_unique<DepthStencil>(DXGI_FORMAT_D32_FLOAT);
	SetDepthStencil(_dp);
	GetDepthStencil()->SetDevice(device);
	GetDepthStencil()->SetWindow(_rect);

	// �f�v�X�V�F�[�_�[���쐬
	std::vector<uint8_t> _vs_depth = DX::ReadData(L"Resources/Shaders/ShadowMap/SM_VS_Depth.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(_vs_depth.data(), _vs_depth.size(), nullptr, GetVS().Depth.ReleaseAndGetAddressOf()));

	std::vector<uint8_t> _ps_depth = DX::ReadData(L"Resources/Shaders/ShadowMap/SM_PS_Depth.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(_ps_depth.data(), _ps_depth.size(), nullptr, GetPS().Depth.ReleaseAndGetAddressOf()));

	// �萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC _normalDesc = {};
	_normalDesc.ByteWidth = static_cast<UINT>(sizeof(NormalBuffer));
	_normalDesc.Usage = D3D11_USAGE_DYNAMIC;
	_normalDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	_normalDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DX::ThrowIfFailed(
		device->CreateBuffer(&_normalDesc, nullptr, GetNormalBuffer().ReleaseAndGetAddressOf()));

	// �e�N�X�`���V�F�[�_�[���쐬
	std::vector<uint8_t> _vs = DX::ReadData(L"Resources/Shaders/ShadowMap/SM_VS.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(_vs.data(), _vs.size(), nullptr, GetVS().Normal.ReleaseAndGetAddressOf()));

	std::vector<uint8_t> _ps = DX::ReadData(L"Resources/Shaders/ShadowMap/SM_PS.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(_ps.data(), _ps.size(), nullptr, GetPS().Normal.ReleaseAndGetAddressOf()));

	// �T���v���[�̍쐬
	D3D11_SAMPLER_DESC _samDesc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	_samDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	_samDesc.AddressU = _samDesc.AddressV = _samDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	_samDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	device->CreateSamplerState(&_samDesc, m_sampler.ReleaseAndGetAddressOf());
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
ShadowMap::~ShadowMap()
{
	m_renderTexture.reset();
	m_depthStencil.reset();
	m_sampler.Reset();
	m_normalBuffer.Reset();
}

//==============================================================================
// �e�̊J�n����(�p�X�@�`�C�����s����)
//==============================================================================
void ShadowMap::Begin(ID3D11DeviceContext1* context, SimpleMath::Matrix& proj)
{
	// �p�X�@�e�ϊ����J�n
	Path1StartTransport();

	// �p�X�A���C�g�r���[���v�Z
	Path2CalcViewMatrix();

	// �p�X�B�o�b�t�@���X�V
	Path3UpdateBuffer(context, proj);
}

//==============================================================================
// �e�̏I������(�p�X�D�����s����)
//==============================================================================
void ShadowMap::End(ID3D11DeviceContext1* context)
{

	// �p�X�C�V���h�E�}�b�v���쐬
	Path4CreateShadowOption(context);
	// �p�X�D�e�ϊ����I��
	Path5EndTransport();
}

//==============================================================================
// �e�̉������(�p�X�E�����s����)
//==============================================================================
void ShadowMap::Exit(ID3D11DeviceContext1* context)
{
	// �p�X�E���\�[�X�����
	Path6ReleaseResources(context);
}

//==============================================================================
// �@�e�ϊ����J�n
//==============================================================================
void ShadowMap::Path1StartTransport()
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	auto _rtvSM = GetRenderTexture()->GetRenderTargetView();
	auto _dsvSM = GetDepthStencil()->GetDepthStencilView();

	// �r���[�|�[�g�������h��Ԃ�
	_context->ClearRenderTargetView(_rtvSM, Colors::White);
	_context->ClearDepthStencilView(_dsvSM, D3D11_CLEAR_DEPTH, 1.f, 0);
	_context->OMSetRenderTargets(1, &_rtvSM, _dsvSM);

	// �r���[�|�[�g�𔽉f
	D3D11_VIEWPORT _vpSM = { 0.f, 0.f,static_cast<float>(SIZE), static_cast<float>(SIZE), 0.f, 1.f };
	_context->RSSetViewports(1, &_vpSM);
}

//==============================================================================
// �A���C�g�r���[���v�Z
//==============================================================================
void ShadowMap::Path2CalcViewMatrix()
{
	// �������v�Z
	SimpleMath::Vector3 _lightDir = SimpleMath::Vector3::Transform(
		SimpleMath::Vector3::UnitZ,		// ��
		GetLightRotate()				// ��]
	);

	// �r���[�s����v�Z
	SimpleMath::Matrix _lightView = SimpleMath::Matrix::CreateLookAt(
		GetLightPosition(),				// ���W
		GetLightPosition() + _lightDir,	// �����_
		SimpleMath::Vector3::UnitY		// �����
	);

	// �������Z�b�g
	SetLightDirection(_lightDir);

	// �v�Z���ʂ��Z�b�g
	SetViewMatrix(_lightView);
}

//==============================================================================
// �B�o�b�t�@���X�V
//==============================================================================
void ShadowMap::Path3UpdateBuffer(ID3D11DeviceContext1* context, SimpleMath::Matrix& proj)
{
	// �r���[�s����擾
	auto& _view = GetViewMatrix();

	// �o�b�t�@�ւ̃A�N�Z�X����
	D3D11_MAPPED_SUBRESOURCE _map;
	context->Map(GetNormalBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &_map);

	// �o�b�t�@�̍X�V
	NormalBuffer _normalBuff = {};
	_normalBuff.ViewProj = XMMatrixTranspose(_view * proj);
	_normalBuff.Position = GetLightPosition();
	_normalBuff.Ambient = Colors::White * AMBIENT_INTENSITY;

	// �o�b�t�@�𔽉f�E��������
	*static_cast<NormalBuffer*>(_map.pData) = _normalBuff;
	context->Unmap(GetNormalBuffer().Get(), 0);
}

//==============================================================================
// �C�e�̃����_�����Z�b�g
//==============================================================================
void ShadowMap::Path4CreateShadowOption(ID3D11DeviceContext1* context)
{
	SetShadowLambda([&]() {
		context->VSSetShader(GetVS().Depth.Get(), nullptr, 0);
		context->PSSetShader(GetPS().Depth.Get(), nullptr, 0); });
}

//==============================================================================
// �D�e�ϊ����I��
//==============================================================================
void ShadowMap::Path5EndTransport()
{
	auto _pDR = DX::DeviceResources::GetInstance();
	auto _context = _pDR->GetD3DDeviceContext();
	auto _rtvDef  = _pDR->GetRenderTargetView();
	auto _dsvDef  = _pDR->GetDepthStencilView();

	// �r���[�|�[�g��߂�
	_context->ClearRenderTargetView(_rtvDef, Colors::White);
	_context->ClearDepthStencilView(_dsvDef, USER_DEPTH_MODE, 1.f, 0);
	_context->OMSetRenderTargets(1, &_rtvDef, _dsvDef);

	// �r���[�|�[�g�𔽉f
	auto const _vpDef = _pDR->GetScreenViewport();
	_context->RSSetViewports(1, &_vpDef);
}

//==============================================================================
// �E���\�[�X�̉��
//==============================================================================
void ShadowMap::Path6ReleaseResources(ID3D11DeviceContext1* context)
{
	ID3D11ShaderResourceView* _nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, _nullsrv);
}