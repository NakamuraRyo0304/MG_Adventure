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
TitleUI::TitleUI(const SimpleMath::Vector2& windowSize):
	is_selectFlag{},
	m_windowSize{windowSize},
	m_startRate{},
	m_exitRate{},
	m_startColor{},
	m_exitColor{}
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
	m_system->GetDrawSprite()->AddTextureData(L"Start", L"Resources/Textures/TITLE_BUTTON/Start.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Exit", L"Resources/Textures/TITLE_BUTTON/Exit.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"UnderLine", L"Resources/Textures/TITLE_BUTTON/UnderLine.dds", device);

	// 拡大率の初期化
	m_startRate = { 1.0f,1.0f };
	m_exitRate  = { 0.0f,1.0f };
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
		// 画像の拡大率
		m_startRate = UserUtility::Lerp(m_startRate, SimpleMath::Vector2::One,		   SimpleMath::Vector2{ 0.2f });
		m_exitRate  = UserUtility::Lerp(m_exitRate,  SimpleMath::Vector2{ 0.0f,1.0f }, SimpleMath::Vector2{ 0.2f });
		// 画像の透明度
		m_startColor = SimpleMath::Vector4::One;
		m_exitColor  = SimpleMath::Vector4{ 1.0f,1.0f,1.0f,0.5f };
	}
	else			// Falseの時はExit
	{
		// 画像の拡大率
		m_startRate = UserUtility::Lerp(m_startRate, SimpleMath::Vector2{ 0.0f,1.0f }, SimpleMath::Vector2{ 0.2f });
		m_exitRate  = UserUtility::Lerp(m_exitRate,  SimpleMath::Vector2::One,		   SimpleMath::Vector2{ 0.2f });
		// 画像の透明度
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

	// スタートの文字
	m_system->GetDrawSprite()->DrawTexture(
		L"Start",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * posRate,
		m_startColor,
		posRate,
		SimpleMath::Vector2::Zero
	);
	// イグジットの文字
	m_system->GetDrawSprite()->DrawTexture(
		L"Exit",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * posRate,
		m_exitColor,
		posRate,
		SimpleMath::Vector2::Zero
	);

	// アンダーライン
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2048.0f,1792.0f } * posRate,
		SimpleMath::Vector4::One,
		m_startRate * posRate,
		SimpleMath::Vector2::Zero,
		{ 0,0,768,256 }
	);
	m_system->GetDrawSprite()->DrawTexture(
		L"UnderLine",
		SimpleMath::Vector2{ 2816.0f,1792.0f } * posRate,
		SimpleMath::Vector4::One,
		m_exitRate * posRate,
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
