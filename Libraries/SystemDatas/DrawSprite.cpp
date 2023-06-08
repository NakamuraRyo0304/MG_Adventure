/*
 *	@File	DrawSprite.cpp
 *	@Brief	画像を描画するクラス。
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "DrawSprite.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
DrawSprite::DrawSprite():
	m_textures{},
	m_SRV{},
	m_spriteBatch{}
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
DrawSprite::~DrawSprite()
{
}

//--------------------------------------------------------//
//スプライトバッチを作成                                  //
//--------------------------------------------------------//
void DrawSprite::MakeSpriteBatch(ID3D11DeviceContext1* context)
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
}

//--------------------------------------------------------//
//画像を登録する関数                                      //
//--------------------------------------------------------//
// 第１引数：登録名 第２引数：画像のパス 第３引数：デバイス
void DrawSprite::AddTextureData(const wchar_t* key, const wchar_t* path ,ID3D11Device* device)
{
	// 画像の追加
	m_textures.emplace(key, path);

	for (std::map<const wchar_t*, const wchar_t*>::const_iterator it = m_textures.begin(); it != m_textures.end(); ++it)
	{

		// 画像の登録
		DirectX::CreateDDSTextureFromFile(
			device,
			it->second,	// テクスチャのパス
			nullptr,
			m_SRV[it->first].ReleaseAndGetAddressOf() // 対応するキー番号に登録
		);
	}
}

//--------------------------------------------------------//
//画像を描画する                                          //
//--------------------------------------------------------//
// 第１引数：キー 第２引数：表示座標 第３引数：色 第４引数：拡大率
void DrawSprite::DrawTexture(const wchar_t* key, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector4 color, float rate)
{
	m_spriteBatch->Begin();

	std::map<const wchar_t*, const wchar_t*>::const_iterator it = m_textures.find(key);

	m_spriteBatch->Draw(
		m_SRV[it->first].Get(),
		pos,
		nullptr,
		color,
		0.0f,
		DirectX::SimpleMath::Vector2(0.0f, 0.0f),
		rate,
		DirectX::SpriteEffects_None, 0.0f
	);

	m_spriteBatch->End();
}