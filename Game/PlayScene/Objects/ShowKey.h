/*
 *	@File	ShowKey.h
 *	@Brief	キーボタンを表示。
 *	@Date	2023-01-16
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SHOWKEY
#define SHOWKEY

class DrawSprite;
class ShowKey
{
private:

	// 画像
	std::unique_ptr<DrawSprite> m_show;

	// フラグ
	std::map<const wchar_t*, bool> is_show;

public:

	ShowKey();
	~ShowKey();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="scale">画面比率</param>
	/// <returns>なし</returns>
	void Draw(const DirectX::SimpleMath::Vector2 scale);
};

#endif // SHOWKEY