/*
 *	@File	MouseCursor.h
 *	@Brief	マウスカーソルの変更。
 *	@Date	2023-08-25
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MOUSECURSOR
#define MOUSECURSOR

class MouseCursor
{
private:

	// スプライトバッチのポインタ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// テクスチャ画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView > m_SRV;

	// 座標
	DirectX::SimpleMath::Vector2 m_cursorPos;

public:
	MouseCursor();
	~MouseCursor();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="filename">マウスポインタの画像パス</param>
	/// <returns>なし</returns>
	void Initialize(const wchar_t* filename);

	/// <summary>
	/// マウス位置の更新
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Render();

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize();
};

#endif // MOUSECURSOR