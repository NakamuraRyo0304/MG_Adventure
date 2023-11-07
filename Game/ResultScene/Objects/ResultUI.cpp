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

// コンストラクタ
ResultUI::ResultUI()
	: m_selectingScene{ NEXT }	// 現在選択中のシーン
	, m_oneCoins{}				//  1の位のコイン数
	, m_tenCoins{}				// 10の位のコイン数
	, m_oneTime{}				//  1の位のクリアタイム
	, m_tenTime{}				// 10の位のクリアタイム
	, m_texInfo{}				// フォントの情報
	, m_rect{}					// 切り取り位置
	, m_coinNum{ 0 }			// コインの枚数
{
}

// デストラクタ
ResultUI::~ResultUI()
{
	Finalize();
}

// 初期化処理
void ResultUI::Create(std::shared_ptr<SystemManager> system, const SimpleMath::Vector2& windowSize)
{
	m_system = system;
	m_windowSize = windowSize;

	// 画像を登録
	m_system->GetDrawSprite()->AddTextureData(L"Number",L"Resources/Textures/Number.dds");
	m_system->GetDrawSprite()->AddTextureData(L"RFont", L"Resources/Textures/RESULT_INFO/ResultFonts.dds");
	m_system->GetDrawSprite()->AddTextureData(L"BLIND", L"Resources/Textures/RESULT_INFO/ResultBack.dds");
	m_system->GetDrawSprite()->AddTextureData(L"PACK",  L"Resources/Textures/RESULT_INFO/ResultSelectPack.dds");

	// 文字の座標の初期化
	float _commonY = FULL_SCREEN_SIZE.y - static_cast<float>(NUM_SIZE * 2);
	m_texInfo[NEXT].pos = SimpleMath::Vector2{FONT_WIDTH * 0.9f, _commonY};
	m_texInfo[RETRY].pos = SimpleMath::Vector2{ m_texInfo[NEXT].pos.x + FONT_WIDTH, _commonY};
	m_texInfo[SELECT].pos = SimpleMath::Vector2{ m_texInfo[RETRY].pos.x + FONT_WIDTH, _commonY};
	// 画像の切り取り位置
	m_rect[NEXT] = {0L,0L,512L, 128L};
	m_rect[RETRY] = {0L,128L,512L, 256L};
	m_rect[SELECT] = {0L,256L,512L, 384L};
}

// 更新処理
void ResultUI::Update(const int& clearTime)
{
	// クリアタイムを保存
	m_oneTime = clearTime % 10;
	m_tenTime = clearTime / 10 % 10;

	// アルファ値とスケールの変更
	UpdateTexture(m_selectingScene);
}

// 描画処理
void ResultUI::Render(const bool& drawSelect)
{
	// 画面比率を計算
	SimpleMath::Vector2 _rate = m_windowSize / FULL_SCREEN_SIZE;

	// レイアウトの描画
	DrawLayout(_rate);

	// 数字を描画
	DrawNumber(SimpleMath::Vector2::One, _rate);

	// フェード中は処理しない
	if (not drawSelect) return;

	// シーン選択文字(ネクスト、リトライ、セレクト)
	DrawFonts(_rate);
}

// 終了処理
void ResultUI::Finalize()
{
	m_system.reset();
}

// 数字の描画
void ResultUI::DrawNumber(SimpleMath::Vector2 texScale, const SimpleMath::Vector2& windowRate)
{
	// クリア時間
	// 切り取り位置設定
	RECT_U _oneRec = { m_oneTime * NUM_SIZE, 0,m_oneTime * NUM_SIZE + NUM_SIZE, NUM_SIZE };
	RECT_U _tenRec = { m_tenTime * NUM_SIZE, 0,m_tenTime * NUM_SIZE + NUM_SIZE, NUM_SIZE };

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

	// コインの数
	// 切り取り位置設定
	_oneRec = { m_oneCoins * NUM_SIZE, 0,m_oneCoins * NUM_SIZE + NUM_SIZE, NUM_SIZE };
	_tenRec = { m_tenCoins * NUM_SIZE, 0,m_tenCoins * NUM_SIZE + NUM_SIZE, NUM_SIZE };

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

// レイアウトの描画
void ResultUI::DrawLayout(const SimpleMath::Vector2& windowRate)
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

// フォントの描画
void ResultUI::DrawFonts(const SimpleMath::Vector2& windowRate)
{
	for (int i = 0; i < SELECTION::LENGTH; ++i)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"PACK",
			m_texInfo[i].pos * windowRate,
			{ 1.0f, 1.0f, 1.0f, m_texInfo[i].alpha },
			IMAGE_RATE * windowRate * m_texInfo[i].scale,
			{ FONT_WIDTH / 2,FONT_HEIGHT / 2 },
			m_rect[i]
		);
	}
}

// テクスチャの変更
void ResultUI::UpdateTexture(SELECTION which)
{
	// 透明度とスケールの変更
	for (int i = 0; i < SELECTION::LENGTH; ++i)
	{
		if (i == which)
		{
			m_texInfo[which].alpha = UserUtility::Lerp(m_texInfo[which].alpha, SELECT_FONT_ALPHA, SELECT_CHANGE_FADE);
			m_texInfo[which].scale = UserUtility::Lerp(m_texInfo[which].scale, SELECT_FONT_SCALE, SELECT_CHANGE_FADE);
		}
		else
		{
			m_texInfo[i].alpha = DEFAULT_FONT_ALPHA;
			m_texInfo[i].scale = DEFAULT_FONT_SCALE;
		}
	}
}

// 合計コイン数の設定
void ResultUI::SetCoins(const int& totalCoinNum)
{
	m_oneCoins = totalCoinNum % 10;
	m_tenCoins = totalCoinNum / 10 % 10;
}