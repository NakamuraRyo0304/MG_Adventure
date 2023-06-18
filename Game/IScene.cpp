/*
 *	@File	IScene.cpps
 *	@Brief	基底クラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "IScene.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
IScene::IScene():
	m_systemManager{std::make_shared<SystemManager>()},
	m_nextScene{SCENE::NONE}
{
	// 作成されていなければシステムを作成
	if (!m_systemManager)
	{
		m_systemManager = std::make_shared<SystemManager>();
	}
}