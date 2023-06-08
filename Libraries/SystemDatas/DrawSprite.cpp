/*
 *	@File	DrawSprite.cpp
 *	@Brief	�摜��`�悷��N���X�B
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "DrawSprite.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
DrawSprite::DrawSprite():
	m_textures{},
	m_SRV{},
	m_spriteBatch{}
{
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
DrawSprite::~DrawSprite()
{
}

//--------------------------------------------------------//
//�X�v���C�g�o�b�`���쐬                                  //
//--------------------------------------------------------//
void DrawSprite::MakeSpriteBatch(ID3D11DeviceContext1* context)
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
}

//--------------------------------------------------------//
//�摜��o�^����֐�                                      //
//--------------------------------------------------------//
// ��P�����F�o�^�� ��Q�����F�摜�̃p�X ��R�����F�f�o�C�X
void DrawSprite::AddTextureData(const wchar_t* key, const wchar_t* path ,ID3D11Device* device)
{
	// �摜�̒ǉ�
	m_textures.emplace(key, path);

	for (std::map<const wchar_t*, const wchar_t*>::const_iterator it = m_textures.begin(); it != m_textures.end(); ++it)
	{

		// �摜�̓o�^
		DirectX::CreateDDSTextureFromFile(
			device,
			it->second,	// �e�N�X�`���̃p�X
			nullptr,
			m_SRV[it->first].ReleaseAndGetAddressOf() // �Ή�����L�[�ԍ��ɓo�^
		);
	}
}

//--------------------------------------------------------//
//�摜��`�悷��                                          //
//--------------------------------------------------------//
// ��P�����F�L�[ ��Q�����F�\�����W ��R�����F�F ��S�����F�g�嗦
void DrawSprite::DrawTexture(const wchar_t* key, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector4 color, float rate)
{
	m_spriteBatch->Begin();

	std::map<const wchar_t*, const wchar_t*>::const_iterator it = m_textures.find(key);

	m_spriteBatch->Draw(
		m_SRV[it->first].Get(),
		pos,
		nullptr,
		color,
		0.0f,
		DirectX::SimpleMath::Vector2(0.0f, 0.0f),
		rate,
		DirectX::SpriteEffects_None, 0.0f
	);

	m_spriteBatch->End();
}