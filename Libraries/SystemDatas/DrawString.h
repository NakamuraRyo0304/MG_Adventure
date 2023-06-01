/*
 *	@File	DrawString.h
 *	@Brief	デバッグ文字描画。
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
	// スプライトの定義
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>  m_spriteFont;
	DirectX::XMVECTORF32 m_fontColor;
public:
	DrawString();

	// フォントの初期化
	void CreateString(ID3D11Device1* device, ID3D11DeviceContext1* context);

	// 描画する関数　第１：コモンステート、第２：位置、第３：文字、(第４：数字)
	void DrawFormatString(DirectX::CommonStates* common,
						  DirectX::SimpleMath::Vector2 pos,
		                  const wchar_t* str) noexcept(false);

	// 文字色を変更
	void ChangeFontColor(DirectX::XMVECTORF32 font_color);

	std::unique_ptr<DirectX::SpriteBatch>& GetSpriteBatch()
	{
		return m_spriteBatch;
	}
};

#endif // DRAWSTRING