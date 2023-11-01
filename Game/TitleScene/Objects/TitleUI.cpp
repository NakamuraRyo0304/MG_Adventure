/*
 *	@File	TitleUI.cpp
 *	@Brief	StartとExitの表示。
 *	@Date	2023-08-19
 *  @Author NakamuraRyo
 */

#include "pch.h"

// システムマネージャ
#include "Libraries/SystemManager/SystemManager.h"

// ユーザーユーティリティ
#include "Libraries/UserUtility.h"

#include "TitleUI.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
TitleUI::TitleUI()
	: is_selectFlag{}			// 選択フラグ
	, m_windowSize{}			// ウィンドウサイズ
	, m_sFontRate{}				// スタートフォントの拡大率
	, m_eFontRate{}				// エンドフォントの拡大率
	, m_sLineRate{}				// スタートラインの拡大率
	, m_eLineRate{}				// エンドラインの拡大率
	, m_startColor{}			// 初めの色
	, m_exitColor{}				// 終わりの色
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
TitleUI::~TitleUI()
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
void TitleUI::Create(std::shared_ptr<SystemManager> system, ID3D11Device1* device,
	const SimpleMath::Vector2& windowSize, const SimpleMath::Vector2& fullSize)
{
	// システム
	m_system = system;
	m_windowSize = windowSize;
	m_fullScreenSize = fullSize;

	// 画像の追加
	m_system->GetDrawSprite()->AddTextureData(L"Start",		L"Resources/Textures/TITLE_BUTTON/Start.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Exit",		L"Resources/Textures/TITLE_BUTTON/Exit.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"UnderLine", L"Resources/Textures/TITLE_BUTTON/UnderLine.dds", device);

	// 拡大率の初期化
	m_sFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_sLineRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eLineRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="selectFlag">開始or終了フラグ</param>
/// <returns>なし</returns>
void TitleUI::Update(const bool& selectFlag)
{
	if (selectFlag) // Trueの時はStart
	{
		// 文字の拡大率
		m_sFontRate = SimpleMath::Vector2{ SELECT_FONT_RATE };
		m_eFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };

		// 下線の拡大率
		m_sLineRate  = UserUtility::Lerp(m_sLineRate,  SimpleMath::Vector2::One,	     SimpleMath::Vector2{ 0.2f });
		m_eLineRate  = UserUtility::Lerp(m_eLineRate,  SimpleMath::Vector2{ 0.0f,1.0f }, SimpleMath::Vector2{ 0.2f });

		// 文字の透明度
		m_startColor = SimpleMath::Vector4::One;
		m_exitColor  = SimpleMath::Vector4{ 1.0f,1.0f,1.0f,0.5f };
	}
	else			// Falseの時はExit
	{
		// 文字の拡大率
		m_sFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
		m_eFontRate = SimpleMath::Vector2{ SELECT_FONT_RATE };

		// 下線の拡大率
		m_sLineRate  = UserUtility::Lerp(m_sLineRate,  SimpleMath::Vector2{ 0.0f,1.0f }, SimpleMath::Vector2{ 0.2f });
		m_eLineRate  = UserUtility::Lerp(m_eLineRate,  SimpleMath::Vector2::One,		 SimpleMath::Vector2{ 0.2f });

		// 文字の透明度
		m_startColor = SimpleMath::Vector4{ 1.0f,1.0f,1.0f,0.5f };
		m_exitColor  = SimpleMath::Vector4::One;
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="fadeValue">フェードの値</param>
/// <param name="endAnim"アニメーションの処理判定</param>
/// <returns>なし</returns>
void TitleUI::Render(const float& fadeValue, const bool& endAnim)
{
	// 画面比率
	SimpleMath::Vector2 _rate = m_windowSize / m_fullScreenSize * 0.5f;

	// フェード中は処理しない
	if (fadeValue >= 0.7f) return;

	// アニメーション中は処理しない
	if (!endAnim) return;

	// スタートの文字--------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"Start",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * _rate,
		m_startColor,
		_rate * m_sFontRate,
		SimpleMath::Vector2::Zero
	);
	// イグジットの文字------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"Exit",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * _rate,
		m_exitColor,
		_rate * m_eFontRate,
		SimpleMath::Vector2::Zero
	);

	// アンダーライン--------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * _rate,
		SimpleMath::Vector4::One,
		m_sLineRate * _rate,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * _rate,
		SimpleMath::Vector4::One,
		m_eLineRate * _rate,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleUI::Finalize()
{
	m_system.reset();
}
