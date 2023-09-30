/*
 *	@File	UserInterface.cpp
 *	@Brief	UIの表示。
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../../../Libraries/SystemManager/SystemManager.h"

// ユーザーユーティリティ
#include "Libraries/UserUtility.h"

#include "UserInterface.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
UserInterface::UserInterface(const SimpleMath::Vector2& windowSize)
	: m_system{}					// システムマネージャ
	, m_windowSize{windowSize}		// 画面サイズ
	, m_nowState{}					// 最新のステート
	, m_imageHitter{}				// 当たり判定
	, m_saveTexPos{}				// セーブアイコンの座標
	, m_cameraTexPos{}				// カメラアイコンの座標
	, m_backTexPos{}				// 戻るアイコンの座標
	, m_openTexPos{}				// ファイルアイコンの座標
	, m_boxHover{}					// ホバー時のサイズ
	, is_boxState{ false }			// 画像フラグ
	, is_saveFlag{ false }			// 保存フラグ
	, is_openFlag{ false }			// 開くフラグ
	, is_cameraFlag{ true }			// カメラモードONでスタート
	, is_backFlag{ false }			// セレクトに戻るフラグ
	, is_toolFlag{ true }			// ツールバーを表示するフラグ
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
UserInterface::~UserInterface()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="shareSystem">システムデータ</param>
/// <param name="context">ID3D11DeviceContext1ポインタ</param>
/// <param name="device">ID3D11Device1ポインタ</param>
/// <returns>なし</returns>
void UserInterface::Initialize(std::shared_ptr<SystemManager> shareSystem,
	ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// ポインタの共有
	m_system = shareSystem;

	// 画像の設定
	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// キー名　：　ファイルパス名　：　デバイス
	// 操作アイコン
	m_system->GetDrawSprite()->AddTextureData(L"Save",      L"Resources/Textures/SaveFile.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Open",      L"Resources/Textures/OpenFile.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Camera",    L"Resources/Textures/Camera.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"CameraMove",L"Resources/Textures/CameraMove.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"ToolOn",	L"Resources/Textures/ToolOn.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"ToolOff",	L"Resources/Textures/ToolOff.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"BackSelect",L"Resources/Textures/BackSelect.dds", device);

	// 背景の帯
	m_system->GetDrawSprite()->AddTextureData(L"ToolBar",   L"Resources/Textures/EditToolBar.dds", device);

	// ブロックアイコン
	m_system->GetDrawSprite()->AddTextureData(L"Grass",     L"Resources/Textures/BLOCK/GrassIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Cloud",     L"Resources/Textures/BLOCK/ClowdIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Coin",      L"Resources/Textures/BLOCK/CoinIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"ReCloud",   L"Resources/Textures/BLOCK/ReClowdIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Player",    L"Resources/Textures/BLOCK/PlayerIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Erase",     L"Resources/Textures/BLOCK/DeleteIcon.dds", device);

	// IDを格納
	m_texName[MapState::GRASS]   = L"Grass";
	m_texName[MapState::CLOUD]   = L"Cloud";
	m_texName[MapState::COIN]    = L"Coin";
	m_texName[MapState::RESET] = L"ReCloud";
	m_texName[MapState::PLAYER]  = L"Player";
	m_texName[MapState::NONE]       = L"Erase";

	// 比率を計算
	float span = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// 座標情報
	m_openTexPos	   = {  80 * span , 80 * span};
	m_saveTexPos       = { 218 * span , 80 * span};
	m_cameraTexPos     = { 356 * span , 80 * span};
	m_backTexPos	   = { m_windowSize.x - (244 * span)  ,80 * span};
	m_toolButtonTexPos = { m_windowSize.x - (96 * span)  ,80 * span};
	for (int i = 0; i < MapState::LENGTH; i++)
	{
		m_imagePos[i] = { 528 * span + (192 * span * i) , 80 * span};
		is_boxState[i] = false;
		m_boxHover[i] = 0.0f;
	}

	// ステータスの初期値は草ブロック
	m_nowState = MapState::GRASS;
	is_boxState[MapState::GRASS] = true;

	// ツールバーを表示
	is_toolFlag = true;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="mouseState">マウス</param>
/// <returns>なし</returns>
void UserInterface::Update(Mouse::State& mouseState)
{
	// ツールバー表示切り替えアイコンをクリック
	bool tool = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },		 // マウスの位置
		{ m_toolButtonTexPos.x,m_toolButtonTexPos.y },	 	 // 画像の位置
		SimpleMath::Vector2{ 5.0f }, 						 // サイズ
		SimpleMath::Vector2{ 80.0f });						 // サイズ

	// ツールを表示するフラグを切り替え
	if (tool && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_toolFlag = !is_toolFlag;
	}

	// ツールバーフラグが立っていたら選択可能にする
	if (!is_toolFlag) return;

	// ボックスのアイコン
	ChangeState(mouseState);

	// セレクトに戻るボタンをクリック
	bool back = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },		 // マウスの位置
		{ m_backTexPos.x,m_backTexPos.y },	 				 // 画像の位置
		SimpleMath::Vector2{ 5.0f }, 						 // サイズ
		SimpleMath::Vector2{ 80.0f });						 // サイズ

	// ツールを表示するフラグを切り替え
	if (back && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_backFlag = true;
	}

	// ファイルを開くアイコン
	is_openFlag = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },// マウスの位置
		{ m_openTexPos.x,m_openTexPos.y },		    // 画像の位置
		SimpleMath::Vector2{ 5.0f },			    // サイズ
		SimpleMath::Vector2{ 100.0f });				// サイズ

	// ファイルを保存するアイコン
	is_saveFlag = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },// マウスの位置
		{ m_saveTexPos.x,m_saveTexPos.y },			// 画像の位置
		SimpleMath::Vector2{ 5.0f },				// サイズ
		SimpleMath::Vector2{ 100.0f });				// サイズ

	// カメラアイコンをクリック
	bool camera = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y }, // マウスの位置
		{ m_cameraTexPos.x,m_cameraTexPos.y },	 	 // 画像の位置
		SimpleMath::Vector2{ 5.0f }, 				 // サイズ
		SimpleMath::Vector2{ 100.0f });				 // サイズ

	// カメラ移動モード切り替え
	if (camera && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_cameraFlag = !is_cameraFlag;
		m_system->GetCamera()->SetEagleMode(is_cameraFlag);
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void UserInterface::Render()
{
	// 画像の拡大率をウィンドウをもとに計算
	SimpleMath::Vector2 imageScale = m_windowSize / FULL_SCREEN_SIZE;

	// ツールフラグがTrueならツールバーを表示する
	if (is_toolFlag)
	{
		// ツールバー
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolBar",
			SimpleMath::Vector2::Zero,			// 座標
			{ 1.0f,1.0f,1.0f,0.7f },			// 色
			imageScale,							// 拡大率
			SimpleMath::Vector2::Zero
		);

		// ファイルアイコン
		if (is_openFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Open",							// 登録キー
				m_openTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,1.0f },			// 色
				IMAGE_RATE * imageScale,			// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Open",							// 登録キー
				m_openTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,0.2f },			// 色
				0.5f * imageScale,					// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);
		}

		// セーブアイコン
		if (is_saveFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Save",							// 登録キー
				m_saveTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,1.0f },			// 色
				IMAGE_RATE * imageScale,			// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Save",							// 登録キー
				m_saveTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,0.2f },			// 色
				0.5f * imageScale,					// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);
		}

		// カメラアイコン
		if (is_cameraFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"CameraMove",						// 登録キー
				m_cameraTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,1.0f },			// 色
				IMAGE_RATE * imageScale,			// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Camera",							// 登録キー
				m_cameraTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,0.3f },			// 色
				IMAGE_RATE * imageScale * 0.9f,		// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);
		}

		// セレクトに戻るボタン
		m_system->GetDrawSprite()->DrawTexture(
				L"BackSelect",
				m_backTexPos,
				{ 1.0f,1.0f,1.0f,1.0f },
				IMAGE_RATE * imageScale,
				{ IMAGE_CENTER,IMAGE_CENTER }
			);

		// ブロックのアイコン
		DrawIcon(imageScale);
	}

	// ツールバーボタン表示
	if (is_toolFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolOn",							// 登録キー
			m_toolButtonTexPos,					// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolOff",							// 登録キー
			m_toolButtonTexPos,					// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void UserInterface::Finalize()
{
}

/// <summary>
/// アイコンの描画
/// </summary>
/// <param name="imageScale">拡大率</param>
/// <returns>なし</returns>
void UserInterface::DrawIcon(const SimpleMath::Vector2& imageScale)
{
	for (int idx = 0; idx < MapState::LENGTH; ++idx)
	{
		if (is_boxState[idx])
		{
			m_system->GetDrawSprite()->DrawTexture(
				m_texName[idx],							// 登録キー
				m_imagePos[idx],						// 座標
				SimpleMath::Vector4::One,				// 色
				IMAGE_RATE * imageScale,				// 拡大率
				SimpleMath::Vector2{ IMAGE_CENTER }		// 中心位置
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				m_texName[idx],							// 登録キー
				m_imagePos[idx],						// 座標
				{ 1.0f,1.0f,1.0f,HALF },				// 色
				(HALF + m_boxHover[idx]) * imageScale,	// 拡大率
				SimpleMath::Vector2{ IMAGE_CENTER }		// 中心位置
			);
		}
	}
}

/// <summary>
/// ステータスの変更
/// </summary>
/// <param name="mouseState">マウス</param>
/// <returns>なし</returns>
void UserInterface::ChangeState(DirectX::Mouse::State& mouseState)
{
	// マウスがUIエリア以外なら処理しない
	if (mouseState.y > 170.0f * (m_windowSize.y / FULL_SCREEN_SIZE.y)) return;

	// アイコンごとの初期値
	bool iconFlags[MapState::LENGTH] = { false };

	// 各アイコンをクリックしたかどうかを判定し、フラグを立てる
	for (int i = 0; i < MapState::LENGTH; ++i)
	{
		iconFlags[i] = m_imageHitter.IsHitAABB2D(
			{ (float)mouseState.x,(float)mouseState.y },// マウスの位置
			m_imagePos[i],                              // 画像の位置
			SimpleMath::Vector2{ 5.0f },                // 最小サイズ
			SimpleMath::Vector2{ 100.0f });             // 最大サイズ

		// 当たっていたらサイズを大きくする(ホバー)
		m_boxHover[i] = iconFlags[i] == true ? 0.1f : 0.0f;
	}


	// マウスがクリックされた場合に、現在のステータスを更新する
	if (mouseState.leftButton)
	{
		for (int i = 0; i < MapState::LENGTH; ++i)
		{
			// 有効フラグを格納
			is_boxState[i] = iconFlags[i];

			if (iconFlags[i])
			{
				switch (i)
				{
				case MapState::GRASS:
					m_nowState = MapState::GRASS;		// 草ブロック
					break;
				case MapState::COIN:
					m_nowState = MapState::COIN;			// コイン
					break;
				case MapState::CLOUD:
					m_nowState = MapState::CLOUD;		// 雲
					break;
				case MapState::RESET:
					m_nowState = MapState::RESET;		// リセットポイント
					break;
				case MapState::PLAYER:
					m_nowState = MapState::PLAYER;		// プレイヤー
					break;
				case MapState::NONE:
					m_nowState = MapState::NONE;			// 空気
					break;
				default:
					break;
				}
			}
		}
	}
}
