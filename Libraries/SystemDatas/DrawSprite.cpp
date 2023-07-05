/*
 *	@File	DrawSprite.cpp
 *	@Brief	�摜��`�悷��N���X�B
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "DrawSprite.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
DrawSprite::DrawSprite():
	m_textures{},
	m_SRV{},
	m_spriteBatch{}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
DrawSprite::~DrawSprite()
{
	m_textures.clear();
	m_SRV.clear();
}

/// <summary>
/// �X�v���C�g�o�b�`���쐬
/// </summary>
/// <param name="context">ID3D11DeviceContext1�|�C���^</param>
void DrawSprite::MakeSpriteBatch(ID3D11DeviceContext1* context)
{
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
}

/// <summary>
/// �摜��o�^����֐�
/// </summary>
/// <param name="key">�o�^�L�[(������w�肵�ČĂяo��)</param>
/// <param name="path">�摜�̃p�X(L"Resources/Textures/....dds)�g���q�́u.dds�v</param>
/// <param name="device">ID3D11Device�|�C���^</param>
void DrawSprite::AddTextureData(const wchar_t* key, const wchar_t* path ,ID3D11Device* device)
{
	// �摜�̒ǉ�
	m_textures.emplace(key, path);

	for (std::map<const wchar_t*, const wchar_t*>::const_iterator it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		// �摜�̓o�^
		CreateDDSTextureFromFile(
			device,
			it->second,	// �e�N�X�`���̃p�X
			nullptr,
			m_SRV[it->first].ReleaseAndGetAddressOf() // �Ή�����L�[�ԍ��ɓo�^
		);
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="key">�L�[</param>
/// <param name="pos">�\�����W</param>
/// <param name="color">�F</param>
/// <param name="rate">�g�嗦</param>
/// <param name="origin">���S�ʒu</param>
void DrawSprite::DrawTexture(const wchar_t* key, SimpleMath::Vector2 pos, 
	SimpleMath::Vector4 color, float rate, SimpleMath::Vector2 origin)
{
	m_spriteBatch->Begin();

	// �`�悵�����L�[�ԍ��ɑΉ�����}�b�v���C�e���[�^�Ɋi�[
	std::map<const wchar_t*, const wchar_t*>::const_iterator it = m_textures.find(key);

	// �摜�̕`��
	m_spriteBatch->Draw(
		m_SRV[it->first].Get(),					// �Ή�����C�e���[�^�̉摜��`��
		pos,									// �\������ʒu
		nullptr,								// �؂���ʒu
		color,									// �`��F
		0.0f,									// ��]
		origin,									// �摜�̌��_
		rate,									// �g�嗦
		SpriteEffects_None, 0.0f				// �`�惌�C���[
	);

	m_spriteBatch->End();
}