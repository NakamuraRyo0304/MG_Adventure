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
	: m_commonState{nullptr}
	, m_drawSprite{nullptr}
	, m_camera{nullptr}
	, m_pDR{nullptr}
	, m_rayCast{nullptr}
	, m_gridFloor{nullptr}
	, m_drawString{nullptr}
{
}

// デストラクタ
SystemManager::~SystemManager()
{

}


// デバイスリソースの取得
DX::DeviceResources* SystemManager::GetDeviceResources()
{
	if (not m_pDR)
	{
		m_pDR = DX::DeviceResources::GetInstance();
	}
	return m_pDR;
}

// コモンステートの取得
const std::unique_ptr<CommonStates>& SystemManager::GetCommonStates()
{
	if (not m_commonState)
	{
		m_commonState = std::make_unique<CommonStates>(GetDeviceResources()->GetD3DDevice());
	}
	return m_commonState;
}

// レイキャストの取得
const std::unique_ptr<RayCast>& SystemManager::GetRayCast()
{
	if (not m_rayCast)
	{
		m_rayCast = std::make_unique<RayCast>();
	}
	return m_rayCast;
}

// ドロースプライトの取得
const std::unique_ptr<DrawSprite>& SystemManager::GetDrawSprite()
{
	if (not m_drawSprite)
	{
		m_drawSprite = std::make_unique<DrawSprite>();
		m_drawSprite->MakeSpriteBatch();
	}
	return m_drawSprite;
}

// カメラの取得
const std::unique_ptr<Camera>& SystemManager::GetCamera()
{
	if (not m_camera)
	{
		m_camera = std::make_unique<Camera>();
	}
	return m_camera;
}

// サウンドマネージャの取得
const std::unique_ptr<SoundManager>& SystemManager::GetSoundManager()
{
	if (not m_soundManager)
	{
		m_soundManager = std::make_unique<SoundManager>();
	}
	return m_soundManager;
}

// ドローストリングの取得
const std::unique_ptr<Debug::DrawString>& SystemManager::GetString()
{
	if (not m_drawString)
	{
		m_pDR = DX::DeviceResources::GetInstance();
		m_drawString = std::make_unique<Debug::DrawString>(m_pDR->GetD3DDevice(), m_pDR->GetD3DDeviceContext());
	}
	return m_drawString;
}

// グリッドフロアの取得
const std::unique_ptr<Debug::GridFloor>& SystemManager::GetGridFloor()
{
	if (not m_gridFloor)
	{
		m_gridFloor = std::make_unique<Debug::GridFloor>(50, 50);
	}
	return m_gridFloor;
}

// 全てのリソースを作成
void SystemManager::CreateSystem()
{
	// デバイスリソース
	m_pDR = DX::DeviceResources::GetInstance();
	auto _device = m_pDR->GetD3DDevice();
	auto _context = m_pDR->GetD3DDeviceContext();

	// ３Ｄレンダリング
	m_commonState = std::make_unique<CommonStates>(_device);

	// カメラの初期化
	m_camera = std::make_unique<Camera>();

	// レイを飛ばす
	m_rayCast = std::make_unique<RayCast>();

	// 画像の描画
	m_drawSprite = std::make_unique<DrawSprite>();

	// サウンドマネージャ
	m_soundManager = std::make_unique<SoundManager>();

	// 文字の描画
	m_drawString = std::make_unique<Debug::DrawString>(_device, _context);

	// グリッドフロア
	m_gridFloor = std::make_unique<Debug::GridFloor>(50, 50);
}