/*
 *	@File	PlayUI.cpp
 *	@Brief	プレイシーンのUI表示。
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#include "pch.h"

// システムマネージャ
#include "../../../Libraries/SystemManager/SystemManager.h"

// ユーザーユーティリティ
#include "../../../Libraries/UserUtility.h"

#include "PlayUI.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
PlayUI::PlayUI(const SimpleMath::Vector2& windowSize)
	: m_system{}								// システムマネージャ
	, m_windowSize{ windowSize }				// ウィンドウサイズ
	, m_gameTimer{0}							// ゲームのタイマー
    , m_oneSecPos{SimpleMath::Vector2::Zero}	// 1秒の座標
    , m_tenSecPos{SimpleMath::Vector2::Zero}	// 10秒の座標
	, m_countDownPos{SimpleMath::Vector2::Zero}	// ３カウントダウンの座標
	, m_underFontPos{SimpleMath::Vector2::Zero}	// 下の文字ロールの座標
	, m_sunPos{SimpleMath::Vector2::Zero}		// 太陽の画像の座標
	, is_effectFlag{false}						// エフェクトの表示フラグ
	, is_helpFlag{false}						// ヘルプ画面のフラグ
{
	m_windowSize = windowSize;
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
PlayUI::~PlayUI()
{
	Finalize();
	m_system.reset();
}

/// <summary>
/// 作成処理
/// </summary>
/// <param name="system">システムマネージャ</param>
/// <param name="device">デバイスポインタ</param>
/// <returns>なし</returns>
void PlayUI::Create(const std::shared_ptr<SystemManager>& system ,ID3D11Device1* device)
{
	// システム
	m_system = system;

	// 画像を登録
	m_system->GetDrawSprite()->AddTextureData(L"Number",			// 数字スプライト
		L"Resources/Textures/Number.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"GameStart",			// スタート合図
		L"Resources/Textures/PLAY_COMMON/GameStart.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Death",				// 死亡エフェクト
		L"Resources/Textures/PLAY_COMMON/DeathEffect.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Sun",				// 日没メーター
		L"Resources/Textures/PLAY_COMMON/sun.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"Help",				// ヘルプ表示
		L"Resources/Textures/PLAY_HELP/Help.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"HelpBack",			// ヘルプ時の背景暗転
		L"Resources/Textures/PLAY_HELP/HelpBack.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"OpenHelp",			// ヘルプを開く表示
		L"Resources/Textures/PLAY_HELP/OpenHelp.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"UnderFont",			// 下のテキスト
		L"Resources/Textures/PLAY_HELP/UnderFont.dds", device);

	m_system->GetDrawSprite()->AddTextureData(L"UnderBack",			// 下のテキストのライン
		L"Resources/Textures/PLAY_HELP/UnderBack.dds", device);

	// 比率を計算
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	// スプライトの位置を計算
	m_countDownPos = { (FULL_SCREEN_SIZE.x / 2 - NUM_SIZE / 2) * _scale.x , 80.0f * _scale.y };
	m_oneSecPos = { m_countDownPos.x + static_cast<float>(NUM_SIZE / 2) * _scale.x ,m_countDownPos.y };
	m_tenSecPos = { m_countDownPos.x - static_cast<float>(NUM_SIZE / 2) * _scale.x ,m_countDownPos.y };
	m_sunPos = { (SUN_SIZE.x * _scale.x) / 2, SUN_SIZE.y * _scale.y };

	// 落下フラグを切る
	is_effectFlag = false;
	// ヘルプフラグを切る
	is_helpFlag = false;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timelimit">残り時間</param>
/// <returns>なし</returns>
void PlayUI::Update(const float& timelimit)
{
	m_gameTimer = static_cast<int>(timelimit);

	// 比率を計算
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	// 太陽の回転をセット
	m_system->GetDrawSprite()->CreateRotation(L"Sun", static_cast<float>(MAX_LIMITS - m_gameTimer) * ROT_SPEED);

	// 太陽の移動処理
	m_sunPos.x += m_gameTimer != 0 ? SUN_MOVE_SPEED * _scale.x : 0.0f;

	// フォントの移動処理
	m_underFontPos.x -= UNDER_SPEED * _scale.x;

	if (m_underFontPos.x < -FULL_SCREEN_SIZE.x * 2 * _scale.x)
	{
		m_underFontPos.x = FULL_SCREEN_SIZE.x;
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayUI::Render()
{
    // 比率を計算
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	// 落下エフェクト
	if (is_effectFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Death",                          // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			_scale,                             // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);
	}

	// 太陽アイコンの描画
	RenderSunny(_scale);

	// タイマーの描画
	RenderTimer(_scale);

	//-------------------------------------------------------------------------------------//
	// ヘルプ中の表示
	if (is_helpFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"HelpBack",                       // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			_scale,                             // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);
		m_system->GetDrawSprite()->DrawTexture(
			L"Help",                           // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			_scale,                             // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);
	}
	//-------------------------------------------------------------------------------------//
	// 通常の表示
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"OpenHelp",
			SimpleMath::Vector2{ m_windowSize.x - HELP_WIDTH * _scale.x, 0.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			SimpleMath::Vector2::Zero,
			{ 0,0,360,120 }
		);

		// 画面下のフォント
		m_system->GetDrawSprite()->DrawTexture(
			L"UnderBack",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			SimpleMath::Vector2::Zero
		);
		m_system->GetDrawSprite()->DrawTexture(
			L"UnderFont",
			m_underFontPos,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			SimpleMath::Vector2::Zero,
			// 画像のサイズ
			{ 0,0,static_cast<LONG>(FULL_SCREEN_SIZE.x * 2),static_cast<LONG>(FULL_SCREEN_SIZE.y) }
		);
	}
}

/// <summary>
/// 開始のカウントダウン
/// </summary>
/// <param name="countDown">カウントダウン</param>
/// <returns>なし</returns>
void PlayUI::RenderCountDown(const float& countDown)
{
	// 比率を計算
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	if (static_cast<int>(countDown / 60) == 0)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"GameStart",                      // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			_scale,                             // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);
		return;
	}

	int _num = static_cast<int>(countDown) / 60;
	RECT_U _countRec = { _num * NUM_SIZE, 0, _num * NUM_SIZE + NUM_SIZE, NUM_SIZE };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_countDownPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		_scale,
		SimpleMath::Vector2::Zero,
		_countRec
	);
}

/// <summary>
/// タイマーの描画
/// </summary>
/// <param name="scale">拡大率</param>
/// <returns>なし</returns>
void PlayUI::RenderTimer(SimpleMath::Vector2 scale)
{
	// 一桁に変換
	int _oneSec = m_gameTimer / 60 % 10;
	int _tenSec = m_gameTimer / 60 / 10 % 10;

	// 切り取り位置設定
	RECT_U _oneRec = { _oneSec * NUM_SIZE, 0, _oneSec * NUM_SIZE + NUM_SIZE, NUM_SIZE };
	RECT_U _tenRec = { _tenSec * NUM_SIZE, 0, _tenSec * NUM_SIZE + NUM_SIZE, NUM_SIZE };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_tenSecPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		_tenRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_oneSecPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		_oneRec
	);
}

/// <summary>
/// 太陽の描画
/// </summary>
/// <param name="scale">画像の拡大率</param>
/// <returns>なし</returns>
void PlayUI::RenderSunny(SimpleMath::Vector2 scale)
{
	m_system->GetDrawSprite()->DrawTexture(
		L"Sun",
		m_sunPos,
		{ 1.0f,1.0f,1.0f,1.0f },
		scale,
		SimpleMath::Vector2{ SUN_SIZE / 2},
		{ 0,0,100,100 }
	);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayUI::Finalize()
{
}
