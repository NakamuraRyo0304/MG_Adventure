/*
 *	@File	DrawSprite.h
 *	@Brief	画像を描画するクラス。
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef DRAWSPRITE
#define DRAWSPRITE

#include <map>

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

	// スプライトバッチの初期化
	void MakeSpriteBatch(ID3D11DeviceContext1* context);

	// 画像を登録する
	void AddTextureData(const wchar_t* key, const wchar_t* path, ID3D11Device* device);

	// 画像を描画する
	void DrawTexture(const wchar_t* key, DirectX::SimpleMath::Vector2 pos,
						DirectX::SimpleMath::Vector4 color, DirectX::SimpleMath::Vector2 rate,
						DirectX::SimpleMath::Vector2 origin, RECT_U rect = RECT_U());

	// 回転率セッター
	void CreateRotation(const wchar_t* key, const float& rotate);

	const float GetRotate(const wchar_t* key) { auto it = m_rotate.find(key); return it->second; }
};

#endif // DRAWSPRITE