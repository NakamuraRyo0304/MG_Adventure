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

// コンストラクタ
SelectUI::SelectUI()
	: m_rightAlpha{}		// 右矢印の透明度
	, m_leftAlpha{}			// 左矢印の透明度
	, m_oneCoins{}			//   1の位のコイン数
	, m_tenCoins{}			//  10の位のコイン数
	, m_hanCoins{}			// 100の位のコイン数
	, m_moveY{}				// 上下移動
	, is_pushToFlag{ false }// スペース画像点滅フラグ
{
}

// デストラクタ
SelectUI::~SelectUI()
{
	Finalize();
}

// 作成関数
void SelectUI::Create(std::shared_ptr<SystemManager> system, const SimpleMath::Vector2& windowSize)
{
	m_system = system;
	m_windowSize = windowSize;

	// 画像の登録
	m_system->GetDrawSprite()->AddTextureData(L"Number",		L"Resources/Textures/Number.dds");
	m_system->GetDrawSprite()->AddTextureData(L"RightArrow",	L"Resources/Textures/SELECT_INFO/RightArrow.dds");
	m_system->GetDrawSprite()->AddTextureData(L"LeftArrow",		L"Resources/Textures/SELECT_INFO/LeftArrow.dds");
	m_system->GetDrawSprite()->AddTextureData(L"CenterCoin",	L"Resources/Textures/SELECT_INFO/TotalCoins.dds");
	m_system->GetDrawSprite()->AddTextureData(L"PushSpace",		L"Resources/Textures/SELECT_INFO/SpaceToStart.dds");
	m_system->GetDrawSprite()->AddTextureData(L"OutsideFrame",	L"Resources/Textures/SELECT_INFO/OutFrame.dds");
}

// 更新処理
void SelectUI::Update(const bool& rightFlag, const bool& leftFlag)
{
	// フラグによって透明度を変える
	m_rightAlpha = !rightFlag ? 0.5f : 1.0f;
	m_leftAlpha  = !leftFlag  ? 0.5f : 1.0f;
}

// 描画処理
void SelectUI::Render(const float& fadeValue, const int& selectNum, const int& maxNum)
{
	// 画面比率を計算
	SimpleMath::Vector2 _rate = m_windowSize / FULL_SCREEN_SIZE;
	SimpleMath::Vector2 _coinsScale = m_windowSize / FULL_SCREEN_SIZE / 1.5;

	// 外枠フレームを描画
	DrawFrame(_rate);

	// 総獲得コイン数を表示
	m_system->GetDrawSprite()->DrawTexture(
		L"CenterCoin",
		SimpleMath::Vector2::Zero,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		_rate,
		SimpleMath::Vector2::Zero
	);

	// 数字を描画
	DrawNumber(_rate, _coinsScale);

	// 矢印を描画
	DrawArrow(_rate, selectNum, maxNum);

	// フェード中は処理しない
	if (fadeValue >= 0.7f) return;

	// プッシュ画像を表示
	m_system->GetDrawSprite()->DrawTexture(
		L"PushSpace",
		SimpleMath::Vector2{ (FULL_SCREEN_SIZE.x - PUSH_SIZE.x) / 2, FULL_SCREEN_SIZE.y - PUSH_SIZE.y } * _rate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		_rate,
		SimpleMath::Vector2::Zero
	);
}

// 終了処理
void SelectUI::Finalize()
{
	m_system.reset();
}

// 矢印描画
void SelectUI::DrawArrow(SimpleMath::Vector2 windowRate, const int& selectNum, const int& maxNum)
{
	if (selectNum != 0) // 左矢印を表示
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"LeftArrow",
			SimpleMath::Vector2{ 20.0f ,360.0f } * windowRate,
			SimpleMath::Color{ 1.0f, 1.0f, 1.0f, m_leftAlpha },
			windowRate,
			SimpleMath::Vector2::Zero
		);
	}
	if (selectNum != maxNum) // 右矢印を表示
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"RightArrow",
			SimpleMath::Vector2{ 1650.0f,360.0f } * windowRate,
			SimpleMath::Color{ 1.0f, 1.0f, 1.0f, m_rightAlpha },
			windowRate,
			SimpleMath::Vector2::Zero
		);
	}
}

// 数字の描画
void SelectUI::DrawNumber(SimpleMath::Vector2 windowRate, SimpleMath::Vector2 texScale)
{
	// 切り取り位置設定
	RECT_U _oneRec = { m_oneCoins * NUM_WIDTH, 0,m_oneCoins * NUM_WIDTH + NUM_WIDTH, NUM_WIDTH };
	RECT_U _tenRec = { m_tenCoins * NUM_WIDTH, 0,m_tenCoins * NUM_WIDTH + NUM_WIDTH, NUM_WIDTH };
	RECT_U _hanRec = { m_hanCoins * NUM_WIDTH, 0,m_hanCoins * NUM_WIDTH + NUM_WIDTH, NUM_WIDTH };

	float _positionY = 15.0f + sinf(m_moveY * MOVE_SPEED) * MOVE_WIDTH;

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 500.0f ,_positionY } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale,
		SimpleMath::Vector2::Zero,
		_hanRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 550.0f ,_positionY } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale,
		SimpleMath::Vector2::Zero,
		_tenRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		SimpleMath::Vector2{ 600.0f ,_positionY } * windowRate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		texScale,
		SimpleMath::Vector2::Zero,
		_oneRec
	);
}

// 外枠フレーム
void SelectUI::DrawFrame(SimpleMath::Vector2 windowRate)
{
	m_system->GetDrawSprite()->DrawTexture(
		L"OutsideFrame",
		SimpleMath::Vector2::Zero,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		windowRate,
		SimpleMath::Vector2::Zero
	);
}

// コイン数を各桁に変換
void SelectUI::SetAllCoins(const int& totalCoinNum)
{
	m_oneCoins = totalCoinNum % 10;
	m_tenCoins = totalCoinNum / 10 % 10;
	m_hanCoins = totalCoinNum / 100;
}