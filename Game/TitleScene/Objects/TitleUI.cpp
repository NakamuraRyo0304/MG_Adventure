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
 /// <param name="windowSize">画面サイズ</param>
 /// <returns>なし</returns>
TitleUI::TitleUI(const SimpleMath::Vector2& windowSize)
	: is_selectFlag{}			// 選択フラグ
	, m_windowSize{windowSize}	// ウィンドウサイズ
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
/// <param name="context">コンテキストポインタ</param>
/// <param name="device">デバイスポインタ</param>
/// <returns>なし</returns>
void TitleUI::Create(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// システム
	m_system = system;

	// 画像の設定
	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	// 画像の追加
	m_system->GetDrawSprite()->AddTextureData(L"Start",		L"Resources/Textures/TITLE_BUTTON/Start.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Exit",		L"Resources/Textures/TITLE_BUTTON/Exit.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"UnderLine", L"Resources/Textures/TITLE_BUTTON/UnderLine.dds", device);

	// 拡大率の初期化
	m_sFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eFontRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_sLineRate = SimpleMath::Vector2{ DEFAULT_FONT_RATE };
	m_eLineRate = SimpleMath::Vector2{ 0.0f,1.0f };
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
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleUI::Render()
{
	// 画面比率
	SimpleMath::Vector2 posRate = m_windowSize / FULL_SCREEN_SIZE * 0.5f;

	// スタートの文字--------------------------------(画面比率の変更を考慮した座標に設定)
	m_system->GetDrawSprite()->DrawTexture(
		L"Start",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * posRate,
		m_startColor,
		posRate * m_sFontRate,
		SimpleMath::Vector2::Zero
	);
	// イグジットの文字------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"Exit",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * posRate,
		m_exitColor,
		posRate * m_eFontRate,
		SimpleMath::Vector2::Zero
	);

	// アンダーライン--------------------------------------------------------------------
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * posRate,
		SimpleMath::Vector4::One,
		m_sLineRate * posRate,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * posRate,
		SimpleMath::Vector4::One,
		m_eLineRate * posRate,
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
