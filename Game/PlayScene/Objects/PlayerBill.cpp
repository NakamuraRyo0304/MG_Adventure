/*
 *	@File	PlayerBill.cpp
 *	@Brief	�v���C���[�̈ʒu����`�悷��N���X�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Libraries/SystemDatas/ParticleUtility.h"

// �t�@�C���ǂݍ���
#include "Libraries/ReadData.h"

#include "PlayerBill.h"

/// <summary>
/// �C���v�b�g���C�A�E�g�̐ݒ�
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerBill::INPUT_LAYOUT =
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
PlayerBill::PlayerBill()
	: m_pDR{nullptr}
	, m_defaultPos{SimpleMath::Vector3::Zero}
	, m_vertice{}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
PlayerBill::~PlayerBill()
{
}

/// <summary>
/// ���\�[�X�̍쐬
/// </summary>
/// <param name="pDR">�f�o�C�X���\�[�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void PlayerBill::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	// �f�o�C�X�̎擾
	auto device = pDR->GetD3DDevice();

	// �V�F�[�_�[�̍쐬
	CreateShader();

	// �摜�̓ǂݍ��݁i�ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/PlayerPoint.dds");

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	// �R�����X�e�[�g�̍쐬
	m_common = std::make_unique<CommonStates>(device);

	// ���W�̏�����
	m_defaultPos = SimpleMath::Vector3{ 0.0f,0.0f,0.0f };
}

/// <summary>
/// �摜�ǂݍ���
/// </summary>
/// <param name="path">�摜�p�X</param>
/// <returns>�Ȃ�</returns>
void PlayerBill::LoadTexture(const wchar_t* path)
{
	// �f�o�C�X
	auto device = m_pDR->GetD3DDevice();
	// �V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex;

	// �t�@�C���ǂݍ���
	CreateDDSTextureFromFile(
		device,
		path,
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);
}

/// <summary>
/// �V�F�[�_�[�̍쐬
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayerBill::CreateShader()
{
	// �f�o�C�X
	auto device = m_pDR->GetD3DDevice();

	//-------------------------------------------------------------------------------------//
	// �V�F�[�_�[�t�@�C���̓ǂݍ���

	// �o�[�e�b�N�X�V�F�[�_�[
	std::vector<uint8_t> VSData = DX::ReadData(L"Resources/Shaders/VS_PlayerPoint.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(VSData.data(), VSData.size(), nullptr,
			m_verShader.ReleaseAndGetAddressOf())
	);
	// �W�I���g���V�F�[�_�[
	std::vector<uint8_t> GSData = DX::ReadData(L"Resources/Shaders/GS_PlayerPoint.cso");
	DX::ThrowIfFailed(
		device->CreateGeometryShader(GSData.data(), GSData.size(), nullptr,
			m_geoShader.ReleaseAndGetAddressOf())
	);
	// �s�N�Z���V�F�[�_�[
	std::vector<uint8_t> PSData = DX::ReadData(L"Resources/Shaders/PS_PlayerPoint.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(PSData.data(), PSData.size(), nullptr,
			m_pixShader.ReleaseAndGetAddressOf())
	);

	//-------------------------------------------------------------------------------------//
	// �V�F�[�_�[�̍쐬

	// �C���v�b�g���C�A�E�g
	DX::ThrowIfFailed(
		device->CreateInputLayout(
			&INPUT_LAYOUT[0],
			static_cast<UINT>(INPUT_LAYOUT.size()),
			VSData.data(),
			VSData.size(),
			m_inputLayout.GetAddressOf())
	);

	// �o�[�e�b�N�X�V�F�[�_�[
	DX::ThrowIfFailed(
		device->CreateVertexShader(
			VSData.data(),
			VSData.size(),
			nullptr,
			m_verShader.ReleaseAndGetAddressOf()
		)
	);

	// �W�I���g���V�F�[�_�[
	DX::ThrowIfFailed(
		device->CreateGeometryShader(
			GSData.data(),
			GSData.size(),
			nullptr,
			m_geoShader.ReleaseAndGetAddressOf()
		)
	);

	// �s�N�Z���V�F�[�_�[
	DX::ThrowIfFailed(
		device->CreatePixelShader(
			PSData.data(),
			PSData.size(),
			nullptr,
			m_pixShader.ReleaseAndGetAddressOf()
		)
	);

	// �R���X�^���g�o�b�t�@�쐬
	CreateConstBuffer(device);
}

/// <summary>
/// �R���X�^���g�o�b�t�@�쐬
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayerBill::CreateConstBuffer(ID3D11Device1*& device)
{
	// �R���X�^���g�o�b�t�@��`
	D3D11_BUFFER_DESC buffer = {};

	// ���g����ɂ���
	ZeroMemory(&buffer, sizeof(buffer));

	// �ǂݏ����̃��[�h���f�t�H���g�ɂ���
	buffer.Usage = D3D11_USAGE_DEFAULT;

	// �V�F�[�_�[�Ŏg���f�[�^�\���̂̃T�C�Y���i�[
	buffer.ByteWidth = sizeof(ConstBuffer);

	// �o�b�t�@�[��萔�o�b�t�@�[�Ƃ��ĕR�Â���
	buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	buffer.CPUAccessFlags = NULL;

	// �쐬�����o�b�t�@���i�[
	device->CreateBuffer(&buffer, nullptr, &m_constBuffer);
}

/// <summary>
/// �r���{�[�h�쐬�֐�
/// </summary>
/// <param name="target">�J�����^�[�Q�b�g�i�����_�j</param>
/// <param name="eye">�J�����A�C�i�J�������W�j</param>
/// <param name="up">������x�N�g���i��{��Y�݂̂P�̃x�N�g���j</param>
/// <returns>�Ȃ�</returns>
void PlayerBill::CreateBillboard(SimpleMath::Vector3 target, SimpleMath::Vector3 eye, SimpleMath::Vector3 up)
{
	m_world = SimpleMath::Matrix::CreateBillboard(SimpleMath::Vector3::Zero, eye - target, up);

	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;

	// �r���{�[�h�̃e�N�X�`������]
	m_world = rot * m_world;
}


/// <summary>
/// �`��
/// </summary>
/// <param name="playerPos">�v���C���̍��W</param>
/// <param name="timer">�^�C�}�[Sinf�Ŏg��</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <returns>�Ȃ�</returns>
void PlayerBill::Render(DirectX::SimpleMath::Vector3 playerPos, float timer, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_pDR->GetD3DDeviceContext();

	// ���_���
	VertexPositionColorTexture vertex = VertexPositionColorTexture(
		m_defaultPos,					// ���W
		SimpleMath::Vector4::One,		// �F���
		SimpleMath::Vector2::Zero		// �e�N�X�`�����W
	);

	// �r���{�[�h�ʒu���X�V
	vertex.position = SimpleMath::Vector3(playerPos.x, playerPos.y + (2.0f + sinf(timer) * 0.2f), playerPos.z);

	// ���_���(�|���S���̂S���_�̍��W���j
	SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			// �J�������ʂ̋����Ń\�[�g
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		}
	);

	for (auto& li : m_particleUtility)
	{
		// ���_���̏�����
		VertexPositionColorTexture vPCT;								// �錾
		vPCT.position = XMFLOAT3(li.GetPosition());						// ���W
		vPCT.color = XMFLOAT4(li.GetNowColor());						// �F
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);	// �摜

		m_vertice = vPCT;
	}

	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_world.Transpose();
	cbuff.Diffuse = SimpleMath::Vector4::One;

	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_constBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_constBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// �摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_common->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// �������`��w��
	ID3D11BlendState* blendstate = m_common->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �u���b�N�̉��ł��r���{�[�h���ђʂ�����
	context->OMSetDepthStencilState(m_common->DepthNone(), 0);

	// �J�����O�͍�����
	context->RSSetState(m_common->CullNone());

	// �V�F�[�_���Z�b�g����
	context->VSSetShader(m_verShader.Get(), nullptr, 0);
	context->GSSetShader(m_geoShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex, 1);
	m_batch->End();

	//�V�F�[�_�̉��
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}