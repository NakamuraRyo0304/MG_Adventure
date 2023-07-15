/*
 *	@File	PlayUI.cpp
 *	@Brief	プレイシーンのUI表示。
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../../../Libraries/SystemManager/SystemManager.h"

#include "PlayUI.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
PlayUI::PlayUI(const DirectX::SimpleMath::Vector2& windowSize):
	m_systemManager{},
	m_windowSize{ windowSize }
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
PlayUI::~PlayUI()
{
	Finalize();
	m_systemManager.reset();
}

/// <summary>
/// 作成処理
/// </summary>
/// <param name="system">システムマネージャ</param>
/// <param name="context">ID3D11DeviceContext1ポインタ</param>
/// <param name="device">ID3D11Device1ポインタ</param>
/// <returns>なし</returns>
void PlayUI::Create(std::shared_ptr<SystemManager> system ,
	ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// システム
	m_systemManager = system;

	// 画像の設定
	m_systemManager->GetDrawSprite()->MakeSpriteBatch(context);

	// 画像を登録
	m_systemManager->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds", device);

	// 比率を計算
	float span = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// 座標情報
	m_timerPos = { m_windowSize.x / 2 * span , 80 * span };
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timelimit">残り時間</param>
/// <returns>なし</returns>
void PlayUI::Update(const float& timelimit)
{
	timelimit;
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayUI::Render()
{
	// 比率を計算
	float scale = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	RECT_U rect;

	rect.left = 0;
	rect.right = rect.left + 1000;
	rect.bottom = 100;

	int num = (rect.left / 100) ;

	SimpleMath::Vector2 Center = {
	static_cast<float>(rect.right / 2),
	static_cast<float>(rect.bottom / 2)};


	m_systemManager->GetDrawSprite()->DrawTexture(
		L"Number",							// 登録キー
		m_timerPos,							// 座標
		{ 1.0f,1.0f,1.0f,1.0f },			// 色
		1.0f * scale,						// 拡大率
		Center,								// 中心位置
		rect								// 切り取り位置
	);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayUI::Finalize()
{
}
