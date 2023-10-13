/*
 *	@File	Shadow.cpp
 *	@Brief	�e�̃V�F�[�_�[�B
 *	@Date	2023-10-10
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �t�@�C���ǂݍ���
#include "Libraries/ReadData.h"

#include "Shadow.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
Shadow::Shadow()
	: m_pixelShader{}		// �s�N�Z���V�F�[�_�[
	, m_world{}				// ���[���h�s��
	, m_view{}				// �r���[�s��
	, m_proj{}				// �v���W�F�N�V�����s��
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Shadow::~Shadow()
{
	m_pixelShader.Reset();
}

/// <summary>
/// �e�V�F�[�_�[�����֐�
/// </summary>
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void Shadow::CreateShadow(ID3D11Device1* device)
{
	// �s�N�Z���V�F�[�_�[�̍쐬
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);
}

/// <summary>
/// ���f�����Z�b�g����
/// </summary>
/// <param name="model">���f���̃p�X</param>
/// <param name="key">�o�^�L�[</param>
/// <returns>�Ȃ�</returns>
void Shadow::SetModel(const wchar_t* key, std::unique_ptr<Model> model)
{
	m_shadowModel.emplace(key, std::move(model));
}

/// <summary>
/// �`��֐�
/// </summary>
/// <param name="key">�o�^�L�[</param>
/// <param name="context">�R���e�L�X�g�|�C���^</param>
/// <param name="states">�R�����X�e�[�g�|�C���^</param>
/// <returns>�Ȃ�</returns>
void Shadow::Draw(const wchar_t* key, ID3D11DeviceContext1* context, CommonStates* states)
{
	auto vol = [&]()
	{
		// �u�����h�X�e�[�g�̐ݒ�
		context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xffffffff);
		// �J�����O���Ȃ�
		context->RSSetState(states->CullNone());
		// �s�N�Z���V�F�[�_�[�̐ݒ�
		context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	};

	m_world *= SimpleMath::Matrix::CreateShadow(
			   SimpleMath::Vector3(0.f, 1.f, 0.f),
			   SimpleMath::Plane(0.f, 1.f, 0.f, 0.f)
	);

	// �e��`�悷��
 	std::map<const wchar_t*, std::unique_ptr<Model>>::const_iterator it = m_shadowModel.find(key);
	it->second->Draw(context, *states, m_world, m_view, m_proj, &vol);
}
