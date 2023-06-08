/*
 *	@File	DrawSprite.h
 *	@Brief	�摜��`�悷��N���X�B
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef DRAWSPRITE
#define DRAWSPRITE

#include <iostream>
#include <map>			// �}�b�v���g�p����

class DrawSprite
{
private:
	// �摜�̊i�[�z��
	std::map<const wchar_t*, const wchar_t*> m_textures;

	// �X�v���C�g�o�b�`�ւ̃|�C���^
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �摜��`�悷��ϐ�
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_SRV;

public:
	DrawSprite();
	~DrawSprite();

	// �X�v���C�g�o�b�`�̏�����
	void MakeSpriteBatch(ID3D11DeviceContext1* context);

	// �摜��o�^����
	void AddTextureData(const wchar_t* key, const wchar_t* path, ID3D11Device* device);

	// �摜��`�悷��
	void DrawTexture(const wchar_t* key, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector4 color,float rate);
};

#endif // DRAWSPRITE