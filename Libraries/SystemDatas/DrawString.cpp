/*
 *	@File	DrawString.cpp
 *	@Brief	デバッグ文字描画。
 *	@Date	2023-04-27
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "CommonStates.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

#include "DrawString.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
Debug::DrawString::DrawString(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// フォントパス
	const wchar_t* fontFilePath = L"Resources/Fonts/SegoeUI_18.spritefont";

	// デバッグ表示
	m_spriteFont = std::make_unique<SpriteFont>(device, fontFilePath);
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
Debug::DrawString::~DrawString()
{
	m_spriteBatch.reset();
	m_spriteFont.reset();
}