/*
 *	@File	TitleUI.h
 *	@Brief	StartとExitの表示。
 *	@Date	2023-08-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TITLEUI
#define TITLEUI

class SystemManager;
class TitleUI
{
private:
	// システム
	std::shared_ptr<SystemManager> m_system;

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// セレクトフラグ
	bool is_selectFlag;

	// サイズ
	const float SELECT_FONT_RATE = 1.05f;
	const float DEFAULT_FONT_RATE = 1.0f;
	DirectX::SimpleMath::Vector2 m_sFontRate;
	DirectX::SimpleMath::Vector2 m_eFontRate;
	DirectX::SimpleMath::Vector2 m_sLineRate;
	DirectX::SimpleMath::Vector2 m_eLineRate;

	// 色
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_exitColor;

private:
	// フルスクリーンサイズ
	DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

public:

	TitleUI();
	~TitleUI();

	// 作成処理
	void Create(std::shared_ptr<SystemManager> system, const DirectX::SimpleMath::Vector2& windowSize);

	// 更新処理
	void Update(const bool& selectFlag);

	// 描画処理
	void Render(const float& fadeValue, const bool& endAnim);

	// 終了処理
	void Finalize();

};

#endif // TITLEUI