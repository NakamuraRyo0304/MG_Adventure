/*
 *	@File	Fade.cpp
 *	@Brief	画面遷移時のフェード処理。
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Fade.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
Fade::Fade():
	m_fadeNum{},
	is_fadeInFlag{},
	is_fadeOutFlag{},
	is_endFlag{}
{
	m_fadeNum = MAX_NUM;
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
Fade::~Fade()
{
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
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

//--------------------------------------------------------//
//フェードアップデート                                    //
//--------------------------------------------------------//
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

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
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

//--------------------------------------------------------//
//フェードイン                                            //
//--------------------------------------------------------//
void Fade::SetFadeIn()
{
	is_endFlag = false;
	is_fadeInFlag = true;
	m_fadeNum = MAX_NUM;
}

//--------------------------------------------------------//
//フェードアウト                                          //
//--------------------------------------------------------//
void Fade::SetFadeOut()
{
	is_endFlag = false;
	is_fadeOutFlag = true;
	m_fadeNum = MIN_NUM;
}

//--------------------------------------------------------//
//リセット関数                                            //
//--------------------------------------------------------//
void Fade::Reset()
{
	m_fadeNum = 0.0f;
	is_fadeInFlag = false;
	is_fadeOutFlag = false;
	is_endFlag = false;
}
