/*
 *	@File	SystemManager.cpp
 *	@Brief	ゲームを作成するためのシステムの管理クラス。
 *	@Date	2023-06-02
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SystemManager.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
SystemManager::SystemManager()
	: m_commonState{nullptr}
	, m_drawSprite{nullptr}
	, m_drawString{nullptr}
	, m_effect{nullptr}
	, m_gridFloor{nullptr}
	, m_keyboardStateTracker{nullptr}
	, m_mouseStateTracker{nullptr}
	, m_camera{nullptr}
	, m_pDR{nullptr}
	, m_rayCast{nullptr}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
SystemManager::~SystemManager()
{

}


/// <summary>
/// デバイスリソースの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>デバイスリソースの生ポインタ</returns>
DX::DeviceResources*
SystemManager::GetDeviceResources()
{
	if (!m_pDR)
	{
		m_pDR = DX::DeviceResources::GetInstance();
	}
	return m_pDR;
}

/// <summary>
/// コモンステートの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>コモンステートのユニークポインタ</returns>
const std::unique_ptr<CommonStates>&
SystemManager::GetCommonStates()
{
	if (!m_commonState)
	{
		m_commonState = std::make_unique<CommonStates>(GetDeviceResources()->GetD3DDevice());
	}
	return m_commonState;
}

/// <summary>
/// キーボードトラッカーの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>キーボードトラッカーのユニークポインタ</returns>
const std::unique_ptr<Keyboard::KeyboardStateTracker>&
SystemManager::GetStateTrack()
{
	if (!m_keyboardStateTracker)
	{
		throw;
	}
	return m_keyboardStateTracker;
}

/// <summary>
/// 文字描画クラスの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>文字描画クラスのユニークポインタ</returns>
const std::unique_ptr<DrawString>&
SystemManager::GetString()
{
	if (!m_drawString)
	{
		throw;
	}
	return m_drawString;
}

/// <summary>
/// マウストラッカーの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>マウストラッカーのユニークポインタ</returns>
const std::unique_ptr<Mouse::ButtonStateTracker>&
SystemManager::GetMouseTrack()
{
	if (!m_mouseStateTracker)
	{
		throw;
	}
	return m_mouseStateTracker;
}

/// <summary>
/// ドローフロアの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>ドローフロアのユニークポインタ</returns>
const std::unique_ptr<GridFloor>&
SystemManager::GetGridFloor()
{
	if (!m_gridFloor)
	{
		throw;
	}
	return m_gridFloor;
}

/// <summary>
/// レイキャストの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>レイキャストのユニークポインタ</returns>
const std::unique_ptr<RayCast>&
SystemManager::GetRayCast()
{
	if (!m_rayCast)
	{
		throw;
	}
	return m_rayCast;
}

/// <summary>
/// ドロースプライトの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>ドロースプライトのユニークポインタ</returns>
const std::unique_ptr<DrawSprite>&
SystemManager::GetDrawSprite()
{
	if (!m_drawSprite)
	{
		throw;
	}
	return m_drawSprite;
}

/// <summary>
/// カメラの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>カメラのユニークポインタ</returns>
const std::unique_ptr<Camera>&
SystemManager::GetCamera()
{
	if (!m_camera)
	{
		m_camera = std::make_unique<Camera>();
	}
	return m_camera;
}

/// <summary>
/// ベーシックエフェクトの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>ベーシックエフェクトのユニークポインタ</returns>
const std::unique_ptr<BasicEffect>&
SystemManager::GetBasicEffect()
{
	if (!m_effect)
	{
		m_effect = std::make_unique<BasicEffect>(GetDeviceResources()->GetD3DDevice());
	}
	return m_effect;
}

/// <summary>
/// サウンドマネージャの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>サウンドマネージャのユニークポインタ</returns>
const std::unique_ptr<SoundManager>& SystemManager::GetSoundManager()
{
	if (!m_soundManager)
	{
		m_soundManager = std::make_unique<SoundManager>();
	}
	return m_soundManager;
}

/// <summary>
/// 一括でシステムのリソースを作成
/// </summary>
/// <param name="device">ID3D11Device1のポインタ</param>
/// <param name="context">ID3D11DeviceContext1のポインタ</param>
/// <returns>なし</returns>
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

	// サウンドマネージャ
	m_soundManager = std::make_unique<SoundManager>();
}