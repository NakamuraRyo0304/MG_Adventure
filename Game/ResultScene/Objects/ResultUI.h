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

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

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

	// 描画で使う情報
	struct TexInfo { DirectX::SimpleMath::Vector2 pos; float alpha; float scale; };
	TexInfo m_retryInfo;
	TexInfo m_selectInfo;
	TexInfo m_titleInfo;

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

	// 数字のデフォルトサイズ
	const DirectX::SimpleMath::Vector2 DRAW_NUM_SIZE = SimpleMath::Vector2{ 1.0f };

	// フルスクサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

public:
	ResultUI();
	~ResultUI();

	// 作成関数
	void Create(std::shared_ptr<SystemManager> system, ID3D11Device1* device, const DirectX::SimpleMath::Vector2& windowSize);

	// 更新処理
	void Update(const int& clearTime);

	// 描画処理
	void Render(const float& fadeValue);

	// 終了処理
	void Finalize();

private:

	// 数字の描画
	void DrawNumber(DirectX::SimpleMath::Vector2 texScale, const DirectX::SimpleMath::Vector2& windowRate);

	// 背景とレイアウトの描画
	void DrawBack(const DirectX::SimpleMath::Vector2& windowRate);

	// 選択文字の描画
	void DrawFonts(const DirectX::SimpleMath::Vector2& windowRate);

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