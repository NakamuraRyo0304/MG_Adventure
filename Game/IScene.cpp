/*
 *	@File	IScene.cpps
 *	@Brief	基底クラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "IScene.h"

// DeviceResources
DX::DeviceResources* 
IScene::GetDeviceResources()
{
	if (!m_pDR)
	{
		m_pDR = DX::DeviceResources::GetInstance();
	}
	return m_pDR;
}

// CommonStates
const std::unique_ptr<DirectX::CommonStates>& 
IScene::GetCommonStates()
{
	if (!m_commonState)
	{
		return nullptr;
	}
	return m_commonState;
}

// KeyboardStateTracker
const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& 
IScene::GetStateTrack()
{
	if (!m_keyboardStateTracker)
	{
		m_keyboardStateTracker
			= std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	}
	return m_keyboardStateTracker;
}

// DrawString
const std::unique_ptr<DrawString>& 
IScene::GetString()
{
	if (!m_drawString)
	{
		return nullptr;
	}
	return m_drawString;
}

// MouseTracker
const std::unique_ptr<DirectX::Mouse::ButtonStateTracker>& 
IScene::GetMouseTrack()
{
	if (!m_mouseStateTracker)
	{
		m_mouseStateTracker
			= std::make_unique<DirectX::Mouse::ButtonStateTracker>();
	}
	return m_mouseStateTracker;
}

// GridFloor
const std::unique_ptr<GridFloor>&
IScene::GetGridFloor()
{
	if (!m_gridFloor)
	{
		return nullptr;
	}
	return m_gridFloor;
}

// RayCast
const std::unique_ptr<RayCast>& 
IScene::GetRayCast()
{
	if (!m_rayCast)
	{
		m_rayCast = std::make_unique<RayCast>();
	}
	return m_rayCast;
}

//--------------------------------------------------------//
//モデルを作成するファクトリー                            //
//--------------------------------------------------------//
// 第１引数：デバイス 第２引数：ファイルの名前
std::unique_ptr<DirectX::DX11::Model> IScene::ModelFactory(
	ID3D11Device1* device,
	const wchar_t* filename)
{
	// モデルの作成(作成時のみの使い捨て)
	std::unique_ptr<DirectX::EffectFactory> fx =
		std::make_unique<DirectX::EffectFactory>(device);

	// ディレクトリの設定
	fx->SetDirectory(L"Resources/Models");

	// モデルデータの読み込み
	std::unique_ptr<DirectX::DX11::Model> model = DirectX::Model::CreateFromCMO(
		device,
		filename,
		*fx
	);

	// 作成したモデルを返却
	return model;
}

//--------------------------------------------------------//
//メイクユニーク                                          //
//--------------------------------------------------------//
void IScene::CreateUnique(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// デバイスリソース
	m_pDR		  = DX::DeviceResources::GetInstance();

	// ３Ｄレンダリング
	m_commonState = std::make_unique<DirectX::CommonStates>(device);
	
	// キーボード
	m_keyboardStateTracker
		          = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	// 文字の描画
	m_drawString  = std::make_unique<DrawString>();

	// カメラの初期化
	m_camera      = new Camera();

	// マウス
	m_mouseStateTracker
				  = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	// グリッドフロア 30 x 30
	m_gridFloor   = std::make_unique<GridFloor>(device, context, 30, 30);

	// レイを飛ばす
	m_rayCast     = std::make_unique<RayCast>();

}
