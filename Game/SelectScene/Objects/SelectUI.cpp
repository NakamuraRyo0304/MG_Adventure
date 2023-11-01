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
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
SelectUI::SelectUI()
	: m_timer{}				// タイマー
	, m_rightAlpha{}		// 右矢印の透明度
	, m_leftAlpha{}			// 左矢印の透明度
	, m_oneCoins{}			//   1の位のコイン数
	, m_tenCoins{}			//  10の位のコイン数
	, m_hanCoins{}			// 100の位のコイン数
	, m_moveY{}				// 上下移動
	, is_pushToFlag{ false }// スペース画像点滅フラグ
{
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
/// 作成関数
/// </summary>
/// <param name="system">システムマネージャ</param>
/// <param name="device">デバイスポインタ</param>
/// <param name="windowSize">ウィンドウサイズ</param>
/// <param name="fullSize">フルスクリーンサイズ</param>
/// <returns>なし</returns>
void SelectUI::Create(std::shared_ptr<SystemManager> system, ID3D11Device1* device,
	const SimpleMath::Vector2& windowSize, const SimpleMath::Vector2& fullSize)
{
	m_system = system;
	m_windowSize = windowSize;
	m_fullScreenSize = fullSize;

	// 画像の登録
	m_system->GetDrawSprite()->AddTextureData(L"Number",		L"Resources/Textures/Number.dds",					device);
	m_system->GetDrawSprite()->AddTextureData(L"RightArrow",	L"Resources/Textures/SELECT_INFO/RightArrow.dds",	device);
	m_system->GetDrawSprite()->AddTextureData(L"LeftArrow",		L"Resources/Textures/SELECT_INFO/LeftArrow.dds",	device);
	m_system->GetDrawSprite()->AddTextureData(L"CenterCoin",	L"Resources/Textures/SELECT_INFO/TotalCoins.dds",	device);
	m_system->GetDrawSprite()->AddTextureData(L"PushSpace",		L"Resources/Textures/SELECT_INFO/SpaceToStart.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"OutsideFrame",	L"Resources/Textures/SELECT_INFO/OutFrame.dds",		device);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="rightFlag">右キーを押した判定</param>
/// <param name="leftFlag">左キーを押した判定</param>
/// <returns>なし</returns>
void SelectUI::Update(const float& timer, const bool& rightFlag, const bool& leftFlag)
{
	m_timer = timer;

	// フラグによって透明度を変える
	m_rightAlpha = !rightFlag ? 0.5f : 1.0f;
	m_leftAlpha  = !leftFlag  ? 0.5f : 1.0f;
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="fadeValue">フェードの値</param>
/// <param name="selectNum">選択中のステージ番号</param>
/// <param name="maxNum">最大のステージ番号</param>
/// <returns>なし</returns>
void SelectUI::Render(const float& fadeValue, const int& selectNum, const int& maxNum)
{
	// 画面比率を計算
	SimpleMath::Vector2 _rate = m_windowSize / m_fullScreenSize;
	SimpleMath::Vector2 _coinsRate = m_windowSize / m_fullScreenSize / 1.5;

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
	DrawNumber(_rate, _coinsRate);

	// 矢印を描画
	DrawArrow(_rate, selectNum, maxNum);

	// フェード中は処理しない
	if (fadeValue >= 0.7f) return;

	// プッシュ画像を表示
	m_system->GetDrawSprite()->DrawTexture(
		L"PushSpace",
		SimpleMath::Vector2{ (m_fullScreenSize.x - PUSH_SIZE.x) / 2, m_fullScreenSize.y - PUSH_SIZE.y } * _rate,
		SimpleMath::Color{ 1.0f, 1.0f, 1.0f, 1.0f },
		_rate,
		SimpleMath::Vector2::Zero
	);
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
/// 矢印を描画する
/// </summary>
/// <param name="windowRate">ウィンドウ比率</param>
/// <param name="selectNum">選択中のステージ番号</param>
/// <param name="maxNum">ステージ最大数</param>
/// <returns>なし</returns>
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

/// <summary>
/// 数字の描画
/// </summary>
/// <param name="windowRate">ウィンドウ比率</param>
/// <param name="texScale">テクスチャのスケール</param>
/// <returns>なし</returns>
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

/// <summary>
/// 外枠フレーム
/// </summary>
/// <param name="windowRate">ウィンドウ比率</param>
/// <returns>なし</returns>
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

/// <summary>
/// 合計コイン数の計算
/// </summary>
/// <param name="totalCoinNum">コイン数</param>
/// <returns>なし</returns>
void SelectUI::SetAllCoins(const int& totalCoinNum)
{
	m_oneCoins = totalCoinNum % 10;
	m_tenCoins = totalCoinNum / 10 % 10;
	m_hanCoins = totalCoinNum / 100;
}