/*
 *	@File	DrawSprite.cpp
 *	@Brief	画像を描画するクラス。
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "DrawSprite.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
DrawSprite::DrawSprite():
	m_textures{},
	m_SRV{},
	m_spriteBatch{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
DrawSprite::~DrawSprite()
{
	m_textures.clear();
	m_SRV.clear();
}

/// <summary>
/// スプライトバッチを作成
/// </summary>
/// <param name="context">ID3D11DeviceContext1ポインタ</param>
void DrawSprite::MakeSpriteBatch(ID3D11DeviceContext1* context)
{
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
}

/// <summary>
/// 画像を登録する関数
/// </summary>
/// <param name="key">登録キー(これを指定して呼び出す)</param>
/// <param name="path">画像のパス(L"Resources/Textures/....dds)拡張子は「.dds」</param>
/// <param name="device">ID3D11Deviceポインタ</param>
void DrawSprite::AddTextureData(const wchar_t* key, const wchar_t* path ,ID3D11Device* device)
{
	// 画像の追加
	m_textures.emplace(key, path);

	for (std::map<const wchar_t*, const wchar_t*>::const_iterator it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		// 画像の登録
		CreateDDSTextureFromFile(
			device,
			it->second,	// テクスチャのパス
			nullptr,
			m_SRV[it->first].ReleaseAndGetAddressOf() // 対応するキー番号に登録
		);
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="key">キー</param>
/// <param name="pos">表示座標</param>
/// <param name="color">色</param>
/// <param name="rate">拡大率</param>
/// <param name="origin">中心位置</param>
void DrawSprite::DrawTexture(const wchar_t* key, SimpleMath::Vector2 pos, 
	SimpleMath::Vector4 color, float rate, SimpleMath::Vector2 origin)
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
		SpriteEffects_None, 0.0f				// 描画レイヤー
	);

	m_spriteBatch->End();
}