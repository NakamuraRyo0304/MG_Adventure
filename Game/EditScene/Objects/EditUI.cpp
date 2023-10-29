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
	m_system->GetDrawSprite()->AddTextureData(L"ToolBar",		// ツールバー
		L"Resources/Textures/EDITS/EditToolBar.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"IconPack",		// アイコンパック
		L"Resources/Textures/EDITS/IconPack.dds",device);

	m_system->GetDrawSprite()->AddTextureData(L"ModePack",		// モードパック
		L"Resources/Textures/EDITS/ToolModes.dds",device);

	m_system->GetDrawSprite()->AddTextureData(L"SavePack",		// セーブパック
		L"Resources/Textures/EDITS/SavePack.dds", device);

	// 切り取り位置を指定
	m_texRect[MAPSTATE::NONE]    = { _0,_1,_1,_2 };		// ブロックアイコン
	m_texRect[MAPSTATE::GRASS]   = { _0,_0,_1,_1 };
	m_texRect[MAPSTATE::CLOUD]   = { _1,_0,_2,_1 };
	m_texRect[MAPSTATE::COIN]    = { _2,_0,_3,_1 };
	m_texRect[MAPSTATE::GRAVITY] = { _2,_1,_3,_2 };
	m_texRect[MAPSTATE::PLAYER]  = { _1,_1,_2,_2 };
	m_modeRect[0] = { _0,_0,_1,_1 };					// 表示切替・戻るアイコン
	m_modeRect[1] = { _1,_0,_2,_1 };
	m_modeRect[2] = { _2,_0,_3,_1 };
	m_saveRect[0] = { _0,_0,_1,_1 };					// カメラとファイルアイコン
	m_saveRect[1] = { _1,_0,_2,_1 };
	m_saveRect[2] = { _2,_0,_3,_1 };
	m_saveRect[3] = { _3,_0,_4,_1 };

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
	SimpleMath::Vector2 _rate = m_windowSize / FULL_SCREEN_SIZE;

	// ツールフラグがTrueならツールバーを表示する
	if (is_toolFlag)
	{
		// ツールバー
		m_system->GetDrawSprite()->DrawTexture(
			L"ToolBar",						// 登録キー
			SimpleMath::Vector2::Zero,		// 座標
			{ 1.0f,1.0f,1.0f,0.7f },		// 色
			_rate,							// 拡大率
			SimpleMath::Vector2::Zero		// 中心位置
		);

		// ファイルアイコン
		if (is_openFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"SavePack",					// 登録キー
				m_openTexPos,					// 座標
				{ 1.0f,1.0f,1.0f,1.0f },		// 色
				IMAGE_RATE * _rate,				// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER },	// 中心位置
				m_saveRect[0]					// 開くアイコン
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"SavePack",					// 登録キー
				m_openTexPos,					// 座標
				{ 1.0f,1.0f,1.0f,0.2f },		// 色
				0.5f * _rate,					// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER },	// 中心位置
				m_saveRect[0]					// 開くアイコン
			);
		}

		// セーブアイコン
		if (is_saveFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"SavePack",					// 登録キー
				m_saveTexPos,					// 座標
				{ 1.0f,1.0f,1.0f,1.0f },		// 色
				IMAGE_RATE * _rate,				// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER },	// 中心位置
				m_saveRect[1]					// 保存アイコン
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"SavePack",					// 登録キー
				m_saveTexPos,					// 座標
				{ 1.0f,1.0f,1.0f,0.2f },		// 色
				0.5f * _rate,					// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER },	// 中心位置
				m_saveRect[1]					// 保存アイコン
			);
		}

		// カメラアイコン
		if (is_cameraFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"SavePack",					// 登録キー
				m_cameraTexPos,					// 座標
				{ 1.0f,1.0f,1.0f,1.0f },		// 色
				IMAGE_RATE * _rate,				// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER },	// 中心位置
				m_saveRect[2]					// カメラ使用中アイコン
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"SavePack",					// 登録キー
				m_cameraTexPos,					// 座標
				{ 1.0f,1.0f,1.0f,0.3f },		// 色
				0.5f * _rate,					// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER },	// 中心位置
				m_saveRect[3]					// カメラ未使用中アイコン
			);
		}

		// セレクトに戻るボタン
		m_system->GetDrawSprite()->DrawTexture(
				L"ModePack",					// 登録キー
				m_backTexPos,					// 座標
				{ 1.0f,1.0f,1.0f,1.0f },		// 色
				IMAGE_RATE * _rate,				// 拡大率
				{ IMAGE_CENTER,IMAGE_CENTER },	// 中心位置
				m_modeRect[2]					// Backの切り取り位置
			);
	}

	// ブロックのアイコン
	if (is_toolFlag)
	{
		DrawIcon(_rate);
	}

	// ツールバーボタン表示
	m_system->GetDrawSprite()->DrawTexture(
		L"ModePack",					// 登録キー
		m_toolButtonTexPos,				// 座標
		{ 1.0f,1.0f,1.0f,1.0f },		// 色
		IMAGE_RATE * _rate,				// 拡大率
		{ IMAGE_CENTER,IMAGE_CENTER },	// 中心位置
		m_modeRect[is_toolFlag ? 0 : 1]	// OnOffの切り取り位置
	);
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
				L"IconPack",
				SimpleMath::Vector2{ m_imagePos[i].x,m_imagePos[i].y + IMAGE_RATE },
				SimpleMath::Vector4::One,
				IMAGE_RATE * imageScale,
				SimpleMath::Vector2{ IMAGE_CENTER },
				m_texRect[i]
			);
		}
		else
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"IconPack",
				m_imagePos[i],
				{ 1.0f,1.0f,1.0f,HALF },
				(HALF + m_boxHover[i]) * imageScale,
				SimpleMath::Vector2{ IMAGE_CENTER },
				m_texRect[i]
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
	if (mouseState.y > BAR_HEIGHT * (m_windowSize.y / FULL_SCREEN_SIZE.y)) return;

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
					 _iconFlags[MAPSTATE::GRAVITY]||
					 _iconFlags[MAPSTATE::PLAYER] ||
					 _iconFlags[MAPSTATE::NONE]);

	// マウスがクリックされた場合に、現在のステータスを更新する
	for (int i = 0; i < MAPSTATE::LENGTH; ++i)
	{
		// 当たっていなければ処理しない
		if (not is_anyHitFlag) return;

		if (not mouseState.leftButton) return;

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
