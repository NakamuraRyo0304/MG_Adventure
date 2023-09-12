/*
 *	@File	IScene.cpp
 *	@Brief	基底クラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "IScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
IScene::IScene():
	m_system{std::make_shared<SystemManager>()},
	m_nextScene{SCENE::NONE},
	is_changeFlag{false},
	m_fadeSpeed{4.0f}
{
	// 作成されていなければシステムを作成
	if (!m_system)
	{
		m_system = std::make_shared<SystemManager>();
	}
}