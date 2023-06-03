/*
 *	@File	SystemManager.cpp
 *	@Brief	ゲームを作成するためのシステムの管理クラス。
 *	@Date	2023-06-02
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SystemManager.h"

// DeviceResources
DX::DeviceResources*
SystemManager::GetDeviceResources()
{
	if (!m_pDR)
	{
		m_pDR = DX::DeviceResources::GetInstance();
	}
	return m_pDR;
}

// CommonStates
const std::unique_ptr<DirectX::CommonStates>&
SystemManager::GetCommonStates()
{
	if (!m_commonState)
	{
		m_commonState = std::make_unique<DirectX::CommonStates>(GetDeviceResources()->GetD3DDevice());
	}
	return m_commonState;
}

// KeyboardStateTracker
const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>&
SystemManager::GetStateTrack()
{
	if (!m_keyboardStateTracker)
	{
		return nullptr;
	}
	return m_keyboardStateTracker;
}

// DrawString
const std::unique_ptr<DrawString>&
SystemManager::GetString()
{
	if (!m_drawString)
	{
		return nullptr;
	}
	return m_drawString;
}

// MouseTracker
const std::unique_ptr<DirectX::Mouse::ButtonStateTracker>&
SystemManager::GetMouseTrack()
{
	if (!m_mouseStateTracker)
	{
		return nullptr;
	}
	return m_mouseStateTracker;
}

// GridFloor
const std::unique_ptr<GridFloor>&
SystemManager::GetGridFloor()
{
	if (!m_gridFloor)
	{
		return nullptr;
	}
	return m_gridFloor;
}

// RayCast
const std::unique_ptr<RayCast>&
SystemManager::GetRayCast()
{
	if (!m_rayCast)
	{
		return nullptr;
	}
	return m_rayCast;
}

//--------------------------------------------------------//
//カメラゲッター                                          //
//--------------------------------------------------------//
Camera*& SystemManager::GetCamera()
{
	return m_camera;
}

//--------------------------------------------------------//
//メイクユニーク                                          //
//--------------------------------------------------------//
void SystemManager::CreateUnique(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// デバイスリソース
	m_pDR = DX::DeviceResources::GetInstance();

	// ３Ｄレンダリング
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// キーボード
	m_keyboardStateTracker
		= std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	// 文字の描画
	m_drawString = std::make_unique<DrawString>();

	// カメラの初期化
	m_camera = new Camera();

	// マウス
	m_mouseStateTracker
		= std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	// グリッドフロア 30 x 30
	m_gridFloor = std::make_unique<GridFloor>(device, context, 30, 30);

	// レイを飛ばす
	m_rayCast = std::make_unique<RayCast>();

}
