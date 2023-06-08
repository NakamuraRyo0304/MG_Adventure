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
// 第１引数：キー 第２引数：表示座標 第３引数：色 第４引数：拡大率 第５引数：中心位置
void DrawSprite::DrawTexture(const wchar_t* key, DirectX::SimpleMath::Vector2 pos, 
	DirectX::SimpleMath::Vector4 color, float rate, DirectX::SimpleMath::Vector2 origin)
{
	m_spriteBatch->Begin();

	// 描画したいキー番号に対応するマップをイテレータに格納
	std::map<const wchar_t*, const wchar_t*>::const_iterator it = m_textures.find(key);

	// 画像の描画
	m_spriteBatch->Draw(
		m_SRV[it->first].Get(),					// 対応するイテレータの画像を描画
		pos,									// 表示する位置
		nullptr,								// 切り取り位置
		color,									// 描画色
		0.0f,									// 回転
		origin,									// 画像の原点
		rate,									// 拡大率
		DirectX::SpriteEffects_None, 0.0f		// 描画レイヤー
	);

	m_spriteBatch->End();
}