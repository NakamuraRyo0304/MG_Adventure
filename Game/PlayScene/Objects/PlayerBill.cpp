/*
 *	@File	PlayerBill.cpp
 *	@Brief	�v���C���[�̈ʒu����`�悷��N���X�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Libraries/SystemDatas/ParticleUtility.h"
#include "Libraries/FactoryManager/FactoryManager.h"

#include "PlayerBill.h"

// �C���v�b�g���C�A�E�g�̐ݒ�
const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerBill::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// �R���X�g���N�^
PlayerBill::PlayerBill(std::shared_ptr<FactoryManager>factory)
	: m_defaultPos{SimpleMath::Vector3::Zero}
	, m_vertice{}
	, m_factory{factory}
{
	m_factory->CreateFactory();
}

// �f�X�g���N�^
PlayerBill::~PlayerBill()
{
}

// �쐬�֐�
void PlayerBill::Create()
{
	auto _pDR = DX::DeviceResources::GetInstance();

	// �V�F�[�_�[�̍쐬
	CreateShader();

	// �摜�̓ǂݍ��݁i�ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/PLAY_COMMON/PlayerPoint.dds");

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(_pDR->GetD3DDeviceContext());

	// �R�����X�e�[�g�̍쐬
	m_states = std::make_unique<CommonStates>(_pDR->GetD3DDevice());

	// ���W�̏�����
	m_defaultPos = SimpleMath::Vector3{ 0.0f,0.0f,0.0f };
}

// �摜�ǂݍ���
void PlayerBill::LoadTexture(const wchar_t* path)
{
	auto _pDR = DX::DeviceResources::GetInstance();

	// �V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _tex;

	// �t�@�C���ǂݍ���
	CreateDDSTextureFromFile(
		_pDR->GetD3DDevice(),
		path,
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);
}

// �V�F�[�_�[�쐬
void PlayerBill::CreateShader()
{
	m_factory->BuildShaderFactory();

	// �V�F�[�_�[�t�@�C���̓ǂݍ���
	m_factory->VisitShaderFactory()->CreateVertexShader(L"Resources/Shaders/VS_PlayerPoint.cso", &m_verShader);
	m_factory->VisitShaderFactory()->CreateGeometryShader(L"Resources/Shaders/GS_PlayerPoint.cso", &m_geoShader);
	m_factory->VisitShaderFactory()->CreatePixelShader(L"Resources/Shaders/PS_PlayerPoint.cso", &m_pixShader);
	m_factory->VisitShaderFactory()->CreateInputLayout(&m_inputLayout);

	m_factory->LeaveShaderFactory();

	// �R���X�^���g�o�b�t�@�쐬
	CreateConstBuffer();
}

// �R���X�^���g�o�b�t�@���쐬
void PlayerBill::CreateConstBuffer()
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

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
	_device->CreateBuffer(&_buffer, nullptr, &m_constBuffer);
}

// �r���{�[�h�쐬�֐�
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


// �`��֐�
void PlayerBill::Render(SimpleMath::Vector3 playerPos, float timer, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

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