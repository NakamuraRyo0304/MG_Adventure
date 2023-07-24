/*
 *	@File	PlayerShadow.cpp
 *	@Brief	�v���C���[�̉e�B
 *	@Date	2023-07-24
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "DeviceResources.h"
#include "Libraries/SystemDatas/LoadFile.h"
#include <vector>

#include "PlayerShadow.h"

 /// <summary>
 /// �C���v�b�g���C�A�E�g
 /// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerShadow::INPUT_LAYOUT =
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
PlayerShadow::PlayerShadow(DX::DeviceResources* pDR) :
	m_deviceResources{ pDR },
	m_position{}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
PlayerShadow::~PlayerShadow()
{
}

/// <summary>
/// �����֐�
/// </summary>
/// <param name="">�Ȃ�</param>
/// <returns>�Ȃ�</returns>
void PlayerShadow::Create()
{
	// �f�o�C�X�̍쐬
	auto device = m_deviceResources->GetD3DDevice();

	//�V�F�[�_�[�̍쐬
	CreateShader();

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_deviceResources->GetD3DDeviceContext());

	// �R�����X�e�[�g�̍쐬
	m_states = std::make_unique<CommonStates>(device);

	// ���W�̃��Z�b�g
	m_position = SimpleMath::Vector3::Zero;

	// �摜�ǂݍ���
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/PlayerShadow.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="position">�v���C���[�̍��W</param>
/// <returns>�Ȃ�</returns>
void PlayerShadow::Update(const DirectX::SimpleMath::Vector3& position)
{
	m_position = position;
	m_position.y += 0.5f;
}

/// <summary>
/// �V�F�[�_�[�쐬�֐�
/// </summary>
/// <param name="">�Ȃ�</param>
/// <returns>�Ȃ�</returns>
void PlayerShadow::CreateShader()
{
	auto device = m_deviceResources->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	LoadFile VSData = std::move(LoadFile::LoadBinaryFile(L"Resources/Shaders/PlayerShadowVS.cso"));
	LoadFile GSData = std::move(LoadFile::LoadBinaryFile(L"Resources/Shaders/PlayerShadowGS.cso"));
	LoadFile PSData = std::move(LoadFile::LoadBinaryFile(L"Resources/Shaders/PlayerShadowPS.cso"));

	//�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// �R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_constBuffer);
}

/// <summary>
/// �`��֐�
/// </summary>
/// <param name="context">�f�o�C�X�R���e�L�X�g�̃|�C���^</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <returns>�Ȃ�</returns>
void PlayerShadow::Render(ID3D11DeviceContext* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	// ���_���
	VertexPositionColorTexture vertex[1] =
	{
		VertexPositionColorTexture(m_position,SimpleMath::Vector4::One,SimpleMath::Vector2(0.0f, 0.0f))
	};

	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_world.Transpose();
	cbuff.Diffuse = SimpleMath::Vector4::One;

	// �󂯓n���p�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_constBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_constBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// �摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// �������`��w��
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// �J�����O�͍�����
	context->RSSetState(m_states->CullNone());

	// �摜���Z�b�g
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	// �V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// �C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	// �V�F�[�_�̓o�^����������
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
