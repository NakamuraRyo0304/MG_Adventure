/*
 *	@File	Transition.h
 *	@Brief	��ʐ؂�ւ��̃g�����W�V�����B
 *	@Date	2023-10-28
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TRANSITION
#define TRANSITION

#include <vector>

class Transition
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	Time;
	};
private:
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;

	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �V�F�[�_�[�̍쐬------------------------------------------------------
	// �o�[�e�b�N�X�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_verShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geoShader;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constBuffer;

public:
	// �C���v�b�g���C�A�E�g�̐ݒ�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	Transition();
	~Transition();

	/// <summary>
	/// ���\�[�X�̍쐬
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Create();

	/// <summary>
	/// �`��֐�
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <returns>�Ȃ�</returns>
	void Render(const float& timer);

	/// <summary>
	/// �e�N�X�`����ύX(�P��̂ݏ���������)
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CycleTextures();

private:

	/// <summary>
	/// �摜�ǂݍ���
	/// </summary>
	/// <param name="path">�摜�p�X</param>
	/// <returns>�Ȃ�</returns>
	void LoadTexture(const wchar_t* path);

	/// <summary>
	/// �V�F�[�_�[�̍쐬
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CreateShader();

	/// <summary>
	/// �R���X�^���g�o�b�t�@�쐬
	/// </summary>
	/// <param name="device">�f�o�C�X�|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void CreateConstBuffer(ID3D11Device1*& device);
};

#endif // TRANSITION