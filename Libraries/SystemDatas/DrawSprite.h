/*
 *	@File	DrawSprite.h
 *	@Brief	画像を描画するクラス。
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef DRAWSPRITE
#define DRAWSPRITE

// デフォルト引数設定用の継承
struct RECT_U : public RECT
{
	RECT_U(LONG left = 0, LONG top = 0, LONG right = 1920, LONG bottom = 1080)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
};

class DrawSprite
{
private:
	// 画像の格納配列
	std::map<const wchar_t*, const wchar_t*> m_textures;

	// スプライトバッチへのポインタ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// 画像を描画する変数
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_SRV;

	// 回転率
	std::map <const wchar_t*,float> m_rotate;

public:
	DrawSprite();
	~DrawSprite();

	/// <summary>
	/// スプライトバッチの作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void MakeSpriteBatch();

	/// <summary>
	/// 画像を登録する関数
	/// </summary>
	/// <param name="key">登録キー(これを指定して呼び出す)</param>
	/// <param name="path">画像のパス(L"Resources/Textures/....dds)拡張子は「.dds」</param>
	/// <returns>なし</returns>
	void AddTextureData(const wchar_t* key, const wchar_t* path);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="key">キー</param>
	/// <param name="pos">表示座標</param>
	/// <param name="color">色</param>
	/// <param name="rate">拡大率</param>
	/// <param name="origin">中心位置</param>
	/// <param name="rect">切り取り位置</param>
	/// <returns>なし</returns>
	void DrawTexture(const wchar_t* key, DirectX::SimpleMath::Vector2 pos,
						DirectX::SimpleMath::Vector4 color, DirectX::SimpleMath::Vector2 rate,
						DirectX::SimpleMath::Vector2 origin, RECT_U rect = RECT_U());


	/// <summary>
	/// 回転量セッター
	/// </summary>
	/// <param name="key">登録キー</param>
	/// <param name="rotate">回転量</param>
	/// <returns>なし</returns>
	void CreateRotation(const wchar_t* key, const float& rotate);


	/// <summary>
	/// 回転量ゲッター
	/// </summary>
	/// <param name="key">登録キー</param>
	/// <returns>回転量</returns>
	const float GetRotate(const wchar_t* key) { auto it = m_rotate.find(key); return it->second; }
};

#endif // DRAWSPRITE