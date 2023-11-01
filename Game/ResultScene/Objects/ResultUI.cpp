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
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
ResultUI::ResultUI()
	: m_selectingScene{ RETRY }	// 現在選択中のシーン
	, m_oneCoins{}				//  1の位のコイン数
	, m_tenCoins{}				// 10の位のコイン数
	, m_oneTime{}				//  1の位のクリアタイム
	, m_tenTime{}				// 10の位のクリアタイム
	, m_retryInfo{}				// リトライフォントの情報
	, m_selectInfo{}			// セレクトフォントの情報
	, m_titleInfo{}				// タイトルフォントの情報
	, m_coinNum{ 0 }			// 枚数
{
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
/// 作成関数
/// </summary>
/// <param name="system">システムマネージャ</param>
/// <param name="device">デバイスポインタ</param>
/// <param name="windowSize">ウィンドウサイズ</param>
/// <param name="fullSize">フルスクリーンサイズ</param>
/// <returns>なし</returns>
void ResultUI::Create(std::shared_ptr<SystemManager> system, ID3D11Device1* device,
	const SimpleMath::Vector2& windowSize, const SimpleMath::Vector2& fullSize)
{
	m_system = system;
	m_windowSize = windowSize;
	m_fullScreenSize = fullSize;

	// 画像を登録
	m_system->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds",		device);
	m_system->GetDrawSprite()->AddTextureData(L"RFont",  L"Resources/Textures/ResultFonts.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"BLIND",  L"Resources/Textures/ResultBack.dds",	device);
	m_system->GetDrawSprite()->AddTextureData(L"RETRY",  L"Resources/Textures/FONT/RETRY.dds",	device);
	m_system->GetDrawSprite()->AddTextureData(L"SELECT", L"Resources/Textures/FONT/SELECT.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"TITLE",  L"Resources/Textures/FONT/TITLE.dds",	device);

	SimpleMath::Vector2 _scale = m_windowSize / m_fullScreenSize;

	// 文字の座標の初期化
	float _commonY = m_fullScreenSize.y - NUM_SIZE * 2;
	m_retryInfo.pos  = SimpleMath::Vector2{ FONT_WIDTH / 2,					 _commonY };
	m_selectInfo.pos = SimpleMath::Vector2{ m_retryInfo.pos.x  + FONT_WIDTH, _commonY };
	m_titleInfo.pos  = SimpleMath::Vector2{ m_selectInfo.pos.x + FONT_WIDTH, _commonY };
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="clearTime">クリアタイム</param>
/// <returns>なし</returns>
void ResultUI::Update(const int& clearTime)
{
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
/// <param name = "fadeValue">フェードの値</param>
/// <returns>なし</returns>
void ResultUI::Render(const float& fadeValue)
{
	// 画面比率を計算
	SimpleMath::Vector2 _rate = m_windowSize / m_fullScreenSize;

	// レイアウトの描画
	DrawBack(_rate);

	// 数字を描画
	DrawNumber(DRAW_NUM_SIZE, _rate);

	// フェード中は処理しない
	if (fadeValue >= 0.7f) return;

	// シーン選択文字(リトライ、セレクト、タイトル)
	DrawFonts(_rate);
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
/// <param name="texScale">テクスチャのスケール</param>
/// <param name="windowRate">画面の拡大率</param>
/// <returns>なし</returns>
void ResultUI::DrawNumber(SimpleMath::Vector2 texScale, const SimpleMath::Vector2& windowRate)
{
	// クリア時間
	// 切り取り位置設定
	RECT_U _oneRec = { m_oneTime * 100, 0,m_oneTime * 100 + 100, 100 };
	RECT_U _tenRec = { m_tenTime * 100, 0,m_tenTime * 100 + 100, 100 };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 1000.0f ,290.0f } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale * windowRate,
		SimpleMath::Vector2::Zero,
		_oneRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 860.0f ,290.0f } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale * windowRate,
		SimpleMath::Vector2::Zero,
		_tenRec
	);

	//-------------------------------------------------------------------------------------//
	// コインの数
	// 切り取り位置設定
	_oneRec = { m_oneCoins * 100, 0,m_oneCoins * 100 + 100, 100 };
	_tenRec = { m_tenCoins * 100, 0,m_tenCoins * 100 + 100, 100 };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 1000.0f ,700.0f } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale * windowRate,
		SimpleMath::Vector2::Zero,
		_oneRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 860.0f ,700.0f } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale * windowRate,
		SimpleMath::Vector2::Zero,
		_tenRec
	);
}

/// <summary>
/// レイアウトの描画
/// </summary>
/// <param name="windowRate">画面の拡大率</param>
/// <returns>なし</returns>
void ResultUI::DrawBack(const SimpleMath::Vector2& windowRate)
{
	// 画面を薄暗くする
	m_system->GetDrawSprite()->DrawTexture(
		L"BLIND",									// 登録キー
		SimpleMath::Vector2::Zero,					// 座標
		{ 1.0f,1.0f,1.0f,0.3f },					// 色
		DEFAULT_RATE * windowRate,					// 拡大率
		SimpleMath::Vector2::Zero					// 中心位置
	);

	// 後ろの文字
	m_system->GetDrawSprite()->DrawTexture(
		L"RFont",
		SimpleMath::Vector2::Zero,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		DEFAULT_RATE * windowRate,
		SimpleMath::Vector2::Zero
	);
}

/// <summary>
/// 選択文字の描画
/// </summary>
/// <param name="windowRate">画面の拡大率</param>
/// <returns>なし</returns>
void ResultUI::DrawFonts(const DirectX::SimpleMath::Vector2& windowRate)
{
	m_system->GetDrawSprite()->DrawTexture(
		L"RETRY",
		m_retryInfo.pos * windowRate,
		{ 1.0f, 1.0f, 1.0f, m_retryInfo.alpha },
		IMAGE_RATE * windowRate * m_retryInfo.scale,
		SimpleMath::Vector2::Zero
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"SELECT",
		m_selectInfo.pos * windowRate,
		{ 1.0f, 1.0f, 1.0f, m_selectInfo.alpha },
		IMAGE_RATE * windowRate * m_selectInfo.scale,
		SimpleMath::Vector2::Zero
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"TITLE",
		m_titleInfo.pos * windowRate,
		{ 1.0f, 1.0f, 1.0f, m_titleInfo.alpha },
		IMAGE_RATE * windowRate * m_titleInfo.scale,
		SimpleMath::Vector2::Zero
	);
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
	m_retryInfo.alpha = UserUtility::Lerp(m_retryInfo.alpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	m_selectInfo.alpha = DEFAULT_FONT_ALPHA;
	m_titleInfo.alpha = DEFAULT_FONT_ALPHA;
	// サイズ
	m_retryInfo.scale = UserUtility::Lerp(m_retryInfo.scale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
	m_selectInfo.scale = DEFAULT_FONT_SCALE;
	m_titleInfo.scale = DEFAULT_FONT_SCALE;
}

/// <summary>
/// セレクトを選択中のテクスチャの状態
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultUI::CaseSelect()
{
	// 透明度
	m_retryInfo.alpha = DEFAULT_FONT_ALPHA;
	m_selectInfo.alpha = UserUtility::Lerp(m_selectInfo.alpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	m_titleInfo.alpha = DEFAULT_FONT_ALPHA;
	// サイズ
	m_retryInfo.scale = DEFAULT_FONT_SCALE;
	m_selectInfo.scale = UserUtility::Lerp(m_selectInfo.scale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
	m_titleInfo.scale = DEFAULT_FONT_SCALE;
}

/// <summary>
/// タイトルを選択中のテクスチャの状態
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultUI::CaseTitle()
{
	// 透明度
	m_retryInfo.alpha = DEFAULT_FONT_ALPHA;
	m_selectInfo.alpha = DEFAULT_FONT_ALPHA;
	m_titleInfo.alpha = UserUtility::Lerp(m_titleInfo.alpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
	// サイズ
	m_retryInfo.scale = DEFAULT_FONT_SCALE;
	m_selectInfo.scale = DEFAULT_FONT_SCALE;
	m_titleInfo.scale = UserUtility::Lerp(m_titleInfo.scale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
}
