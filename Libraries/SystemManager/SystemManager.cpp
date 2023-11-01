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
	, m_effect{nullptr}
	, m_camera{nullptr}
	, m_pDR{nullptr}
	, m_rayCast{nullptr}
#ifdef _DEBUG
	, m_gridFloor{nullptr}
	, m_drawString{nullptr}
#endif
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

#ifdef _DEBUG

/// <summary>
/// 文字描画クラスの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>文字描画クラスのユニークポインタ</returns>
const std::unique_ptr<Debug::DrawString>&
SystemManager::GetString()
{
	if (!m_drawString)
	{
		m_pDR = DX::DeviceResources::GetInstance();
		m_drawString = std::make_unique<Debug::DrawString>(m_pDR->GetD3DDevice(), m_pDR->GetD3DDeviceContext());
	}
	return m_drawString;
}
/// <summary>
/// ドローフロアの取得
/// </summary>
/// <param name="引数無し"></param>
/// <returns>ドローフロアのユニークポインタ</returns>
const std::unique_ptr<Debug::GridFloor>&
SystemManager::GetGridFloor()
{
	if (!m_gridFloor)
	{
		m_gridFloor = std::make_unique<Debug::GridFloor>(50, 50);
	}
	return m_gridFloor;
}

#endif

/// <summary>
/// 一括でシステムのリソースを作成
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SystemManager::CreateUnique()
{
	// デバイスリソース
	m_pDR = DX::DeviceResources::GetInstance();
	auto _device = m_pDR->GetD3DDevice();

	// ３Ｄレンダリング
	m_commonState = std::make_unique<CommonStates>(_device);

	// カメラの初期化
	m_camera = std::make_unique<Camera>();

	// レイを飛ばす
	m_rayCast = std::make_unique<RayCast>();

	// 画像の描画
	m_drawSprite = std::make_unique<DrawSprite>();

	// ベーシックエフェクト
	m_effect = std::make_unique<BasicEffect>(_device);

	// サウンドマネージャ
	m_soundManager = std::make_unique<SoundManager>();

#ifdef _DEBUG

	auto _context = m_pDR->GetD3DDeviceContext();

	// 文字の描画
	m_drawString = std::make_unique<Debug::DrawString>(_device, _context);

	// グリッドフロア 30 x 30
	m_gridFloor = std::make_unique<Debug::GridFloor>(50, 50);
#endif
}