/*
 *	@File	DrawSprite.cpp
 *	@Brief	画像を描画するクラス。
 *	@Date	2023-06-09
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "DrawSprite.h"

// コンストラクタ
DrawSprite::DrawSprite()
	: m_spriteBatch{}	// スプライトバッチ
	, m_textures{}		// テクスチャ配列
	, m_SRV{}			// シェーダーリソースビュー
	, m_rotate{}		// 画像の回転率
{
}

// デストラクタ
DrawSprite::~DrawSprite()
{
	m_textures.clear();
	m_SRV.clear();
}

// スプライトバッチを作成
void DrawSprite::MakeSpriteBatch()
{
	m_spriteBatch = std::make_unique<SpriteBatch>(DX::DeviceResources::GetInstance()->GetD3DDeviceContext());
}

//  画像を登録する関数
void DrawSprite::AddTextureData(const wchar_t* key, const wchar_t* path)
{
	// 画像の追加
	m_textures.emplace(key, path);

	for (std::map<const wchar_t*, const wchar_t*>::const_iterator i = m_textures.begin(); i != m_textures.end(); ++i)
	{
		// 画像の登録
		CreateDDSTextureFromFile(
			DX::DeviceResources::GetInstance()->GetD3DDevice(),	// デバイスポインタ
			i->second,											// テクスチャのパス
			nullptr,											// 特性は識別しない
			m_SRV[i->first].ReleaseAndGetAddressOf()			// 対応するキー番号に登録
		);
	}

	// 初期化で０を入れる
	m_rotate.emplace(key, 0.0f);
}

// 画像を描画する
void DrawSprite::DrawTexture(const wchar_t* key, SimpleMath::Vector2 pos,
	SimpleMath::Vector4 color, SimpleMath::Vector2 rate, SimpleMath::Vector2 origin, RECT rect)
{
	m_spriteBatch->Begin();

	// 描画したいキー番号に対応するマップをイテレータに格納
	std::map<const wchar_t*, const wchar_t*>::const_iterator _it = m_textures.find(key);

	// 回転量を格納
	std::map<const wchar_t*, float>::const_iterator _rt = m_rotate.find(key);

	// 画像の描画
	m_spriteBatch->Draw(
		m_SRV[_it->first].Get(),				// 対応するイテレータの画像を描画
		pos,									// 表示する位置
		&rect,								    // 切り取り位置
		color,									// 描画色
		m_rotate[_it->first],					// 回転
		origin,									// 画像の原点
		rate,									// 拡大率
		SpriteEffects_None, 0.0f				// 描画レイヤー
	);

	m_spriteBatch->End();
}

// 回転量セッター
void DrawSprite::CreateRotation(const wchar_t* key, const float& rotate)
{
	// 回転量を格納
	std::map<const wchar_t*, float>::iterator _rt = m_rotate.find(key);

	_rt->second = rotate;
}
