/*
 *	@File	IScene.h
 *	@Brief	基底クラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef ISCENE
#define	ISCENE

#include "GameMain.h"

// ライブラリ
#include "../Libraries/SystemManager/SystemManager.h"
#include "../Libraries/UserUtility.h"
#include "../Factories/ModelFactory.h"

class IScene
{
private:
	// システムマネージャ
	std::unique_ptr<SystemManager> m_systemManager;

	// 次のシーンを指定する
	SCENE m_nextScene;

public:
	IScene();

	virtual ~IScene() = default;

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update(const float& elapsedTime,DirectX::Keyboard::State& keyState,DirectX::Mouse::State& mouseState) = 0;

	// 描画
	virtual void Draw() = 0;

	// 終了処理
	virtual void Finalize() = 0;

	// 画面依存の初期化
	virtual void CreateWindowDependentResources() = 0;

public:
	// ステージ番号
	int m_stageNum = 0;
	
	// 番号ゲッター
	int GetStageNum() { return m_stageNum; }
	
	// 番号セッター
	void SetStageNum(int stageNum) { m_stageNum = stageNum; }

public:
	// 遷移先のシーン設定
	void GoNextScene(SCENE nextScene) {	m_nextScene = nextScene; }

	// ソフト終了
	void ExitApp(){	PostQuitMessage(0);	}

	// 次のシーンをゲット
	SCENE GetNextScene() { return m_nextScene; }

	// システムマネージャをゲット
	std::unique_ptr<SystemManager>& GetSystemManager() { return m_systemManager; }
};

#endif // ISCENE