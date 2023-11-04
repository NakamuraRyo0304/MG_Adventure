/*
 *	@File	IScene.cpp
 *	@Brief	基底クラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "IScene.h"

// コンストラクタ
IScene::IScene()
	: m_system{ std::make_shared<SystemManager>() }		// システムマネージャ
	, m_factory{ std::make_unique<FactoryManager>() } 	// ファクトリマネージャ
	, m_nextScene{SCENE::NONE}							// 次のシーン
	, m_screenSize{}									// フルスクリーンサイズ
	, m_fadeSpeed{4.0f}									// フェードの速度
	, m_fadeValue{0.0f}									// フェードの現在値
	, is_changeFlag{false}								// シーン切り替えフラグ
{
	// 作成されていなければシステムを作成
	if (not m_system)
	{
		m_system = std::make_shared<SystemManager>();
	}
	if (not m_factory)
	{
		m_factory = std::make_unique<FactoryManager>();
	}
}