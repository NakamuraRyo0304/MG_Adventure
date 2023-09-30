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

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
DrawString::DrawString()
	: m_fontColor{Colors::Black}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
DrawString::~DrawString()
{
}

/// <summary>
/// �����ݒ�
/// </summary>
/// <param name="device">ID3D11Device1�|�C���^</param>
/// <param name="context">ID3D11DeviceContext1�|�C���^</param>
/// <returns>�Ȃ�</returns>
void DrawString::CreateString(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// �f�o�b�O�\��
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	m_spriteFont = std::make_unique<SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");
}

/// <summary>
/// �����̕`��
/// </summary>
/// <param name="common">�R�����X�e�[�g</param>
/// <param name="pos">�`�悵�������W</param>
/// <param name="str">�`�悵����������</param>
/// <returns>�Ȃ�</returns>
void DrawString::DrawFormatString(CommonStates* common,
								  SimpleMath::Vector2 pos, const wchar_t* str)
	                              noexcept(false)
{
	m_spriteBatch->Begin(SpriteSortMode_Deferred,
		common->NonPremultiplied());

	m_spriteFont->DrawString(m_spriteBatch.get(), str, pos, m_fontColor);
	m_spriteBatch->End();
}

/// <summary>
/// �����F�ύX
/// </summary>
/// <param name="font_color">�ύX�������F</param>
/// <returns>�Ȃ�</returns>
void DrawString::ChangeFontColor(XMVECTORF32 font_color)
{
	m_fontColor = font_color;
}
