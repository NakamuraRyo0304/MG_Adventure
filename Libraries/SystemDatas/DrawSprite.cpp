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
 /// <returns>なし</returns>
DrawSprite::DrawSprite()
	: m_spriteBatch{}	// スプライトバッチ
	, m_textures{}		// テクスチャ配列
	, m_SRV{}			// シェーダーリソースビュー
	, m_rotate{}		// 画像の回転率
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
DrawSprite::~DrawSprite()
{
	m_textures.clear();
	m_SRV.clear();
}

/// <summary>
/// スプライトバッチを作成
/// </summary>
/// <param name="context">ID3D11DeviceContext1ポインタ</param>
/// <returns>なし</returns>
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
/// <returns>なし</returns>
void DrawSprite::AddTextureData(const wchar_t* key, const wchar_t* path ,ID3D11Device* device)
{
	// 画像の追加
	m_textures.emplace(key, path);

	for (std::map<const wchar_t*, const wchar_t*>::const_iterator _it = m_textures.begin(); _it != m_textures.end(); ++_it)
	{
		// 画像の登録
		CreateDDSTextureFromFile(
			device,										// デバイスポインタ
			_it->second,								// テクスチャのパス
			nullptr,									// 特性は識別しない
			m_SRV[_it->first].ReleaseAndGetAddressOf()	// 対応するキー番号に登録
		);
	}

	// 初期化で０を入れる
	m_rotate.emplace(key, 0.0f);
}

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
void DrawSprite::DrawTexture(const wchar_t* key, SimpleMath::Vector2 pos,
	SimpleMath::Vector4 color, SimpleMath::Vector2 rate, SimpleMath::Vector2 origin, RECT_U rect)
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

/// <summary>
/// 回転量セッター
/// </summary>
/// <param name="key">登録キー</param>
/// <param name="rotate">回転量</param>
/// <returns>なし</returns>
void DrawSprite::CreateRotation(const wchar_t* key, const float& rotate)
{
	// 回転量を格納
	std::map<const wchar_t*, float>::iterator _rt = m_rotate.find(key);

	_rt->second = rotate;
}
