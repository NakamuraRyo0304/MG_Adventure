/*
 *	@File	UserInterface.cpp
 *	@Brief	UIの表示。
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../../../Libraries/SystemManager/SystemManager.h"

#include "UserInterface.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
UserInterface::UserInterface(const DirectX::SimpleMath::Vector2& windowSize):
	m_system{},						// システムマネージャ
	m_windowSize{windowSize},		// 画面サイズ
	m_aabbCol{},					// 当たり判定
	m_saveTexPos{},					// 画像座標
	m_cameraTexPos{},				// 画像の位置
	m_openTexPos{},					// |
	is_saveFlag{},					// 保存フラグ
	is_openFlag{},					// 開くフラグ
	is_cameraFlag{ true },			// カメラモードONでスタート
	is_boxState{ false }			// 画像フラグ
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
	m_system->GetDrawSprite()->AddTextureData(L"Save", L"Resources/Textures/SaveFile.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Open", L"Resources/Textures/OpenFile.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Camera", L"Resources/Textures/Camera.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"CameraMove", L"Resources/Textures/CameraMove.dds", device);

	// 背景の帯
	m_system->GetDrawSprite()->AddTextureData(L"ToolBar", L"Resources/Textures/EditToolBar.dds", device);

	// ブロックアイコン
	m_system->GetDrawSprite()->AddTextureData(L"Grass", L"Resources/Textures/BLOCK/GrassIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Clowd", L"Resources/Textures/BLOCK/ClowdIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Coin", L"Resources/Textures/BLOCK/CoinIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"ReClowd", L"Resources/Textures/BLOCK/ReClowdIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Player", L"Resources/Textures/BLOCK/PlayerIcon.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Erase", L"Resources/Textures/EraseBlock.dds", device);

	// IDを格納
	m_texName[MapState::GrassBox]   = L"Grass";
	m_texName[MapState::ClowdBox]   = L"Clowd";
	m_texName[MapState::CoinBox]    = L"Coin";
	m_texName[MapState::ResetClowd] = L"ReClowd";
	m_texName[MapState::PlayerPos]  = L"Player";
	m_texName[MapState::None]       = L"Erase";

	// 比率を計算
	float span = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// 座標情報
	m_openTexPos	= {  80 * span , 80 * span};
	m_saveTexPos    = { 208 * span , 80 * span};
	m_cameraTexPos  = { 336 * span , 80 * span};
	for (int i = 0; i < MapState::LENGTH; i++)
	{
		m_imagePos[i] = { 464 + (192 * span * i) , 80 * span};
		is_boxState[i] = false;
	}

	// ステータスの初期値は草ブロック
	m_nowState = MapState::GrassBox;
	is_boxState[MapState::GrassBox] = true;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="mouseState">マウス</param>
/// <returns>なし</returns>
void UserInterface::Update(Mouse::State& mouseState)
{
	// ボックスのアイコン
	ChangeState(mouseState);

	// ファイルを開くアイコン
	bool open = false;
	open = m_aabbCol.HitAABB_2D(
		{ (float)mouseState.x,(float)mouseState.y },// マウスの位置
		{ m_openTexPos.x,m_openTexPos.y },		    // 画像の位置
		SimpleMath::Vector2{ 5.0f },			    // サイズ
		SimpleMath::Vector2{ 100.0f });				// サイズ
	if (open)
	{
		is_openFlag = true;
	}
	else
	{
		is_openFlag = false;
	}


	// ファイルを保存するアイコン
	bool save = false;
	save = m_aabbCol.HitAABB_2D(
		{ (float)mouseState.x,(float)mouseState.y },// マウスの位置
		{ m_saveTexPos.x,m_saveTexPos.y },			// 画像の位置
		SimpleMath::Vector2{ 5.0f },				// サイズ
		SimpleMath::Vector2{ 100.0f });				// サイズ
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
	bool camera = m_aabbCol.HitAABB_2D(
		{ (float)mouseState.x,(float)mouseState.y }, // マウスの位置
		{ m_cameraTexPos.x,m_cameraTexPos.y },	 	 // 画像の位置
		SimpleMath::Vector2{ 5.0f }, 				 // サイズ
		SimpleMath::Vector2{ 100.0f });				 // サイズ

	// カメラ移動モード切り替え
	if (camera && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
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
	float imageScale = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

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
			{ 1.0f,1.0f,1.0f,1.0f},				// 色
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
			IMAGE_RATE* imageScale,				// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Camera",							// 登録キー
			m_cameraTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,0.3f },			// 色
			IMAGE_RATE* imageScale,				// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// ブロックのアイコン
	DrawIcon(imageScale);
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
void UserInterface::DrawIcon(const float& imageScale)
{
	// 草ブロック
	if (is_boxState[MapState::GrassBox])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Grass",							// 登録キー
			m_imagePos[MapState::GrassBox],		// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Grass",							// 登録キー
			m_imagePos[MapState::GrassBox],		// 座標
			{ 1.0f,1.0f,1.0f,0.5f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// 雲ブロック
	if (is_boxState[MapState::ClowdBox])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Clowd",							// 登録キー
			m_imagePos[MapState::ClowdBox],		// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Clowd",							// 登録キー
			m_imagePos[MapState::ClowdBox],		// 座標
			{ 1.0f,1.0f,1.0f,0.5f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// コインブロック
	if (is_boxState[MapState::CoinBox])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Coin",							// 登録キー
			m_imagePos[MapState::CoinBox],		// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Coin",							// 登録キー
			m_imagePos[MapState::CoinBox],		// 座標
			{ 1.0f,1.0f,1.0f,0.5f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// 雲リセットブロック
	if (is_boxState[MapState::ResetClowd])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ReClowd",							// 登録キー
			m_imagePos[MapState::ResetClowd],	// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ReClowd",							// 登録キー
			m_imagePos[MapState::ResetClowd],	// 座標
			{ 1.0f,1.0f,1.0f,0.5f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// プレイヤーブロック
	if (is_boxState[MapState::PlayerPos])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Player",							// 登録キー
			m_imagePos[MapState::PlayerPos],	// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Player",							// 登録キー
			m_imagePos[MapState::PlayerPos],	// 座標
			{ 1.0f,1.0f,1.0f,0.5f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// 消しゴム
	if (is_boxState[MapState::None])
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Erase",							// 登録キー
			m_imagePos[MapState::None],			// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Erase",							// 登録キー
			m_imagePos[MapState::None],			// 座標
			{ 1.0f,1.0f,1.0f,0.5f },			// 色
			IMAGE_RATE * imageScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
}

/// <summary>
/// ステータスの変更
/// </summary>
/// <param name="mouseState">マウス</param>
/// <returns>なし</returns>
void UserInterface::ChangeState(DirectX::Mouse::State& mouseState)
{
	// アイコンごとの初期値
	bool iconFlags[MapState::LENGTH] = { false };

	// 各アイコンをクリックしたかどうかを判定し、フラグを立てる
	for (int i = 0; i < MapState::LENGTH; ++i)
	{
		iconFlags[i] = m_aabbCol.HitAABB_2D(
			{ (float)mouseState.x,(float)mouseState.y },// マウスの位置
			m_imagePos[i],                              // 画像の位置
			SimpleMath::Vector2{ 5.0f },                // サイズ
			SimpleMath::Vector2{ 100.0f });             // サイズ
	}

	// マウスがクリックされた場合に、現在のステータスを更新する
	if (mouseState.leftButton)
	{
		for (int i = 0; i < MapState::LENGTH; ++i)
		{
			is_boxState[i] = iconFlags[i];
			if (iconFlags[i])
			{
				switch (i)
				{
				case MapState::GrassBox:
					m_nowState = MapState::GrassBox;
					break;
				case MapState::CoinBox:
					m_nowState = MapState::CoinBox;
					break;
				case MapState::ClowdBox:
					m_nowState = MapState::ClowdBox;
					break;
				case MapState::ResetClowd:
					m_nowState = MapState::ResetClowd;
					break;
				case MapState::PlayerPos:
					m_nowState = MapState::PlayerPos;
					break;
				case MapState::None:
					m_nowState = MapState::None;
					break;
				default:
					break;
				}
			}
		}
	}
}
