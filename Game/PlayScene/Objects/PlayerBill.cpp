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
	auto _device = pDR->GetD3DDevice();

	// �V�F�[�_�[�̍쐬
	CreateShader();

	// �摜�̓ǂݍ��݁i�ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/PLAY_COMMON/PlayerPoint.dds");

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	// �R�����X�e�[�g�̍쐬
	m_states = std::make_unique<CommonStates>(_device);

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
	auto _device = m_pDR->GetD3DDevice();
	// �V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _tex;

	// �t�@�C���ǂݍ���
	CreateDDSTextureFromFile(
		_device,
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
	auto _device = m_pDR->GetD3DDevice();

	//-------------------------------------------------------------------------------------//
	// �V�F�[�_�[�t�@�C���̓ǂݍ���

	// �o�[�e�b�N�X�V�F�[�_�[
	std::vector<uint8_t> _VSData = DX::ReadData(L"Resources/Shaders/VS_PlayerPoint.cso");
	DX::ThrowIfFailed(
		_device->CreateVertexShader(_VSData.data(), _VSData.size(), nullptr,
			m_verShader.ReleaseAndGetAddressOf())
	);
	// �W�I���g���V�F�[�_�[
	std::vector<uint8_t> _GSData = DX::ReadData(L"Resources/Shaders/GS_PlayerPoint.cso");
	DX::ThrowIfFailed(
		_device->CreateGeometryShader(_GSData.data(), _GSData.size(), nullptr,
			m_geoShader.ReleaseAndGetAddressOf())
	);
	// �s�N�Z���V�F�[�_�[
	std::vector<uint8_t> _PSData = DX::ReadData(L"Resources/Shaders/PS_PlayerPoint.cso");
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
void PlayerBill::CreateConstBuffer(ID3D11Device1*& device)
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
/// �r���{�[�h�쐬�֐�
/// </summary>
/// <param name="target">�J�����^�[�Q�b�g�i�����_�j</param>
/// <param name="eye">�J�����A�C�i�J�������W�j</param>
/// <param name="up">������x�N�g���i��{��Y�݂̂P�̃x�N�g���j</param>
/// <returns>�Ȃ�</returns>
void PlayerBill::CreateBillboard(SimpleMath::Vector3 target, SimpleMath::Vector3 eye, SimpleMath::Vector3 up)
{
	m_world = SimpleMath::Matrix::CreateBillboard(SimpleMath::Vector3::Zero, eye - target, up);

	// ��]���~�߂ď�ɐ��ʂɌ�����
	SimpleMath::Matrix _rot = SimpleMath::Matrix::Identity;
	_rot._11 = -1;
	_rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;

	// �r���{�[�h�̃e�N�X�`������]
	m_world = _rot * m_world;
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
	auto _context = m_pDR->GetD3DDeviceContext();

	// ���_���
	VertexPositionColorTexture _vertex = VertexPositionColorTexture(
		m_defaultPos,					// ���W
		SimpleMath::Vector4::One,		// �F���
		SimpleMath::Vector2::Zero		// �e�N�X�`�����W
	);

	// �r���{�[�h�ʒu���X�V
	_vertex.position = SimpleMath::Vector3(playerPos.x, playerPos.y + (2.0f + sinf(timer) * 0.2f), playerPos.z);

	// ���_���(�|���S���̂S���_�̍��W���j
	SimpleMath::Vector3 _cameraDir = m_cameraTarget - m_cameraPosition;
	_cameraDir.Normalize();

	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			// �J�������ʂ̋����Ń\�[�g
			return _cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > _cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		}
	);

	for (auto& i : m_particleUtility)
	{
		// ���_���̏�����
		VertexPositionColorTexture _vPCT = {};							// �錾
		_vPCT.position = XMFLOAT3(i.GetPosition());						// ���W
		_vPCT.color = XMFLOAT4(i.GetNowColor());						// �F
		_vPCT.textureCoordinate = XMFLOAT2(i.GetNowScale().x, 0.0f);	// �摜

		m_vertice = _vPCT;
	}

	// �V�F�[�_�[�ɃR���X�^���g�o�b�t�@��n��
	ConstBuffer _buffer;
	_buffer.matView = view.Transpose();
	_buffer.matProj = proj.Transpose();
	_buffer.matWorld = m_world.Transpose();
	_buffer.Diffuse = SimpleMath::Vector4::One;

	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	_context->UpdateSubresource(m_constBuffer.Get(), 0, NULL, &_buffer, 0, 0);

	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* _cbuffer[1] = { m_constBuffer.Get() };
	_context->VSSetConstantBuffers(0, 1, _cbuffer);
	_context->GSSetConstantBuffers(0, 1, _cbuffer);
	_context->PSSetConstantBuffers(0, 1, _cbuffer);

	// �摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	_context->PSSetSamplers(0, 1, sampler);

	// �������`��w��
	ID3D11BlendState* _blendState = m_states->NonPremultiplied();

	// �������菈��
	_context->OMSetBlendState(_blendState, nullptr, 0xFFFFFFFF);

	// �u���b�N�̉��ł��r���{�[�h���ђʂ�����
	_context->OMSetDepthStencilState(m_states->DepthNone(), 0);

	// �J�����O�͍�����
	_context->RSSetState(m_states->CullNone());

	// �V�F�[�_���Z�b�g����
	_context->VSSetShader(m_verShader.Get(), nullptr, 0);
	_context->GSSetShader(m_geoShader.Get(), nullptr, 0);
	_context->PSSetShader(m_pixShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����
	_context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//�C���v�b�g���C�A�E�g�̓o�^
	_context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &_vertex, 1);
	m_batch->End();

	//�V�F�[�_�̉��
	_context->VSSetShader(nullptr, nullptr, 0);
	_context->GSSetShader(nullptr, nullptr, 0);
	_context->PSSetShader(nullptr, nullptr, 0);
}