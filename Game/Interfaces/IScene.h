/*
 *	@File	IScene.h
 *	@Brief	基底クラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef ISCENE
#define	ISCENE

#include "../GameMain.h"

 // ライブラリ
#include "../../Libraries/SystemManager/SystemManager.h"
#include "../../Libraries/UserUtility.h"
#include "../../Libraries/SystemDatas/Input.h"
#include "../../Libraries/Factories/ModelFactory.h"

class IScene
{
private:
	// システムマネージャ
	std::shared_ptr<SystemManager> m_system;

	// 次のシーンを指定する
	SCENE m_nextScene;

	// スクリーンサイズ
	DirectX::SimpleMath::Vector2 m_screenSize;
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.f,1080.f };

	// シーン切り替え判定フラグ
	bool is_changeFlag;

	// シーン切り替えフェード速度
	float m_fadeSpeed;

	// フェードの値
	float m_fadeValue;

private:

	// シーン内の変数の初期化関数
	virtual void SetSceneValues() = 0;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="可変長引数">任意の値を渡す</param>
	/// <returns>なし</returns>
	IScene()
		: m_system{ std::make_shared<SystemManager>() }	// システムマネージャ
		, m_nextScene{ SCENE::NONE }					// 次のシーン
		, m_screenSize{}							    // フルスクリーンサイズ
		, m_fadeSpeed{ 4.0f }							// フェードの速度
		, m_fadeValue{ 0.0f }							// フェードの現在値
		, is_changeFlag{ false }						// シーン切り替えフラグ
	{
		// 作成されていなければシステムを作成
		if (!m_system)
		{
			m_system = std::make_shared<SystemManager>();
		}
	}

	virtual ~IScene() = default;

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 終了処理
	virtual void Finalize() = 0;

	// 画面依存の初期化
	virtual void CreateWindowDependentResources() = 0;

public:
	// フェード速度アクセサ
	const float& GetFadeSpeed(){ return m_fadeSpeed; }
	void SetFadeSpeed(const float& speed) { m_fadeSpeed = speed; }

	// フェード値アクセサ
	const float& GetFadeValue() { return m_fadeValue; }
	void SetFadeValue(const float& value) { m_fadeValue = value; }

public:

	//-------------------------------------------------------------------------------------//
	// ゲームシーン内で使用する関数
	//-------------------------------------------------------------------------------------//

	// システムマネージャを取得する
	inline const std::shared_ptr<SystemManager>& GetSystemManager() { return m_system; }

	// 遷移先のシーン設定
	inline void ChangeScene(const SCENE& nextScene)
	{
		m_nextScene = nextScene;
		is_changeFlag = true;
	}

	// スクリーンサイズを取得する
	const DirectX::SimpleMath::Vector2& GetScreenSize() { return m_screenSize; }
	// フルスクリーンサイズを取得する
	const DirectX::SimpleMath::Vector2& GetFullScreenSize() { return FULL_SCREEN_SIZE; }


	//-------------------------------------------------------------------------------------//
	// GameMainで使用する関数 (通常は使用しない)
	//-------------------------------------------------------------------------------------//

	// フェード用遷移阻止関数
	inline void StopNextScene()
	{
		m_nextScene = SCENE::NONE;
		is_changeFlag = false;
	}

	// 画面サイズを設定する
	inline void SetScreenSize(const DirectX::SimpleMath::Vector2& size) { m_screenSize = size; }

	// ソフト終了
	inline void ExitApp(){	PostQuitMessage(0);	}

	// 次のシーンをゲット
	inline const SCENE& GetNextScene() { return m_nextScene; }

	// シーンフラグをゲット
	inline const bool& GetChangeSceneFlag() { return is_changeFlag; }
};

#endif // ISCENE