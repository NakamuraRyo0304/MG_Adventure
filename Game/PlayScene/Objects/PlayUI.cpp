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
PlayUI::PlayUI(const SimpleMath::Vector2& windowSize):
	m_system{},
	m_windowSize{ windowSize },
	m_timeLimit{0},
    m_oneSecPos{SimpleMath::Vector2::Zero},
    m_tenSecPos{SimpleMath::Vector2::Zero},
	m_countDownPos{SimpleMath::Vector2::Zero},
	is_effectFlag{false},
	is_helpFlag{false}
{
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
	m_system->GetDrawSprite()->AddTextureData(L"Death", L"Resources/Textures/DeathEffect.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Help", L"Resources/Textures/PLAY_HELP/Help.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"HelpBack", L"Resources/Textures/PLAY_HELP/HelpBack.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"OpenHelp", L"Resources/Textures/PLAY_HELP/OpenHelp.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"GameStart", L"Resources/Textures/PLAY_HELP/GameStart.dds", device);

	// 比率を計算
	SimpleMath::Vector2 span = m_windowSize / FULL_SCREEN_SIZE;

	// スプライトの位置を計算
	m_oneSecPos	   = { 1010.0f * span.x, 80.0f * span.y };
	m_tenSecPos	   = {  910.0f * span.x, 80.0f * span.y };
	m_countDownPos = {  960.0f * span.x, 80.0f * span.y };

    // 座標補正
    m_oneSecPos.x	 -= NUMBER_SPRITE_SIZE / 2 * span.x;
    m_tenSecPos.x	 -= NUMBER_SPRITE_SIZE / 2 * span.x;
	m_countDownPos.x -= NUMBER_SPRITE_SIZE / 2 * span.x;

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
	m_timeLimit = static_cast<int>(timelimit);
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

    // 秒数を計算
    int oneSec = m_timeLimit / 60;

    // 一桁目の数字を表示
    RenderDigit(oneSec % 10, m_oneSecPos, scale, NUMBER_SPRITE_SIZE, NUMBER_SPRITE_SIZE);

    // 十の桁の数字を表示
    RenderDigit((oneSec / 10) % 10, m_tenSecPos, scale, NUMBER_SPRITE_SIZE, NUMBER_SPRITE_SIZE);

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

	// 画像表示
	RenderDigit(
		static_cast<int>(countDown / 60),
		m_countDownPos,
		scale,
		NUMBER_SPRITE_SIZE,
		NUMBER_SPRITE_SIZE
	);
}

/// <summary>
/// 数字を描画する
/// </summary>
/// <param name="digit">描画する数字</param>
/// <param name="position">座標</param>
/// <param name="scale">拡大率</param>
/// <param name="digitWidth">数字の幅</param>
/// <param name="digitHeight">数字の高さ</param>
/// <returns>なし</returns>
void PlayUI::RenderDigit(int digit, const SimpleMath::Vector2& position, SimpleMath::Vector2 scale, int digitWidth, int digitHeight)
{
	// スプライトの位置を計算
	float spritePosX = position.x * scale.x;
	float spritePosY = position.y * scale.y;

	// スプライトの中心位置を計算
	SimpleMath::Vector2 center = { spritePosX * scale.x / 2.0f, spritePosY * scale.y / 2.0f };

	// 切り取り位置の設定
	RECT_U rect;

	// 切り取り開始位置を設定(横)
	rect.left = digit * digitWidth;

	// 切り取り終了位置を設定(横)
	rect.right = rect.left + digitWidth;

	// 画像縦幅を設定
	rect.bottom = digitHeight;

	// 数字表示
	m_system->GetDrawSprite()->DrawTexture(
		L"Number",                         // 登録キー
		position + center,                 // 座標
		{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
		scale,                             // 拡大率
		center,                            // 中心位置
		rect                               // 切り取り位置
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
