/*
 *	@File	SelectUI.cpp
 *	@Brief	セレクトのUI表示。
 *	@Date	2023-09-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

// システムマネージャ
#include "Libraries/SystemManager/SystemManager.h"

#include "SelectUI.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="system">システムマネージャ</param>
 /// <param name="context">コンテキストポインタ</param>
 /// <param name="device">デバイスポインタ</param>
 /// <returns>なし</returns>
SelectUI::SelectUI(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device)
	: m_rightAlpha{}		// 右矢印の透明度
	, m_leftAlpha{}			// 左矢印の透明度
	, m_oneCoins{}			//  1の位のコイン数
	, m_tenCoins{}			// 10の位のコイン数
{
	m_system = system;

	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// 画像の登録
	m_system->GetDrawSprite()->AddTextureData(L"RightArrow", L"Resources/Textures/SELECT_INFO/RightArrow.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"LeftArrow",  L"Resources/Textures/SELECT_INFO/LeftArrow.dds",  device);
	m_system->GetDrawSprite()->AddTextureData(L"CenterCoin", L"Resources/Textures/SELECT_INFO/TotalCoins.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Number",	 L"Resources/Textures/Number.dds",				   device);
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
SelectUI::~SelectUI()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="windowSize">画面サイズ</param>
/// <returns>なし</returns>
void SelectUI::Initialize(const SimpleMath::Vector2& windowSize)
{
	m_windowSize = windowSize;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">時間</param>
/// <param name="rightFlag">右キーを押した判定</param>
/// <param name="leftFlag">左キーを押した判定</param>
/// <returns>なし</returns>
void SelectUI::Update(const float& timer, const bool& rightFlag, const bool& leftFlag)
{
	m_timer = timer;

	// フラグによって透明度を変える
	m_rightAlpha = !rightFlag ? 0.5f : 1.0f;
	m_leftAlpha = !leftFlag ? 0.5f : 1.0f;
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="selectNum">選択中のステージ番号</param>
/// <returns>なし</returns>
void SelectUI::Render(const int& selectNum)
{
	// 画面比率を計算
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;
	SimpleMath::Vector2 coinsScale = m_windowSize / FULL_SCREEN_SIZE / 2;

	//-------------------------------------------------------------------------------------//

	// 総獲得コイン数を表示
	m_system->GetDrawSprite()->DrawTexture(
		L"CenterCoin",
		SimpleMath::Vector2{ 0.0f ,0.0f } * scale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		coinsScale,
		SimpleMath::Vector2::Zero
	);

	DrawNumber(coinsScale);

	//-------------------------------------------------------------------------------------//

	if (selectNum != 0) // 左矢印を表示
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"LeftArrow",
			SimpleMath::Vector2{ 20.0f ,360.0f } * scale,
			SimpleMath::Color{ 1.0f, 1.0f, 1.0f, m_leftAlpha },
			scale,
			SimpleMath::Vector2::Zero
		);
	}
	if (selectNum != 9) // 右矢印を表示
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"RightArrow",
			SimpleMath::Vector2{ 1650.0f,360.0f} * scale,
			SimpleMath::Color{ 1.0f, 1.0f, 1.0f, m_rightAlpha },
			scale,
			SimpleMath::Vector2::Zero
		);
	}
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectUI::Finalize()
{
	m_system.reset();
}

/// <summary>
/// 数字の描画
/// </summary>
/// <param name="scale">テクスチャのスケール</param>
/// <returns>なし</returns>
void SelectUI::DrawNumber(SimpleMath::Vector2 scale)
{
	// 画面サイズ
	SimpleMath::Vector2 wScale = m_windowSize / FULL_SCREEN_SIZE;

	// 初期値
	RECT_U oneRec = { 0,0,1000,100 };
	RECT_U tenRec = { 0,0,1000,100 };

	oneRec = { m_oneCoins * 100, 0,m_oneCoins * 100 + 100, 100 };
	tenRec = { m_tenCoins * 100, 0,m_tenCoins * 100 + 100, 100 };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 400.0f ,50.0f } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		tenRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 450.0f ,50.0f } * wScale,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		oneRec
	);

}

/// <summary>
/// 合計コイン数の計算
/// </summary>
/// <param name="totalCoinNum">コイン数</param>
/// <returns>なし</returns>
void SelectUI::SetTotalCoins(const int& totalCoinNum)
{
	m_oneCoins = totalCoinNum % 10;
	m_tenCoins = totalCoinNum / 10;
}
