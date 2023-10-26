/*
 *	@File	EditUI.cpp
 *	@Brief	UIの表示。
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../../../Libraries/SystemManager/SystemManager.h"

// ユーザーユーティリティ
#include "Libraries/UserUtility.h"

#include "EditUI.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
EditUI::EditUI(const SimpleMath::Vector2& windowSize)
	: m_system{}					// システムマネージャ
	, m_windowSize{windowSize}		// 画面サイズ
	, m_nowState{}					// 最新のステート
	, m_imageHitter{}				// 当たり判定
	, m_saveTexPos{}				// セーブアイコンの座標
	, m_cameraTexPos{}				// カメラアイコンの座標
	, m_backTexPos{}				// 戻るアイコンの座標
	, m_openTexPos{}				// ファイルアイコンの座標
	, m_boxHover{}					// ホバー時のサイズ
	, is_anyHitFlag{ false }		// ホバーフラグ
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
EditUI::~EditUI()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="shareSystem">システムデータ</param>
/// <param name="device">ID3D11Device1ポインタ</param>
/// <returns>なし</returns>
void EditUI::Initialize(const std::shared_ptr<SystemManager>& shareSystem, ID3D11Device1* device)
{
	// ポインタの共有
	m_system = shareSystem;

	// キー名　：　ファイルパス名
	m_system->GetDrawSprite()->AddTextureData(L"Save",			// セーブアイコン
		L"Resources/Textures/EDITS/SaveFile.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Open",			// ファイルアイコン
		L"Resources/Textures/EDITS/OpenFile.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Camera",		// カメラアイコン(未使用時)
		L"Resources/Textures/EDITS/Camera.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"CameraMove",	// カメラアイコン(使用時)
		L"Resources/Textures/EDITS/CameraMove.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"ToolOn",		// ツール使用切り替えアイコン
		L"Resources/Textures/EDITS/ToolOn.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"ToolOff",		// ツール使用切り替えアイコン
		L"Resources/Textures/EDITS/ToolOff.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"BackSelect",	// セレクトに戻る
		L"Resources/Textures/EDITS/BackSelect.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"ToolBar",		// ツールバー
		L"Resources/Textures/EDITS/EditToolBar.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Grass",			// 草ブロックアイコン
		L"Resources/Textures/BLOCK/GrassIcon.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Cloud",			// 雲ブロックアイコン
		L"Resources/Textures/BLOCK/CloudIcon.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Coin",			// コインブロックアイコン
		L"Resources/Textures/BLOCK/CoinIcon.dds",  device);

	m_system->GetDrawSprite()->AddTextureData(L"Gravity",		// 重力ブロックアイコン
		L"Resources/Textures/BLOCK/ResetIcon.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Player",		// プレイヤーアイコン
		L"Resources/Textures/BLOCK/PlayerIcon.dds",device);

	m_system->GetDrawSprite()->AddTextureData(L"Delete",		// ブロック削除アイコン
		L"Resources/Textures/BLOCK/DeleteIcon.dds",device);

	// IDを格納
	m_texName[MAPSTATE::GRASS]   = L"Grass";
	m_texName[MAPSTATE::CLOUD]   = L"Cloud";
	m_texName[MAPSTATE::COIN]    = L"Coin";
	m_texName[MAPSTATE::GRAVITY] = L"Gravity";
	m_texName[MAPSTATE::PLAYER]  = L"Player";
	m_texName[MAPSTATE::NONE]    = L"Delete";

	// 比率を計算
	float _span = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// 座標情報
	m_openTexPos	   = {  80 * _span , 80 * _span};
	m_saveTexPos       = { 218 * _span , 80 * _span};
	m_cameraTexPos     = { 356 * _span , 80 * _span};
	m_backTexPos	   = { m_windowSize.x - (244 * _span)  ,80 * _span};
	m_toolButtonTexPos = { m_windowSize.x - (96 * _span)  ,80 * _span};

	// 設置ブロックアイコン
	for (int i = 0; i < MAPSTATE::LENGTH; i++)
	{
		m_imagePos[i] = { 545 * _span + (192 * _span * i) , 80 * _span};
		is_boxState[i] = false;
		m_boxHover[i] = 0.0f;
	}

	// ステータスの初期値は草ブロック
	m_nowState = MAPSTATE::GRASS;
	is_boxState[MAPSTATE::GRASS] = true;

	// ツールバーを表示
	is_toolFlag = true;

	// ホバーフラグをリセット
	is_anyHitFlag = false;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="mouseState">マウス</param>
/// <returns>なし</returns>
void EditUI::Update(Mouse::State& mouseState)
{
	// ツールバー表示切り替えアイコンをクリック
	bool _tool = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },		 // マウスの位置
		{ m_toolButtonTexPos.x,m_toolButtonTexPos.y },	 	 // 画像の位置
		SimpleMath::Vector2{ 5.0f }, 						 // サイズ
		SimpleMath::Vector2{ 80.0f });						 // サイズ

	// ツールを表示するフラグを切り替え
	if (_tool && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		is_toolFlag = !is_toolFlag;
	}

	// ツールバーフラグが立っていたら選択可能にする
	if (!is_toolFlag) return;

	// ボックスのアイコン
	ChangeState(mouseState);

	// セレクトに戻るボタンをクリック
	bool _back = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y },		 // マウスの位置
		{ m_backTexPos.x,m_backTexPos.y },	 				 // 画像の位置
		SimpleMath::Vector2{ 5.0f }, 						 // サイズ
		SimpleMath::Vector2{ 80.0f });						 // サイズ

	// ツールを表示するフラグを切り替え
	if (_back && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
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
	bool _camera = m_imageHitter.IsHitAABB2D(
		{ (float)mouseState.x,(float)mouseState.y }, // マウスの位置
		{ m_cameraTexPos.x,m_cameraTexPos.y },	 	 // 画像の位置
		SimpleMath::Vector2{ 5.0f }, 				 // サイズ
		SimpleMath::Vector2{ 100.0f });				 // サイズ

	// カメラ移動モード切り替え
	if (_camera && m_system->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::PRESSED)
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
void EditUI::Render()
{
	// 画像の拡大率をウィンドウをもとに計算
	SimpleMath::Vector2 _imageScale = m_windowSize / FULL_SCREEN_SIZE;

	// ツールフラグがTrueならツールバーを表示する
	if (is_toolFlag)
	{
		// ツールバー
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolBar",
			SimpleMath::Vector2::Zero,				// 座標
			{ 1.0f,1.0f,1.0f,0.7f },				// 色
			_imageScale,							// 拡大率
			SimpleMath::Vector2::Zero
		);

		// ファイルアイコン
		if (is_openFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Open",							// 登録キー
				m_openTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,1.0f },			// 色
				IMAGE_RATE * _imageScale,			// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Open",							// 登録キー
				m_openTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,0.2f },			// 色
				0.5f * _imageScale,					// 拡大率
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
				IMAGE_RATE * _imageScale,			// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Save",							// 登録キー
				m_saveTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,0.2f },			// 色
				0.5f * _imageScale,					// 拡大率
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
				IMAGE_RATE * _imageScale,			// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"Camera",							// 登録キー
				m_cameraTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,0.3f },			// 色
				IMAGE_RATE * _imageScale * 0.9f,	// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);
		}

		// セレクトに戻るボタン
		m_system->GetDrawSprite()->DrawTexture(
				L"BackSelect",						// 登録キー
				m_backTexPos,						// 座標
				{ 1.0f,1.0f,1.0f,1.0f },			// 色
				IMAGE_RATE * _imageScale,			// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
			);

		// ブロックのアイコン
		DrawIcon(_imageScale);
	}

	// ツールバーボタン表示
	if (is_toolFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolOn",								// 登録キー
			m_toolButtonTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },				// 色
			IMAGE_RATE * _imageScale,				// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }			// 中心位置
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolOff",								// 登録キー
			m_toolButtonTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },				// 色
			IMAGE_RATE * _imageScale,				// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }			// 中心位置
		);
	}
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void EditUI::Finalize()
{
}

/// <summary>
/// アイコンの描画
/// </summary>
/// <param name="imageScale">拡大率</param>
/// <returns>なし</returns>
void EditUI::DrawIcon(const SimpleMath::Vector2& imageScale)
{
	for (int i = 0; i < MAPSTATE::LENGTH; ++i)
	{
		if (is_boxState[i])
		{
			m_system->GetDrawSprite()->DrawTexture(
				m_texName[i],
				SimpleMath::Vector2{ m_imagePos[i].x,m_imagePos[i].y + IMAGE_RATE},
				SimpleMath::Vector4::One,
				IMAGE_RATE * imageScale,
				SimpleMath::Vector2{ IMAGE_CENTER }
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				m_texName[i],
				m_imagePos[i],
				{ 1.0f,1.0f,1.0f,HALF },
				(HALF + m_boxHover[i]) * imageScale,
				SimpleMath::Vector2{ IMAGE_CENTER }
			);
		}
	}
}

/// <summary>
/// ステータスの変更
/// </summary>
/// <param name="mouseState">マウス</param>
/// <returns>なし</returns>
void EditUI::ChangeState(DirectX::Mouse::State& mouseState)
{
	// マウスがUIエリア以外なら処理しない
	if (mouseState.y > 170.0f * (m_windowSize.y / FULL_SCREEN_SIZE.y)) return;

	// アイコンごとの初期値
	bool _iconFlags[MAPSTATE::LENGTH] = { false };

	// 各アイコンをクリックしたかどうかを判定し、フラグを立てる
	for (int i = 0; i < MAPSTATE::LENGTH; ++i)
	{
		_iconFlags[i] = m_imageHitter.IsHitAABB2D(
			{ (float)mouseState.x,(float)mouseState.y },// マウスの位置
			m_imagePos[i],                              // 画像の位置
			SimpleMath::Vector2{ 5.0f },                // 最小サイズ
			SimpleMath::Vector2{ 100.0f });             // 最大サイズ

		// 当たっていたらサイズを大きくする(ホバー)
		m_boxHover[i] = _iconFlags[i] == true ? 0.1f : 0.0f;
	}

	// どれか一つ当たっていたら
	is_anyHitFlag = (_iconFlags[MAPSTATE::GRASS]  ||
					 _iconFlags[MAPSTATE::COIN]   ||
					 _iconFlags[MAPSTATE::CLOUD]  ||
					 _iconFlags[MAPSTATE::GRAVITY]  ||
					 _iconFlags[MAPSTATE::PLAYER] ||
					 _iconFlags[MAPSTATE::NONE]);

	// マウスがクリックされた場合に、現在のステータスを更新する
	for (int i = 0; i < MAPSTATE::LENGTH; ++i)
	{
		// 当たっていなければ処理しない
		if (is_anyHitFlag == false) return;

		if (mouseState.leftButton)
		{
			// クリック音
			auto& _sound = m_system->GetSoundManager();
			_sound->SetVolume(_sound->GetVolume(XACT_WAVEBANK_SKBX_SE_ICONTAP) / 2, XACT_WAVEBANK_SKBX_SE_ICONTAP);
			_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);

			// 有効フラグを格納
			is_boxState[i] = _iconFlags[i];

			if (_iconFlags[i])
			{
				switch (i)
				{
				case MAPSTATE::GRASS:
					m_nowState = MAPSTATE::GRASS;		// 草ブロック
					break;
				case MAPSTATE::COIN:
					m_nowState = MAPSTATE::COIN;		// コイン
					break;
				case MAPSTATE::CLOUD:
					m_nowState = MAPSTATE::CLOUD;		// 雲
					break;
				case MAPSTATE::GRAVITY:
					m_nowState = MAPSTATE::GRAVITY;		// リセットポイント
					break;
				case MAPSTATE::PLAYER:
					m_nowState = MAPSTATE::PLAYER;		// プレイヤー
					break;
				case MAPSTATE::NONE:
					m_nowState = MAPSTATE::NONE;		// なし
					break;
				default:
					break;
				}
			}
		}
	}
}
