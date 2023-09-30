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
DrawString::DrawString()
	: m_fontColor{Colors::Black}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
DrawString::~DrawString()
{
}

/// <summary>
/// 初期設定
/// </summary>
/// <param name="device">ID3D11Device1ポインタ</param>
/// <param name="context">ID3D11DeviceContext1ポインタ</param>
/// <returns>なし</returns>
void DrawString::CreateString(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// デバッグ表示
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	m_spriteFont = std::make_unique<SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");
}

/// <summary>
/// 文字の描画
/// </summary>
/// <param name="common">コモンステート</param>
/// <param name="pos">描画したい座標</param>
/// <param name="str">描画したい文字列</param>
/// <returns>なし</returns>
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
/// 文字色変更
/// </summary>
/// <param name="font_color">変更したい色</param>
/// <returns>なし</returns>
void DrawString::ChangeFontColor(XMVECTORF32 font_color)
{
	m_fontColor = font_color;
}
