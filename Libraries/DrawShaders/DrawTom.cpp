/*
 *	@File	DrawTom.cpp
 *	@Brief	�g����`�悷��N���X�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

// C++
#include <vector>

#include "../MyLib/LoadFile.h"
#include "DrawTom.h"

using namespace DirectX;

/// <summary>
/// �C���v�b�g���C�A�E�g�̐ݒ�
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> DrawTom::INPUT_LAYOUT =
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
DrawTom::DrawTom():
	m_pDR{nullptr},
	m_world{SimpleMath::Matrix::Identity},
	m_defaultPos{SimpleMath::Vector3::Zero}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
DrawTom::~DrawTom()
{
}

/// <summary>
/// �摜�ǂݍ���
/// </summary>
/// <param name="path">�摜�p�X</param>
/// <returns>�Ȃ�</returns>
void DrawTom::LoadTexture(const wchar_t* path)
{
	// �f�o�C�X
	auto device = m_pDR->GetD3DDevice();
	// �V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex;

	// �t�@�C���ǂݍ���
	DirectX::CreateWICTextureFromFile(
		device,
		path,
		nullptr, 
		tex.ReleaseAndGetAddressOf()
	);

	// �摜��ǉ�
	m_textures.push_back(tex);
}

/// <summary>
/// ���\�[�X�̍쐬
/// </summary>
/// <param name="pDR">�f�o�C�X���\�[�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void DrawTom::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	auto device = pDR->GetD3DDevice();

	//�V�F�[�_�[�̍쐬
	CreateShader();

	//�摜�̓ǂݍ��݁i�ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/PlayerPoint.png");

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	// �R�����X�e�[�g�̍쐬
	m_common = std::make_unique<CommonStates>(device);

	// ���W�̏�����
	m_defaultPos[0] = SimpleMath::Vector3{ 0.0f,0.0f,0.0f };	 // ����
	m_defaultPos[1] = SimpleMath::Vector3{ 0.0f,0.0f,0.0f };	 // �E��
	m_defaultPos[2] = SimpleMath::Vector3{ 0.0f,0.0f,0.0f };	 // ����
	m_defaultPos[3] = SimpleMath::Vector3{ 0.0f,0.0f,0.0f };	 // �E��
}

/// <summary>
/// �`��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <returns>�Ȃ�</returns>
void DrawTom::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_pDR->GetD3DDeviceContext();

	// ���_���(�|���S���̂S���_�̍��W���j
	//VertexPositionColorTexture vertex[4] =
	//{
	//	VertexPositionColorTexture(m_defaultPos[0], SimpleMath::Vector4::One, SimpleMath::Vector2(0.0f, 0.0f)),	// ����
	//	VertexPositionColorTexture(m_defaultPos[1], SimpleMath::Vector4::One, SimpleMath::Vector2(1.0f, 0.0f)),	// �E��
	//	VertexPositionColorTexture(m_defaultPos[2], SimpleMath::Vector4::One, SimpleMath::Vector2(0.0f, 1.0f)),	// ����
	//	VertexPositionColorTexture(m_defaultPos[3], SimpleMath::Vector4::One, SimpleMath::Vector2(1.0f, 1.0f)),	// �E��
	//};
	// ���_���(�|���S���̂P���_�̍��W���j
	VertexPositionColorTexture vertex[1] =
	{
		VertexPositionColorTexture(m_defaultPos[0], SimpleMath::Vector4::One, SimpleMath::Vector2(0.0f, 0.0f)),	// ����
	};


	// ���_���(�|���S���̂S���_�̍��W���j
	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			//�J�������ʂ̋����Ń\�[�g
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});

	// �o�^����Ă��钸�_�����Z�b�g
	m_vertices.clear();

	for (auto& li : m_particleUtility)
	{
		// �J�����̌��ɂ����珈�����Ȃ�
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f)continue;

		VertexPositionColorTexture vPCT;
		vPCT.position = XMFLOAT3(li.GetPosition());
		vPCT.color = XMFLOAT4(li.GetNowColor());
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_vertices.push_back(vPCT);
	}


	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose();
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

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

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_common->DepthDefault(), 0);

	// �J�����O�͍�����
	context->RSSetState(m_common->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(),   nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(),    nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����
	for (int i = 0; i < m_textures.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_textures[i].GetAddressOf());
	}

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//�V�F�[�_�̉��
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

/// <summary>
/// �V�F�[�_�[�̍쐬
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void DrawTom::CreateShader()
{
	// �f�o�C�X
	auto device = m_pDR->GetD3DDevice();

	// �V�F�[�_�[�t�@�C���̓ǂݍ���
	// �o�[�e�b�N�X�V�F�[�_�[
	LoadFile VSData = LoadFile::LoadBinaryFile(L"Resources/Shaders/TomVS.cso");
	// �W�I���g���V�F�[�_�[
	LoadFile GSData = LoadFile::LoadBinaryFile(L"Resources/Shaders/TomGS.cso");
	// �s�N�Z���V�F�[�_�[
	LoadFile PSData = LoadFile::LoadBinaryFile(L"Resources/Shaders/TomPS.cso");

	// �C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(
		&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(),
		VSData.GetSize(),
		m_inputLayout.GetAddressOf()
	);

	// �o�[�e�b�N�X�V�F�[�_�[�̍쐬
	device->CreateVertexShader(
		VSData.GetData(),
		VSData.GetSize(), 
		nullptr,
		m_vertexShader.ReleaseAndGetAddressOf()
	);

	// �W�I���g���V�F�[�_�[�̍쐬
	device->CreateGeometryShader(
		GSData.GetData(),
		GSData.GetSize(),
		nullptr,
		m_geometryShader.ReleaseAndGetAddressOf()
	);

	// �s�N�Z���V�F�[�_�[�̍쐬
	device->CreatePixelShader(
		PSData.GetData(), 
		PSData.GetSize(), 
		nullptr,
		m_pixelShader.ReleaseAndGetAddressOf()
	);

	// �R���X�^���g�o�b�t�@�쐬
	CreateConstBuffer(device);
}

/// <summary>
/// �R���X�^���g�o�b�t�@�쐬
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void DrawTom::CreateConstBuffer(ID3D11Device1*& device)
{
	// �R���X�^���g�o�b�t�@��`
	D3D11_BUFFER_DESC buffer;

	// ���g����ɂ���
	ZeroMemory(&buffer, sizeof(buffer));

	buffer.Usage = D3D11_USAGE_DEFAULT;

	// �V�F�[�_�[�Ŏg���f�[�^�\���̂̃T�C�Y���i�[
	buffer.ByteWidth = sizeof(ConstBuffer);
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
void DrawTom::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
{
	m_billboard = SimpleMath::Matrix::CreateBillboard(SimpleMath::Vector3::Zero, eye - target, up);

	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;

	m_billboard = rot * m_billboard;
}
