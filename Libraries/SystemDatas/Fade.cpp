/*
 *	@File	Fade.cpp
 *	@Brief	画面遷移時のフェード処理。
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Fade.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
Fade::Fade():
	m_fadeNum{},			// フェードカウンタ
	is_fadeInFlag{},		// フェードインフラグ
	is_fadeOutFlag{},		// フェードアウトフラグ
	is_endFlag{}			// 終了判定フラグ
{
	m_fadeNum = MAX_NUM;
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
Fade::~Fade()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="context">ID3D11DeviceContext1ポインタ</param>
/// <param name="device">ID3D11Device1ポインタ</param>
void Fade::Initialize(ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// スプライトバッチの初期化
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// フェードフラグ初期化
	is_endFlag = false;

	// 画像の登録
	CreateDDSTextureFromFile(
		device,
		L"Resources/Textures/FadeBlind.dds",		// テクスチャのパス
		nullptr,
		m_SRV.ReleaseAndGetAddressOf()
	);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="speed">フェードする速度</param>
void Fade::Update(float speed)
{
	// フェードが終わっていたら処理しない
	if (is_endFlag) return;

	// フェードイン
	if (is_fadeInFlag)
	{
 		m_fadeNum -= speed;
		if (m_fadeNum < MIN_NUM)
		{
			m_fadeNum = MIN_NUM;
		}
	}
	// フェードアウト
	if (is_fadeOutFlag)
	{
		m_fadeNum += speed;
		if (m_fadeNum > MAX_NUM)
		{
			m_fadeNum = MAX_NUM;
		}
	}

	// 終了判定
	if (m_fadeNum == MAX_NUM || m_fadeNum == MIN_NUM)
	{
		is_endFlag = true;
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
void Fade::Draw()
{
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(
		m_SRV.Get(),												// スプライト
		SimpleMath::Vector2{0.0f},									// 座標
		nullptr,													// 切り抜き位置
		SimpleMath::Vector4{1.0f,1.0f,1.0f,m_fadeNum / MAX_NUM},	// 色
		0.f,														// 回転
		SimpleMath::Vector2::Zero,									// 中心位置
		1.f,														// スケール
		SpriteEffects_None,	0.f
	);

	m_spriteBatch->End();
}

/// <summary>
/// フェードインを開始する関数
/// </summary>
/// <param name="引数無し"></param>
void Fade::SetFadeIn()
{
	Reset();
	is_endFlag = false;
	is_fadeInFlag = true;
	m_fadeNum = MAX_NUM;
}

/// <summary>
/// フェードアウトを開始する関数
/// </summary>
/// <param name="引数無し"></param>
void Fade::SetFadeOut()
{
	Reset();
	is_endFlag = false;
	is_fadeOutFlag = true;
	m_fadeNum = MIN_NUM;
}

/// <summary>
/// フェード処理を行う前に呼び出す関数
/// </summary>
/// <param name="引数無し"></param>
void Fade::Reset()
{
	m_fadeNum = 0.0f;
	is_fadeInFlag = false;
	is_fadeOutFlag = false;
	is_endFlag = false;
}
