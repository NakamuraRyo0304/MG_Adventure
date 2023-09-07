/*
 *	@File	MouseCursor.cpp
 *	@Brief	マウスカーソルの変更。
 *	@Date	2023-08-25
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "MouseCursor.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="context">コンテキストのポインタ</param>
 /// <returns>なし</returns>
MouseCursor::MouseCursor(ID3D11DeviceContext1* context)
{
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
MouseCursor::~MouseCursor()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="filename">マウスポインタの画像パス</param>
/// <param name="device">デバイスのポインタ</param>
/// <returns>なし</returns>
void MouseCursor::Initialize(const wchar_t* filename, ID3D11Device* device)
{
	// 画像の登録
	CreateDDSTextureFromFile(
		device,
		filename,
		nullptr,
		m_SRV.ReleaseAndGetAddressOf()
	);
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void MouseCursor::Render()
{
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(
		m_SRV.Get(),
		m_cursorPos,				// 表示する位置
		nullptr,					// 切り取り位置
		Colors::White,				// 描画色
		0.0f,						// 回転
		SimpleMath::Vector2{128.0f},// 画像の原点
		0.5f,						// 拡大率
		SpriteEffects_None, 0.0f	// 描画レイヤー
	);

	m_spriteBatch->End();
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void MouseCursor::Finalize()
{
	m_SRV.Reset();
}
