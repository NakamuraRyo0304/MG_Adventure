/*
 *	@File	SelectUI.h
 *	@Brief	セレクトのUI表示。
 *	@Date	2023-09-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTUI
#define SELECTUI

class SystemManager;
class SelectUI
{
private:
	// システム
	std::shared_ptr<SystemManager> m_system;

	// フルスクリーンサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// タイマー
	float m_timer;

	// キー判定
	float m_rightAlpha;
	float m_leftAlpha;

	// 合計コイン数
	int m_oneCoins;
	int m_tenCoins;
	int m_hanCoins;

public:
	SelectUI(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device);
	~SelectUI();

	// 初期化処理
	void Initialize(const DirectX::SimpleMath::Vector2& windowSize);

	// 更新処理
	void Update(const float& timer, const bool& rightFlag, const bool& leftFlag);

	// 描画処理
	void Render(const int& selectNum , const int& maxNum);

	// 終了処理
	void Finalize();

private:
	void DrawNumber(DirectX::SimpleMath::Vector2 scale);

public:
	// コイン数セッター
	void SetAllCoins(const int& totalCoinNum);
};

#endif // SELECTUI