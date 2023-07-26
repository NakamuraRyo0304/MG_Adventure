/*
 *	@File	PlayerShadow.h
 *	@Brief	�v���C���[�̉e�B
 *	@Date	2023-07-24
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYERSHADOW
#define PLAYERSHADOW

class PlayerShadow
{
public:
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// �R���X�^���g�o�b�t�@
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

private:
	DX::DeviceResources* m_deviceResources;

	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>					m_constBuffer;

	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>					m_states;

	// �s��
	DirectX::SimpleMath::Matrix								m_world;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>				m_inputLayout;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	// �摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//-------------------------------------------------------------------------------------//

	// ���_���W
	DirectX::SimpleMath::Vector3 m_position;

	//-------------------------------------------------------------------------------------//

	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_verShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geoShader;

	//-------------------------------------------------------------------------------------//
public:
	// �C���v�b�g���C�A�E�g�̐ݒ�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	PlayerShadow(DX::DeviceResources* pDR);
	~PlayerShadow();


	// �쐬����
	void Create();

	// �X�V����
	void Update(const DirectX::SimpleMath::Vector3& position);

	// �`�揈��
	void Render(ID3D11DeviceContext* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

private:

	// �V�F�[�_�[�̍쐬
	void CreateShader();
};

#endif // PLAYERSHADOW