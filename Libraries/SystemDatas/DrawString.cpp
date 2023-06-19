/*
 *	@File	DrawString.cpp
 *	@Brief	�f�o�b�O�����`��B
 *	@Date	2023-04-27
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "CommonStates.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

#include "DrawString.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
DrawString::DrawString():
	m_fontColor{Colors::Black}
{
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
DrawString::~DrawString()
{
}

//--------------------------------------------------------//
//������                                                  //
//--------------------------------------------------------//
void DrawString::CreateString(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// �f�o�b�O�\��
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	m_spriteFont = std::make_unique<SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");
}

//--------------------------------------------------------//
//�����̕`��֐�                                          //
//--------------------------------------------------------//
void DrawString::DrawFormatString(CommonStates* common,
								  SimpleMath::Vector2 pos, const wchar_t* str)
	                              noexcept(false)
{
	m_spriteBatch->Begin(SpriteSortMode_Deferred,
		common->NonPremultiplied());

	m_spriteFont->DrawString(m_spriteBatch.get(), str, pos, m_fontColor);
	m_spriteBatch->End();
}

//--------------------------------------------------------//
//�����̐F��ύX                                          //
//--------------------------------------------------------//
void DrawString::ChangeFontColor(XMVECTORF32 font_color)
{
	m_fontColor = font_color;
}
