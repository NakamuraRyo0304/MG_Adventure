/*
 *	@File	TitleUI.cpp
 *	@Brief	StartとExitの表示。
 *	@Date	2023-08-19
 *  @Author NakamuraRyo
 */

#include "pch.h"

// システムマネージャ
#include "Libraries/SystemManager/SystemManager.h"

#include "TitleUI.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
TitleUI::TitleUI(const SimpleMath::Vector2& windowSize):
	is_selectFlag{},
	m_windowSize{windowSize}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
TitleUI::~TitleUI()
{
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

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="selectFlag">開始or終了フラグ</param>
/// <returns>なし</returns>
void TitleUI::Update(const bool& selectFlag)
{
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleUI::Render()
{
	SimpleMath::Vector2 posRate = m_windowSize / FULL_SCREEN_SIZE;

	m_system->GetDrawSprite()->DrawTexture(
		L"Start",
		SimpleMath::Vector2{ 100.0f,700.0f } * posRate,
		SimpleMath::Color{ 1.0f,1.0f,1.0f,1.0f },
		1.0f * posRate,
		SimpleMath::Vector2::Zero
	);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleUI::Finalize()
{
}
