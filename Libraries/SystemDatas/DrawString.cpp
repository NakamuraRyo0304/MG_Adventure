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
Debug::DrawString::DrawString(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// �t�H���g�p�X
	const wchar_t* fontFilePath = L"Resources/Fonts/SegoeUI_18.spritefont";

	// �f�o�b�O�\��
	m_spriteFont = std::make_unique<SpriteFont>(device, fontFilePath);
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Debug::DrawString::~DrawString()
{
	m_spriteBatch.reset();
	m_spriteFont.reset();
}