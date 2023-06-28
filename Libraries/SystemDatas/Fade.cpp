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
		if (m_fadeNum <= MIN_NUM)
		{
			m_fadeNum = MIN_NUM;
			is_endFlag = true;
		}
	}
	// フェードアウト
	if (is_fadeOutFlag)
	{
		m_fadeNum += speed;
		if (m_fadeNum >= MAX_NUM)
		{
			m_fadeNum = MAX_NUM;
			is_endFlag = true;
		}
	}
}

//--------------------------------------------------------//
//フェードイン                                            //
//--------------------------------------------------------//
void Fade::SetFadeIn()
{
	m_fadeNum = MAX_NUM;
	is_fadeInFlag = true;
	is_endFlag = false;
}

//--------------------------------------------------------//
//フェードアウト                                          //
//--------------------------------------------------------//
void Fade::SetFadeOut()
{
	m_fadeNum = MIN_NUM;
	is_fadeOutFlag = true;
	is_endFlag = false;
}
