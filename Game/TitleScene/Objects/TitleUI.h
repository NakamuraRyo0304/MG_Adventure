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

	/// <summary>
	/// 作成関数
	/// </summary>
	/// <param name="system">システムマネージャ</param>
	/// <param name="windowSize">ウィンドウサイズ</param>
	/// <returns>なし</returns>
	void Create(std::shared_ptr<SystemManager> system, const DirectX::SimpleMath::Vector2& windowSize);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="selectFlag">開始or終了フラグ</param>
	/// <returns>なし</returns>
	void Update(const bool& selectFlag);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="fadeValue">フェードの値</param>
	/// <param name="endAnim"アニメーションの処理判定</param>
	/// <returns>なし</returns>
	void Render(const float& fadeValue, const bool& endAnim);

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize();

};

#endif // TITLEUI