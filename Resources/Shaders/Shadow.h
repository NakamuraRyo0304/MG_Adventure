/*
 *	@File	Shadow.h
 *	@Brief	�e�̃V�F�[�_�[�B
 *	@Date	2023-10-10
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SHADOW
#define SHADOW

class Shadow
{
private:

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// �s��
	DirectX::SimpleMath::Matrix m_world, m_view, m_proj;

	// �e�p���f��
	std::unique_ptr<Model> m_shadowModel;

public:
	Shadow();
	~Shadow();

	// �����֐�
	void CreateShadow(ID3D11Device1* device);

	// �s����Z�b�g
	void SetMatrix(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
	{
		m_world = world; m_view = view; m_proj = proj;
	}

	// ���f���̃Z�b�^�[
	void SetModel(std::unique_ptr<Model> model) { m_shadowModel = std::move(model); }

	// �`��
	void Draw(ID3D11DeviceContext1* context, DirectX::CommonStates* states);

	// ���f���̃Q�b�^�[
	std::unique_ptr<Model>& GetModel() { return m_shadowModel; }
};


#endif // SHADOW