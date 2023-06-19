/*
 *	@File	UserInterface.cpp
 *	@Brief	UIの表示。
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "UserInterface.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
UserInterface::UserInterface():
	m_systemManager{},				// システムマネージャ
	m_aabbCol{},					// 当たり判定
	m_saveTexPos{},					// 画像座標
	m_cameraTexPos{},
	m_penTexPos{},
	m_openTexPos{},
	is_saveFlag{},					// 保存フラグ
	is_openFlag{},					// 開くフラグ
	is_cameraFlag{ true },			// カメラモードONでスタート
	is_drawFlag{ true }				// ペンモードでスタート		

{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
UserInterface::~UserInterface()
{
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
void UserInterface::Initialize(std::shared_ptr<SystemManager> shareSystem,ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// ポインタの共有
	m_systemManager = shareSystem;

	// 画像の設定
	m_systemManager->GetDrawSprite()->MakeSpriteBatch(context);
	// キー名　：　ファイルパス名　：　デバイス
	m_systemManager->GetDrawSprite()->AddTextureData(L"Save", L"Resources/Textures/SaveFile.dds", device);
	m_systemManager->GetDrawSprite()->AddTextureData(L"Camera", L"Resources/Textures/Camera.dds", device);
	m_systemManager->GetDrawSprite()->AddTextureData(L"CameraMove", L"Resources/Textures/CameraMove.dds", device);
	m_systemManager->GetDrawSprite()->AddTextureData(L"Pen", L"Resources/Textures/AddBlock.dds", device);
	m_systemManager->GetDrawSprite()->AddTextureData(L"Erase", L"Resources/Textures/EraseBlock.dds", device);
	m_systemManager->GetDrawSprite()->AddTextureData(L"Open", L"Resources/Textures/OpenFile.dds", device);

	// 座標情報
	m_openTexPos = { 80 , 80 };
	m_saveTexPos = { 208  , 80 };
	m_cameraTexPos = { 336 , 80 };
	m_penTexPos = { 464, 80 };
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
void UserInterface::Update(Mouse::State& mouseState)
{
	// ファイルを開くアイコン
	bool open = false;
	open = m_aabbCol.HitAABB_2D({ (float)mouseState.x,(float)mouseState.y },// マウスの位置
		{ m_openTexPos.x,m_openTexPos.y },		    // 画像の位置
		SimpleMath::Vector2{ 5.0f },		    // サイズ
		SimpleMath::Vector2{ 100.0f });	    // サイズ
	if (open)
	{
		is_openFlag = true;
	}
	else
	{
		is_openFlag = false;
	}


	// 保存アイコンをクリック
	bool save = false;
	save = m_aabbCol.HitAABB_2D({ (float)mouseState.x,(float)mouseState.y },// マウスの位置
		{ m_saveTexPos.x,m_saveTexPos.y },		    // 画像の位置
		SimpleMath::Vector2{ 5.0f },		    // サイズ
		SimpleMath::Vector2{ 100.0f });	    // サイズ
	// 当っていてクリックした時処理
	if (save)
	{
		is_saveFlag = true;
	}
	else
	{
		is_saveFlag = false;
	}

	// カメラアイコンをクリック
	bool camera = m_aabbCol.HitAABB_2D({ (float)mouseState.x,(float)mouseState.y },// マウスの位置
		{ m_cameraTexPos.x,m_cameraTexPos.y },	 									// 画像の位置
		SimpleMath::Vector2{ 5.0f }, 										// サイズ
		SimpleMath::Vector2{ 100.0f });									// サイズ

	// カメラ移動モード切り替え
	if (camera && m_systemManager->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		is_cameraFlag = !is_cameraFlag;
		m_systemManager->GetCamera()->SetEagleMode(is_cameraFlag);
	}

	// ペン/消しゴムアイコンをクリック
	bool tool = m_aabbCol.HitAABB_2D({ (float)mouseState.x,(float)mouseState.y },// マウスの位置
		{ m_penTexPos.x,m_penTexPos.y },	        							 // 画像の位置
		SimpleMath::Vector2{ 5.0f },									 // サイズ
		SimpleMath::Vector2{ 100.0f });								 // サイズ

	// 描画モード切り替え
	if (tool && m_systemManager->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		is_drawFlag = !is_drawFlag;
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void UserInterface::Render()
{
	// セーブアイコン
	if (is_openFlag)
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Open",							// 登録キー
			m_openTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			0.55f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Open",							// 登録キー
			m_openTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,0.3f },			// 色
			0.5f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// セーブアイコン
	if (is_saveFlag)
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Save",							// 登録キー
			m_saveTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			0.55f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Save",							// 登録キー
			m_saveTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,0.3f },			// 色
			0.5f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// カメラアイコン
	if (is_cameraFlag)
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"CameraMove",						// 登録キー
			m_cameraTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			0.55,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Camera",							// 登録キー
			m_cameraTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,0.3f },			// 色
			0.5f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// ペンアイコン
	if (is_drawFlag)
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Pen",								// 登録キー
			m_penTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			0.55f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_systemManager->GetDrawSprite()->DrawTexture(
			L"Erase",							// 登録キー
			m_penTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			0.5f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
}
