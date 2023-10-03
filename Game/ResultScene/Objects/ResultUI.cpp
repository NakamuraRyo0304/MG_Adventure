/*
 *	@File	ResultUI.cpp
 *	@Brief	リザルトのUI表示。
 *	@Date	2023-09-25
 *  @Author NakamuraRyo
 */


#include "pch.h"

 // システムマネージャ
#include "Libraries/SystemManager/SystemManager.h"

// ユーザーユーティリティ
#include "Libraries/UserUtility.h"

#include "ResultUI.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="system">システムマネージャ</param>
 /// <param name="context">コンテキストポインタ</param>
 /// <param name="device">デバイスポインタ</param>
 /// <returns>なし</returns>
ResultUI::ResultUI(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device)
	: m_selectingScene{ RETRY }	// 現在選択中のシーン
	, m_oneCoins{}				//  1の位のコイン数
	, m_tenCoins{}				// 10の位のコイン数
	, m_oneTime{}				//  1の位のクリアタイム
	, m_tenTime{}				// 10の位のクリアタイム
	//-------------------------------------------------------------------------------------//
	,m_retryPos{}				// リトライテキストの位置
	,m_retryAlpha{}				// 透明度
	,m_retryScale{}				// 大きさ
	//-------------------------------------------------------------------------------------//
	,m_selectPos{}				// セレクトテキストの位置
	,m_selectAlpha{}			// 透明度
	,m_selectScale{}			// 大きさ
	//-------------------------------------------------------------------------------------//
	,m_titlePos{}				// タイトルテキストの位置
	,m_titleAlpha{}				// 透明度
	,m_titleScale{}				// 大きさ
	//-------------------------------------------------------------------------------------//
	,m_coinNum{ 0 }				// 枚数
	//-------------------------------------------------------------------------------------//
{
	m_system = system;

	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// 画像を登録
	m_system->GetDrawSprite()->AddTextureData(L"RETRY",  L"Resources/Textures/FONT/RETRY.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"SELECT", L"Resources/Textures/FONT/SELECT.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"TITLE",  L"Resources/Textures/FONT/TITLE.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"RFont",  L"Resources/Textures/ResultFonts.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"BLIND",  L"Resources/Textures/ResultBack.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds",      device);
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
ResultUI::~ResultUI()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="windowSize">画面サイズ</param>
/// <returns>なし</returns>
void ResultUI::Initialize(const SimpleMath::Vector2& windowSize)
{
	m_windowSize = windowSize;

	SimpleMath::Vector2 rate = m_windowSize / FULL_SCREEN_SIZE;

	// 文字の座標の初期化
	m_retryPos  = SimpleMath::Vector2{ FONT_WIDTH / 2, FULL_SCREEN_SIZE.y - 150.0f };
	m_selectPos = SimpleMath::Vector2{ m_retryPos.x + FONT_WIDTH, FULL_SCREEN_SIZE.y - 150.0f };
	m_titlePos  = SimpleMath::Vector2{ m_selectPos.x + FONT_WIDTH, FULL_SCREEN_SIZE.y - 150.0f };
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">時間</param>
/// <param name="clearTime">クリアタイム</param>
/// <returns>なし</returns>
void ResultUI::Update(const float& timer, const int& clearTime)
{
	m_timer = timer;

	// クリアタイムを保存
	m_oneTime = clearTime % 10;
	m_tenTime = clearTime / 10 % 10;

	// アルファ値とスケールの変更
	switch (m_selectingScene)
	{
	case RETRY:
		CaseRetry();
		break;
	case SELECT:
		CaseSelect();
		break;
	case TITLE:
		CaseTitle();
		break;
	default:
		break;
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name = "引数無し"></param>
/// <returns>なし</returns>
void ResultUI::Render()
{
	// 画面比率を計算
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	//-------------------------------------------------------------------------------------//

	// 画面を薄暗くする
	m_system->GetDrawSprite()->DrawTexture(
		L"BLIND",									// 登録キー
		SimpleMath::Vector2::Zero,					// 座標
		{ 1.0f,1.0f,1.0f,0.3f },					// 色
		1.0f * scale,								// 拡大率
		SimpleMath::Vector2::Zero					// 中心位置
	);

	// 後ろの文字
	m_system->GetDrawSprite()->DrawTexture(
		L"RFont",
		SimpleMath::Vector2::Zero,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		DEFAULT_RATE * scale,
		SimpleMath::Vector2::Zero
	);

	// 数字を描画
	DrawNumber(SimpleMath::Vector2::One);

	// シーン選択文字(リトライ、セレクト、タイトル)
	m_system->GetDrawSprite()->DrawTexture(
		L"RETRY",
		m_retryPos * scale,
		{ 1.0f, 1.0f, 1.0f, m_retryAlpha },
		IMAGE_RATE * scale * m_retryScale,
		SimpleMath::Vector2::Zero
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"SELECT",
		m_selectPos * scale,
		{ 1.0f, 1.0f, 1.0f, m_selectAlpha },
		IMAGE_RATE * scale * m_selectScale,
		SimpleMath::Vector2::Zero
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"TITLE",
		m_titlePos * scale,
		{ 1.0f, 1.0f, 1.0f, m_titleAlpha },
		IMAGE_RATE * scale * m_titleScale,
		SimpleMath::Vector2::Zero
	);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultUI::Finalize()
{
	m_system.reset();
}

/// <summary>
/// 数字の描画
/// </summary>
/// <param name="scale">テクスチャのスケール</param>
/// <returns>なし</returns>
void ResultUI::DrawNumber(SimpleMath::Vector2 scale)
{
	// 画面サイズ
	SimpleMath::Vector2 wScale = m_windowSize / FULL_SCREEN_SIZE;

	//-------------------------------------------------------------------------------------//
	// クリア時間
	// 切り取り位置設定
	RECT_U oneRec = { m_oneTime * 100, 0,m_oneTime * 100 + 100, 100 };
	RECT_U tenRec = { m_tenTime * 100, 0,m_tenTime * 100 + 100, 100 };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 1000.0f ,290.0f } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale * wScale,
		SimpleMath::Vector2::Zero,
		oneRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 860.0f ,290.0f } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale * wScale,
		SimpleMath::Vector2::Zero,
		tenRec
	);

	//-------------------------------------------------------------------------------------//

	//-------------------------------------------------------------------------------------//
	// コインの数
	// 切り取り位置設定
	oneRec = { m_oneCoins * 100, 0,m_oneCoins * 100 + 100, 100 };
	tenRec = { m_tenCoins * 100, 0,m_tenCoins * 100 + 100, 100 };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 1000.0f ,760.0f } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale * wScale,
		SimpleMath::Vector2::Zero,
		oneRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 860.0f ,760.0f } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale * wScale,
		SimpleMath::Vector2::Zero,
		tenRec
	);
	//-------------------------------------------------------------------------------------//
}

/// <summary>
/// 合計コイン数の計算
/// </summary>
/// <param name="totalCoinNum">コイン数</param>
/// <returns>なし</returns>
void ResultUI::SetCoins(const int& totalCoinNum)
{
	m_oneCoins = totalCoinNum % 10;
	m_tenCoins = totalCoinNum / 10 % 10;
}

/// <summary>
/// リトライを選択中のテクスチャの状態
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultUI::CaseRetry()
{
	// 透明度
	m_retryAlpha = UserUtility::Lerp(m_retryAlpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	m_selectAlpha = DEFAULT_FONT_ALPHA;
	m_titleAlpha = DEFAULT_FONT_ALPHA;
	// サイズ
	m_retryScale = UserUtility::Lerp(m_retryScale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
	m_selectScale = DEFAULT_FONT_SCALE;
	m_titleScale = DEFAULT_FONT_SCALE;
}

/// <summary>
/// セレクトを選択中のテクスチャの状態
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultUI::CaseSelect()
{
	// 透明度
	m_retryAlpha = DEFAULT_FONT_ALPHA;
	m_selectAlpha = UserUtility::Lerp(m_selectAlpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	m_titleAlpha = DEFAULT_FONT_ALPHA;
	// サイズ
	m_retryScale = DEFAULT_FONT_SCALE;
	m_selectScale = UserUtility::Lerp(m_selectScale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
	m_titleScale = DEFAULT_FONT_SCALE;
}

/// <summary>
/// タイトルを選択中のテクスチャの状態
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultUI::CaseTitle()
{
	// 透明度
	m_retryAlpha = DEFAULT_FONT_ALPHA;
	m_selectAlpha = DEFAULT_FONT_ALPHA;
	m_titleAlpha = UserUtility::Lerp(m_titleAlpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	// サイズ
	m_retryScale = DEFAULT_FONT_SCALE;
	m_selectScale = DEFAULT_FONT_SCALE;
	m_titleScale = UserUtility::Lerp(m_titleScale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
}
