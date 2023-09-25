/*
 *	@File	ResultUI.h
 *	@Brief	リザルトのUI表示。
 *	@Date	2023-09-25
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef RESULTUI
#define RESULTUI

 // モード
enum { RETRY, SELECT, TITLE };

class SystemManager;
class ResultUI
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

	// セレクト
	int m_selectingScene;

	// コインの枚数
	int m_coinNum;

	// 合計コイン数
	int m_oneCoins;
	int m_tenCoins;

	// クリアタイム
	int m_oneTime;
	int m_tenTime;

	// 透明度
	float m_retryAlpha;
	float m_selectAlpha;
	float m_titleAlpha;

	// 拡大率
	float m_retryScale;
	float m_selectScale;
	float m_titleScale;

private:
	// 画像拡大率
	const float IMAGE_RATE = 0.55f;
	const float DEFAULT_RATE = 1.0f;
	// 画像の中心位置
	const float	IMAGE_CENTER = 128.0f;
	const float FONT_WIDTH = 512.0f;

	// 画像の１スプライトサイズ
	const float NUM_SIZE = 100.0f;

	// フォントサイズ
	const float DEFAULT_FONT_SCALE = 2.0f;
	const float SELECT_FONT_SCALE = 2.1f;

	// フォント透明度
	const float DEFAULT_FONT_ALPHA = 0.5f;
	const float SELECT_FONT_ALPHA = 1.0f;

	// フォントの選択変更時の動き
	const float SELECT_CHANGE_FADE = 0.45f;


private:
	// 画像座標
	DirectX::SimpleMath::Vector2 m_retryPos;
	DirectX::SimpleMath::Vector2 m_selectPos;
	DirectX::SimpleMath::Vector2 m_titlePos;

public:
	ResultUI(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device);
	~ResultUI();

	// 初期化処理
	void Initialize(const DirectX::SimpleMath::Vector2& windowSize);

	// 更新処理
	void Update(const float& timer,const int& clearTime);

	// 描画処理
	void Render();

	// 終了処理
	void Finalize();

private:
	void DrawNumber(DirectX::SimpleMath::Vector2 scale);

	// 各選択項目の変更
	void CaseRetry();
	void CaseSelect();
	void CaseTitle();

public:
	// コイン数セッター
	void SetCoins(const int& coinNum);

	// 選択中の項目のセット
	void SetSelecting(const int& select) { m_selectingScene = select; }


};

#endif // RESULTUI