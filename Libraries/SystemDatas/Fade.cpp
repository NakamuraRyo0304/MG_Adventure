/*
 *	@File	Fade.cpp
 *	@Brief	画面遷移時のフェード処理。
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Transition.h"

#include "Fade.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="speed">フェードの速度</param>
 /// <returns>なし</returns>
Fade::Fade(float speed)
	: m_fadeNum{}			// フェードカウンタ
	, m_fadeSpeed{speed}	// フェード速度
	, is_fadeInFlag{}		// フェードインフラグ
	, is_fadeOutFlag{}		// フェードアウトフラグ
	, is_endFlag{}			// 終了判定フラグ
{
	m_fadeNum = MAX_NUM;
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
Fade::~Fade()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="pDR">デバイスリソース</param>
/// <returns>なし</returns>
void Fade::Initialize(DX::DeviceResources* pDR)
{
	// トランジションを作成
	m_transition = std::make_unique<Transition>();
	m_transition->Create(pDR);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Fade::Update()
{
	// フェードが終わっていたら処理しない
	if (is_endFlag) return;

	// フェードイン
	if (is_fadeInFlag)
	{
 		m_fadeNum -= m_fadeSpeed / CLAMP_NUM;
		if (m_fadeNum < MIN_NUM)
		{
			m_fadeNum = MIN_NUM;
		}
	}
	// フェードアウト
	if (is_fadeOutFlag)
	{
		m_fadeNum += m_fadeSpeed / CLAMP_NUM;
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
/// <returns>なし</returns>
void Fade::Draw()
{
	// フェードが終わっていたら描画しない
	if (is_endFlag) return;

	// 描画関数
	m_transition->Render(m_fadeNum);
}

/// <summary>
/// フェードインを開始する関数
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
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
/// <returns>なし</returns>
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
/// <returns>なし</returns>
void Fade::Reset()
{
	m_fadeNum = MIN_NUM;
	is_fadeInFlag = false;
	is_fadeOutFlag = false;
	is_endFlag = false;
}
