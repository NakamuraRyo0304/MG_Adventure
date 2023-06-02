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

// 自作関数
#include "../Libraries/SystemDatas/DrawString.h"
#include "../Libraries/SystemDatas/Camera.h"
#include "../Libraries/SystemDatas/GridFloor.h"
#include "../Libraries/SystemDatas/RayCast.h"
#include "../Libraries/UserUtility.h"

class IScene
{
private:
	// デバイスリソーシーズ
	DX::DeviceResources*										m_pDR = nullptr;
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>						m_commonState;
	// 押された瞬間を検知する
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>	m_keyboardStateTracker;
	// マウス
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker>			m_mouseStateTracker;
	// 文字描画
	std::unique_ptr<DrawString>                                 m_drawString;
	// グリッド床
	std::unique_ptr<GridFloor>                                  m_gridFloor;
	// レイキャスト
	std::unique_ptr<RayCast>									m_rayCast;
private:
	// 次のシーンを指定する
	SCENE m_nextScene = SCENE::NONE;
public:
	// カメラ
	Camera*														 m_camera = nullptr;

public:

	virtual ~IScene()
	{
		DeletePointer(m_camera);
	};

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

public: // ゲッター、セッター、ファクトリー
	// 共通のメイクユニーク
	void CreateUnique(ID3D11Device1* device, ID3D11DeviceContext1* context);

	// unique_ptrのアクセサ

	// デバイスリソースの取得
	DX::DeviceResources* GetDeviceResources();

	// コモンステートの取得
	const std::unique_ptr<DirectX::CommonStates>& GetCommonStates();

	// キーボードステートトラッカーの取得
	const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& GetStateTrack();

	// ドローストリングの取得
	const std::unique_ptr<DrawString>& GetString();

	// マウストラッカーの取得
	const std::unique_ptr<DirectX::Mouse::ButtonStateTracker>& GetMouseTrack();
	
	// グリッドフロアの取得
	const std::unique_ptr<GridFloor>& GetGridFloor();
	
	// レイキャストの取得
	const std::unique_ptr<RayCast>& GetRayCast();
	
	// モデルの作成をするファクトリー
	std::unique_ptr<DirectX::DX11::Model> ModelFactory(ID3D11Device1* device, const wchar_t* filename);

public:
	// 共通機能（使用）
	
	// 遷移先のシーン設定
	void GoNextScene(SCENE nextScene) 
	{
		m_nextScene = nextScene; 
	};

	// ソフト終了
	void ExitApp()
	{
		PostQuitMessage(0); 
	};

public:
	// 共通機能（内部使用）
	
	// 次のシーンをゲット
	SCENE GetNextScene()
	{
		return m_nextScene;
	};
	// ポインタの削除
	template <typename T>
	inline void DeletePointer(T*& p)
	{
		delete p; 
		p = nullptr;
	}
};

#endif // ISCENE