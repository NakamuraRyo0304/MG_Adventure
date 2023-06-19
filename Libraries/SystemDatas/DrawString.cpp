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

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
DrawString::DrawString():
	m_fontColor{Colors::Black}
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
DrawString::~DrawString()
{
}

//--------------------------------------------------------//
//初期化                                                  //
//--------------------------------------------------------//
void DrawString::CreateString(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// デバッグ表示
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	m_spriteFont = std::make_unique<SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");
}

//--------------------------------------------------------//
//文字の描画関数                                          //
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
//文字の色を変更                                          //
//--------------------------------------------------------//
void DrawString::ChangeFontColor(XMVECTORF32 font_color)
{
	m_fontColor = font_color;
}
