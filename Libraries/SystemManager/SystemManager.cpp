/*
 *	@File	SystemManager.cpp
 *	@Brief	ゲームを作成するためのシステムの管理クラス。
 *	@Date	2023-06-02
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SystemManager.h"

// コンストラクタ
SystemManager::SystemManager()
	:m_commonState{nullptr}
	,m_drawSprite{nullptr}
	,m_drawString{nullptr}
	,m_effect{nullptr}
	,m_gridFloor{nullptr}
	,m_keyboardStateTracker{nullptr}
	,m_mouseStateTracker{nullptr}
	,m_camera{nullptr}
	,m_pDR{nullptr}
	,m_rayCast{nullptr}
{
}

// デストラクタ
SystemManager::~SystemManager()
{

}


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
const std::unique_ptr<CommonStates>&
SystemManager::GetCommonStates()
{
	if (!m_commonState)
	{
		m_commonState = std::make_unique<CommonStates>(GetDeviceResources()->GetD3DDevice());
	}
	return m_commonState;
}

// KeyboardStateTracker
const std::unique_ptr<Keyboard::KeyboardStateTracker>&
SystemManager::GetStateTrack()
{
	if (!m_keyboardStateTracker)
	{
		throw;
	}
	return m_keyboardStateTracker;
}

// DrawString
const std::unique_ptr<DrawString>&
SystemManager::GetString()
{
	if (!m_drawString)
	{
		throw;
	}
	return m_drawString;
}

// MouseTracker
const std::unique_ptr<Mouse::ButtonStateTracker>&
SystemManager::GetMouseTrack()
{
	if (!m_mouseStateTracker)
	{
		throw;
	}
	return m_mouseStateTracker;
}

// GridFloor
const std::unique_ptr<GridFloor>&
SystemManager::GetGridFloor()
{
	if (!m_gridFloor)
	{
		throw;
	}
	return m_gridFloor;
}

// RayCast
const std::unique_ptr<RayCast>&
SystemManager::GetRayCast()
{
	if (!m_rayCast)
	{
		throw;
	}
	return m_rayCast;
}

// DrawSprite
const std::unique_ptr<DrawSprite>&
SystemManager::GetDrawSprite()
{
	if (!m_drawSprite)
	{
		throw;
	}
	return m_drawSprite;
}

// Camera
const std::unique_ptr<Camera>&
SystemManager::GetCamera()
{
	if (!m_camera)
	{
		m_camera = std::make_unique<Camera>();
	}
	return m_camera;
}

// BasicEffect
const std::unique_ptr<BasicEffect>&
SystemManager::GetBasicEffect()
{
	if (!m_effect)
	{
		m_effect = std::make_unique<BasicEffect>(GetDeviceResources()->GetD3DDevice());
	}
	return m_effect;
}

//--------------------------------------------------------//
//メイクユニーク                                          //
//--------------------------------------------------------//
void SystemManager::CreateUnique(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// デバイスリソース
	m_pDR = DX::DeviceResources::GetInstance();

	// ３Ｄレンダリング
	m_commonState = std::make_unique<CommonStates>(device);

	// キーボード
	m_keyboardStateTracker
		= std::make_unique<Keyboard::KeyboardStateTracker>();

	// 文字の描画
	m_drawString = std::make_unique<DrawString>();

	// カメラの初期化
	m_camera = std::make_unique<Camera>();

	// マウス
	m_mouseStateTracker
		= std::make_unique<Mouse::ButtonStateTracker>();

	// グリッドフロア 30 x 30
	m_gridFloor = std::make_unique<GridFloor>(device, context, 30, 30);

	// レイを飛ばす
	m_rayCast = std::make_unique<RayCast>();

	// 画像の描画
	m_drawSprite = std::make_unique<DrawSprite>();

	// ベーシックエフェクト
	m_effect = std::make_unique<BasicEffect>(device);
}