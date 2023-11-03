/*
 *	@File	SystemManager.h
 *	@Brief	ゲームを作成するためのシステムの管理クラス。
 *	@Date	2023-06-02
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SYSTEMMANAGER
#define SYSTEMMANAGER

#include "Game/GameMain.h"

 //-------------------------------------------------------------------------------------//
 // システムのインクルード
#include "../SystemDatas/Camera.h"
#include "../SystemDatas/RayCast.h"
#include "../SystemDatas/DrawSprite.h"
#include "../SystemDatas/SoundManager.h"
#include "../SystemDatas/GridFloor.h"
#include "../SystemDatas/DrawString.h"

//-------------------------------------------------------------------------------------//

class SystemManager
{
private:
	// デバイスリソーシーズ
	DX::DeviceResources*										m_pDR = nullptr;
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>						m_commonState;
	// レイキャスト
	std::unique_ptr<RayCast>									m_rayCast;
	// ドロースプライト
	std::unique_ptr<DrawSprite>									m_drawSprite;
	// カメラ
	std::unique_ptr<Camera>										m_camera;
	// サウンド
	std::unique_ptr<SoundManager>								m_soundManager;
	// 文字描画
	std::unique_ptr<Debug::DrawString>                          m_drawString;
	// グリッド床
	std::unique_ptr<Debug::GridFloor>                           m_gridFloor;


public: // ゲッター、セッター、ファクトリー
	// 共通のメイクユニーク
	void CreateUnique();

	// unique_ptrのアクセサ

	// デバイスリソースの取得
	DX::DeviceResources* GetDeviceResources();

	// コモンステートの取得
	const std::unique_ptr<DirectX::CommonStates>& GetCommonStates();

	// レイキャストの取得
	const std::unique_ptr<RayCast>& GetRayCast();

	// ドロースプライトの取得
	const std::unique_ptr<DrawSprite>& GetDrawSprite();

	// カメラの取得
	const std::unique_ptr<Camera>& GetCamera();

	// サウンドマネージャの取得
	const std::unique_ptr<SoundManager>& GetSoundManager();

	// ドローストリングの取得
	const std::unique_ptr<Debug::DrawString>& GetString();

	// グリッドフロアの取得
	const std::unique_ptr<Debug::GridFloor>& GetGridFloor();

public:
	SystemManager();
	~SystemManager();
};

#endif // SYSTEMMANAGER