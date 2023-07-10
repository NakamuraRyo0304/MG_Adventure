/*
 *	@File	PlayerBill.h
 *	@Brief	�v���C���[�̈ʒu����`�悷��N���X�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYERBILL
#define PLAYERBILL

#include <vector>
#include <list>

class ParticleUtility;
class PlayerBill
{
public:
	// �R���X�^���g�o�b�t�@(�V�F�[�_�[�Ŏg���\����)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix  matWorld;
		DirectX::SimpleMath::Matrix  matView;
		DirectX::SimpleMath::Matrix  matProj;
		DirectX::SimpleMath::Vector4 Diffuse;
	};
private:
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_common;

	// ���_
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;

	// �e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �V�F�[�_�[�̍쐬------------------------------------------------------
	// �o�[�e�b�N�X�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constBuffer;

	// �V�F�[�_�[�̕\�����W
	DirectX::SimpleMath::Vector3 m_defaultPos;

	// Utility
	std::list<ParticleUtility> m_particleUtility;
	
	// �r���{�[�h
	DirectX::SimpleMath::Matrix m_billboard;

	// �r���{�[�h�p�J����
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;

public:
	// �C���v�b�g���C�A�E�g�̐ݒ�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	PlayerBill();
	~PlayerBill();

	// �摜�ǂݍ���
	void LoadTexture(const wchar_t* path);

	// ���\�[�X�̍쐬
	void Create(DX::DeviceResources* pDR);

	// �`��
	void Render(DirectX::SimpleMath::Vector3 playerPos, float timer, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// �r���{�[�h�쐬
	void CreateBillboard(DirectX::SimpleMath::Vector3 target,DirectX::SimpleMath::Vector3 eye,DirectX::SimpleMath::Vector3 up);

	// �\�����W�ݒ�
	void SetVertexMovePos(DirectX::SimpleMath::Vector3 movePos) { m_defaultPos = movePos; }

private:

	// �V�F�[�_�[�̍쐬
	void CreateShader();

	// �R���X�^���g�o�b�t�@�̍쐬
	void CreateConstBuffer(ID3D11Device1*& device);
};

#endif // PLAYERBILL