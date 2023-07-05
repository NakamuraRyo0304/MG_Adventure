/*
 *	@File	IScene.cpps
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
	m_systemManager{std::make_shared<SystemManager>()},
	m_nextScene{SCENE::NONE},
	is_changeFlag{false}
{
	// 作成されていなければシステムを作成
	if (!m_systemManager)
	{
		m_systemManager = std::make_shared<SystemManager>();
	}
}