/*
 *	@File	DrawString.h
 *	@Brief	�f�o�b�O�����`��B
 *	@Date	2023-04-27
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef DRAWSTRING
#define DRAWSTRING

#include "CommonStates.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

class DrawString
{
private:
	// �X�v���C�g�̒�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>  m_spriteFont;
	DirectX::XMVECTORF32 m_fontColor;
public:
	DrawString();

	// �t�H���g�̏�����
	void CreateString(ID3D11Device1* device, ID3D11DeviceContext1* context);

	// �`�悷��֐��@��P�F�R�����X�e�[�g�A��Q�F�ʒu�A��R�F�����A(��S�F����)
	void DrawFormatString(DirectX::CommonStates* common,
						  DirectX::SimpleMath::Vector2 pos,
		                  const wchar_t* str) noexcept(false);

	// �����F��ύX
	void ChangeFontColor(DirectX::XMVECTORF32 font_color);

	std::unique_ptr<DirectX::SpriteBatch>& GetSpriteBatch()
	{
		return m_spriteBatch;
	}
};

#endif // DRAWSTRING