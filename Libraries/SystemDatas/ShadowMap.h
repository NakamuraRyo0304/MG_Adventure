/*
 *	@File	ShadowMap.h
 *	@Brief	�V���h�E�}�b�v�̓K�p�N���X
 *	@Date	2023-12-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SHADOWMAP
#define SHADOWMAP

//==============================================================================
// �e�Ŏg�p����w�b�_�[
//==============================================================================
#include "RenderTexture.h"
#include "Libraries/SystemDatas/DepthStencil.h"

using SLAMBDA = std::function<void __cdecl()>;

class ShadowMap
{
public:

	//==============================================================================
	// �e�̃T�C�Y�Ɗ����̋���
	//==============================================================================
	static const int SIZE;									// �V���h�E�}�b�v�̃T�C�Y
	static const float AMBIENT_INTENSITY;					// �V���h�E�}�b�v�̊����̋���(0�قǋ���)

private:

	//==============================================================================
	// �R���X�^���g�o�b�t�@�̒�`
	//==============================================================================
	struct NormalBuffer
	{
		DirectX::XMMATRIX ViewProj;							// ���C�g�̓��e��Ԃ֍��W�ϊ�����s��
		DirectX::XMVECTOR Position;							// ���C�g�̈ʒu
		DirectX::XMVECTOR Ambient;							// ���C�g�̊���
	};
	//==============================================================================
	// ���_�V�F�[�_�[�̒�`
	//==============================================================================
	struct Vertex
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> Depth;	// �f�v�X
		Microsoft::WRL::ComPtr<ID3D11VertexShader> Normal;	// �m�[�}��
	};
	//==============================================================================
	// �s�N�Z���V�F�[�_�[�̒�`
	//==============================================================================
	struct Pixel
	{
		Microsoft::WRL::ComPtr<ID3D11PixelShader> Depth;	// �f�v�X
		Microsoft::WRL::ComPtr<ID3D11PixelShader> Normal;	// �m�[�}��
	};
	//==============================================================================
	// ���C�e�B���O�̒�`
	//==============================================================================
	struct Light
	{
		DirectX::SimpleMath::Vector3 Position;				// �ʒu
		DirectX::SimpleMath::Quaternion Rotate;				// ��]
		DirectX::SimpleMath::Matrix Matrix;					// �s��
		DirectX::SimpleMath::Vector3 Direction;				// ����
	};
	//==============================================================================
	// �f�v�X�X�e���V���̃��[�h���`
	//==============================================================================
	const int USER_DEPTH_MODE = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;

	//==============================================================================
	// �`��Ώۂ̕ύX�Ɏg���ϐ�
	//==============================================================================
	std::unique_ptr<DX::RenderTexture> m_renderTexture;		// �����_�[�e�N�X�`��
	std::unique_ptr<DepthStencil> m_depthStencil;			// �f�v�X�X�e���V��

	//==============================================================================
	// �V�F�[�_�[�̐ݒ���󂯓n���ϐ�
	//==============================================================================
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_normalBuffer;	// ���C�g�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_spotBuffer;		// �X�|�b�g���C�g�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;	// �T���v���[

	//==============================================================================
	// ���C�e�B���O�ƃV�F�[�_�[
	//==============================================================================
	Light m_light;											// ���C�g�̃p�����[�^
	Vertex m_vertex;										// ���_�V�F�[�_�[
	Pixel m_pixel;											// �s�N�Z���V�F�[�_�[
	float m_lightRadius;									// ���C�g�̔��a

	//==============================================================================
	// �����_��
	//==============================================================================
	SLAMBDA m_shadowLam;									// �e�{�̂̃����_��
	SLAMBDA m_modelLam;										// �`��惂�f���̃����_��

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="device">�f�o�C�X�|�C���^</param>
	/// <returns>�Ȃ�</returns>
	ShadowMap(ID3D11Device1* device);
	~ShadowMap();

	/// <summary>
	/// �e�ϊ��̑O����
	/// </summary>
	/// <param name="context">�R���e�L�X�g</param>
	/// <param name="proj">�ˉe�s��</param>
	/// <returns>�Ȃ�</returns>
	void Begin(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// �e�ϊ��̌㏈��
	/// </summary>
	/// <param name="context">�R���e�L�X�g</param>
	/// <returns>�Ȃ�</returns>
	void End(ID3D11DeviceContext1* context);

	/// <summary>
	/// �e�ϊ��̉��
	/// </summary>
	/// <param name="context">�R���e�L�X�g</param>
	/// <returns>�Ȃ�</returns>
	void Exit(ID3D11DeviceContext1* context);


private:

	/// <summary>
	/// �e�ϊ����J�n
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Path1StartTransport();

	/// <summary>
	/// ���C�g�̍s����v�Z
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Path2CalcViewMatrix();

	/// <summary>
	/// �o�b�t�@���X�V
	/// </summary>
	/// <param name="context">�R���e�L�X�g�|�C���^</param>
	/// <param name="proj">�ˉe�s��</param>
	/// <returns>�Ȃ�</returns>
	void Path3UpdateBuffer(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// �V���h�E�}�b�v�̃����_���쐬
	/// </summary>
	/// <param name="context">�R���e�L�X�g�|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void Path4CreateShadowOption(ID3D11DeviceContext1* context);

	/// <summary>
	/// �e�ϊ����I��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Path5EndTransport();

	/// <summary>
	/// ���\�[�X���J��
	/// </summary>
	/// <param name="context">�R���e�L�X�g�|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void Path6ReleaseResources(ID3D11DeviceContext1* context);

public:
	// ���C�g�̈ʒu���擾
	DirectX::SimpleMath::Vector3 const GetLightPosition() { return m_light.Position; }
	// ���C�g�̈ʒu��ݒ�
	void SetLightPosition(const DirectX::SimpleMath::Vector3& pos) { m_light.Position = pos; }
	// ���C�g�̕������擾
	DirectX::SimpleMath::Vector3 const GetLightDirection() { return m_light.Direction; }
	// ���C�g�̕�����ݒ�
	void SetLightDirection(const DirectX::SimpleMath::Vector3& dir) { m_light.Direction = dir; }
	// ���C�g�̉�]���擾
	const DirectX::SimpleMath::Quaternion& GetLightRotate() { return m_light.Rotate; }
	// ���C�g�̉�]��ݒ�
	void SetLightRotate(const DirectX::SimpleMath::Vector3& rotate)
	{ m_light.Rotate = SimpleMath::Quaternion::CreateFromYawPitchRoll(rotate); }
	// �����_�[�e�N�X�`�����擾
	DX::RenderTexture* const GetRenderTexture() { return m_renderTexture.get(); }
	// �m�[�}�����C�g�o�b�t�@���擾
	Microsoft::WRL::ComPtr<ID3D11Buffer>& GetNormalBuffer() { return m_normalBuffer; }
	// �e�{�̂̃����_�����擾
	SLAMBDA GetShadowOption() const { return m_shadowLam; }
	// �`��惂�f���̃����_����ݒ�
	void SetModelOption(const SLAMBDA& lam) { m_modelLam = lam; }
	// �e�̕`���̃����_�����擾
	SLAMBDA GetModelOption() const { return m_modelLam; }
	// ���_�V�F�[�_�[���擾
	Vertex& GetVS() { return m_vertex; }
	// �s�N�Z���V�F�[�_�[���擾
	Pixel& GetPS() { return m_pixel; }
	// �T���v���[���擾
	Microsoft::WRL::ComPtr<ID3D11SamplerState>& GetSampler() { return m_sampler; }
	// ���C�g�̍s����擾
	DirectX::SimpleMath::Matrix& GetViewMatrix() { return m_light.Matrix; }

private:

	// �����_�[�e�N�X�`����ݒ�
	void SetRenderTexture(std::unique_ptr<DX::RenderTexture>& rt) { m_renderTexture = std::move(rt); }
	// �f�v�X�X�e���V�����擾
	DepthStencil* const GetDepthStencil() { return m_depthStencil.get(); }
	// �f�v�X�X�e���V����ݒ�
	void SetDepthStencil(std::unique_ptr<DepthStencil>& dp) { m_depthStencil = std::move(dp); }
	// �e�{�̂̃����_����ݒ�
	void SetShadowLambda(const SLAMBDA& lam) { m_shadowLam = lam; }
	// ���C�g�̍s���ݒ�
	void SetViewMatrix(DirectX::SimpleMath::Matrix& matrix) { m_light.Matrix = matrix; }
};

#endif // SHADOWMAP