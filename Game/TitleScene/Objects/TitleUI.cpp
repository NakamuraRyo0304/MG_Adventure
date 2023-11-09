/*
 *	@File	TitleUI.cpp
 *	@Brief	StartとExitの表示。
 *	@Date	2023-08-19
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemManager/SystemManager.h"
#include "Libraries/UserUtility.h"
#include "TitleUI.h"

// コンストラクタ
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

// デストラクタ
TitleUI::~TitleUI()
{
	Finalize();
}

// 作成関数
void TitleUI::Create(std::shared_ptr<SystemManager> system, const SimpleMath::Vector2& windowSize)
{
	// システム
	m_system = system;
	m_windowSize = windowSize;

	// 画像の追加
	m_system->GetDrawSprite()->AddTextureData(L"Start",		L"Resources/Textures/TITLE_BUTTON/Start.dds");
	m_system->GetDrawSprite()->AddTextureData(L"Exit",		L"Resources/Textures/TITLE_BUTTON/Exit.dds");
	m_system->GetDrawSprite()->AddTextureData(L"UnderLine", L"Resources/Textures/TITLE_BUTTON/UnderLine.dds");

	// 拡大率の初期化
	m_sFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_sLineRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eLineRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
}

// 更新処理
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

// 描画処理
void TitleUI::Render(const float& fadeValue, const bool& endAnim)
{
	// 画面比率
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE * 0.5f;

	// フェード中は処理しない
	if (fadeValue >= 0.7f) return;

	// アニメーション中は処理しない
	if (!endAnim) return;

	// スタートの文字--------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"Start",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * _scale,
		m_startColor,
		_scale * m_sFontRate,
		SimpleMath::Vector2::Zero
	);
	// イグジットの文字------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"Exit",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * _scale,
		m_exitColor,
		_scale * m_eFontRate,
		SimpleMath::Vector2::Zero
	);

	// アンダーライン--------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * _scale,
		SimpleMath::Vector4::One,
		m_sLineRate * _scale,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * _scale,
		SimpleMath::Vector4::One,
		m_eLineRate * _scale,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
}

// 終了処理
void TitleUI::Finalize()
{
	m_system.reset();
}
