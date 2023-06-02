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

#include "../SystemDatas/DrawString.h"
#include "../SystemDatas/Camera.h"
#include "../SystemDatas/GridFloor.h"
#include "../SystemDatas/RayCast.h"

class SystemManager
{
private:
	// デバイスリソーシーズ
	DX::DeviceResources* m_pDR = nullptr;
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
public:
	// カメラ
	Camera* m_camera = nullptr;

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

	// カメラの取得
	Camera*& GetCamera();

	// モデルの作成をするファクトリー
	std::unique_ptr<DirectX::DX11::Model> ModelFactory(ID3D11Device1* device, const wchar_t* filename);

public:
	// ポインタの削除
	template <typename T>
	inline void DeletePointer(T*& p)
	{
		delete p;
		p = nullptr;
	}
};

#endif // SYSTEMMANAGER