/*
 *	@File	Shadow.h
 *	@Brief	�e�̃V�F�[�_�[�B
 *	@Date	2023-10-10
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SHADOW
#define SHADOW

#include <map>

class Shadow
{
private:

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// �s��
	DirectX::SimpleMath::Matrix m_world, m_view, m_proj;

	// ���f��
	std::map <const wchar_t* ,std::unique_ptr<Model>> m_shadowModel;

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
	void SetModel(const wchar_t* key, std::unique_ptr<Model> model);

	// �`��
	void Draw(const wchar_t* key, ID3D11DeviceContext1* context, DirectX::CommonStates* states);
};


#endif // SHADOW