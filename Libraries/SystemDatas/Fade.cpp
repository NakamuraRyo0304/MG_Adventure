/*
 *	@File	Fade.cpp
 *	@Brief	画面遷移時のフェード処理。
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Transition.h"

#include "Fade.h"

// コンストラクタ
Fade::Fade(float speed)
	: m_fadeNum{}			// フェードカウンタ
	, m_fadeSpeed{speed}	// フェード速度
	, is_fadeInFlag{}		// フェードインフラグ
	, is_fadeOutFlag{}		// フェードアウトフラグ
	, is_endFlag{}			// 終了判定フラグ
{
	m_fadeNum = MAX_NUM;
}

// デストラクタ
Fade::~Fade()
{
}

// 初期化処理
void Fade::Initialize()
{
	// トランジションを作成
	m_transition = std::make_unique<Transition>();
	m_transition->Create();
}

// 更新処理
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

// 描画処理
void Fade::Draw()
{
	// フェードが終わっていたら描画しない
	if (is_endFlag) return;

	// 描画関数
	m_transition->Render(m_fadeNum);
}

// フェードインスタート
void Fade::SetFadeIn()
{
	Reset();
	is_endFlag = false;
	is_fadeInFlag = true;
	m_fadeNum = MAX_NUM;
}

// フェードアウトスタート
void Fade::SetFadeOut()
{
	Reset();
	is_endFlag = false;
	is_fadeOutFlag = true;
	m_fadeNum = MIN_NUM;
}

// リセット関数
void Fade::Reset()
{
	m_fadeNum = MIN_NUM;
	is_fadeInFlag = false;
	is_fadeOutFlag = false;
	is_endFlag = false;
	m_transition->CycleTextures();
}
