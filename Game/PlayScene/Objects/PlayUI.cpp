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
/// <param name="context">コンテキストポインタ</param>
/// <param name="device">デバイスポインタ</param>
/// <returns>なし</returns>
void PlayUI::Create(std::shared_ptr<SystemManager> system ,
	ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// システム
	m_system = system;

	// 画像の設定
	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// 画像を登録
	m_system->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Death",  L"Resources/Textures/DeathEffect.dds", device);

	// ヘルプ画面
	m_system->GetDrawSprite()->AddTextureData(L"Help",	   L"Resources/Textures/PLAY_HELP/Help.dds",      device);
	m_system->GetDrawSprite()->AddTextureData(L"HelpBack", L"Resources/Textures/PLAY_HELP/HelpBack.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"OpenHelp", L"Resources/Textures/PLAY_HELP/OpenHelp.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"GameStart",L"Resources/Textures/PLAY_HELP/GameStart.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"UnderBack",L"Resources/Textures/PLAY_HELP/UnderBack.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"UnderFont",L"Resources/Textures/PLAY_HELP/UnderFont.dds", device);

	// 比率を計算
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	// スプライトの位置を計算
	m_countDownPos = { (FULL_SCREEN_SIZE.x / 2 - NUM_SIZE / 2) * scale.x , 80.0f * scale.y };
	m_oneSecPos = { m_countDownPos.x + static_cast<float>(NUM_SIZE / 2) * scale.x ,m_countDownPos.y };
	m_tenSecPos = { m_countDownPos.x - static_cast<float>(NUM_SIZE / 2) * scale.x ,m_countDownPos.y };

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
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	// 移動処理
	m_underFontPos.x -= UNDER_SPEED * scale.x;

	if (m_underFontPos.x < -FULL_SCREEN_SIZE.x * 2 * scale.x)
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
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	// 落下エフェクト
	if (is_effectFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Death",                          // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			scale,                             // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);
	}

	// タイマーの描画
	RenderTimer(scale);

	// ヘルプフラグ(操作説明)
	if (is_helpFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"HelpBack",                       // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			scale,                             // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);
		m_system->GetDrawSprite()->DrawTexture(
			L"Help",                           // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			scale,                             // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);
	}
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"OpenHelp",                       // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			scale,                             // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);

		//-------------------------------------------------------------------------------------//
		// 画面下のフォント
		m_system->GetDrawSprite()->DrawTexture(
			L"UnderBack",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			scale,
			SimpleMath::Vector2::Zero
		);
		m_system->GetDrawSprite()->DrawTexture(
			L"UnderFont",
			m_underFontPos,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			scale,
			SimpleMath::Vector2::Zero,
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
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	if (static_cast<int>(countDown / 60) == 0)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"GameStart",                      // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			scale,                             // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);
		return;
	}

	int num = static_cast<int>(countDown) / 60;
	RECT_U countRec = { num * NUM_SIZE, 0, num * NUM_SIZE + NUM_SIZE, NUM_SIZE };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_countDownPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		countRec
	);
}

/// <summary>
/// タイマーの描画
/// </summary>
/// <param name="scale">拡大率</param>
/// <returns>なし</returns>
void PlayUI::RenderTimer(SimpleMath::Vector2 scale)
{
	// 1の位に変換
	int oneSec = m_gameTimer / 60 % 10;
	int tenSec = m_gameTimer / 60 / 10 % 10;

	// 切り取り位置設定
	RECT_U oneRec = { oneSec * NUM_SIZE, 0, oneSec * NUM_SIZE + NUM_SIZE, NUM_SIZE };
	RECT_U tenRec = { tenSec * NUM_SIZE, 0, tenSec * NUM_SIZE + NUM_SIZE, NUM_SIZE };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_tenSecPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		tenRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_oneSecPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		oneRec
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
