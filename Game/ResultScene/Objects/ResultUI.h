/*
 *	@File	ResultUI.h
 *	@Brief	リザルトのUI表示。
 *	@Date	2023-09-25
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef RESULTUI
#define RESULTUI

#include "Libraries/SystemDatas/DrawSprite.h"

 // モード
enum SELECTION{ NEXT, RETRY, SELECT, LENGTH };

class SystemManager;
class ResultUI
{
private:
	// システム
	std::shared_ptr<SystemManager> m_system;

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// セレクト
	SELECTION m_selectingScene;

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
	TexInfo m_texInfo[LENGTH];
	RECT m_rect[LENGTH];

private:
	// 画像拡大率
	const float IMAGE_RATE = 0.55f;
	const float DEFAULT_RATE = 1.0f;
	// 画像の中心位置
	const float	FONT_HEIGHT = 128.0f;
	const float FONT_WIDTH = 512.0f;

	// 画像の１スプライトサイズ
	const int NUM_SIZE = 100;

	// フォントサイズ/透明度
	const float DEFAULT_FONT_SCALE = 1.85f;
	const float DEFAULT_FONT_ALPHA = 0.5f;
	const float SELECT_FONT_SCALE = 2.1f;
	const float SELECT_FONT_ALPHA = 1.0f;

	// フォントの選択変更時の動き
	const float SELECT_CHANGE_FADE = 0.45f;
	const float NO_SELECT_TIME = 0.2f;

	// フルスクサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

public:
	ResultUI();
	~ResultUI();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="system">システムマネージャ</param>
	/// <param name="windowSize">ウィンドウサイズ</param>
	/// <returns>なし</returns>
	void Create(std::shared_ptr<SystemManager> system, const DirectX::SimpleMath::Vector2& windowSize);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="clearTime">クリアタイム</param>
	/// <returns>なし</returns>
	void Update(const int& clearTime);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name = "drawSelect">セレクトを描画するフラグ</param>
	/// <returns>なし</returns>
	void Render(const bool& drawSelect);

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize();

private:

	/// <summary>
	/// 数字の描画
	/// </summary>
	/// <param name="texScale">テクスチャのスケール</param>
	/// <param name="windowRate">画面の拡大率</param>
	/// <returns>なし</returns>
	void DrawNumber(DirectX::SimpleMath::Vector2 texScale, const DirectX::SimpleMath::Vector2& windowRate);

	/// <summary>
	/// レイアウトの描画
	/// </summary>
	/// <param name="windowRate">画面の拡大率</param>
	/// <returns>なし</returns>
	void DrawLayout(const DirectX::SimpleMath::Vector2& windowRate);

	/// <summary>
	/// 選択文字の描画
	/// </summary>
	/// <param name="windowRate">画面の拡大率</param>
	/// <returns>なし</returns>
	void DrawFonts(const DirectX::SimpleMath::Vector2& windowRate);

	/// <summary>
	/// テクスチャの状態を変更
	/// </summary>
	/// <param name="which">選択項目</param>
	/// <returns>なし</returns>
	void UpdateTexture(SELECTION which);

public:
	/// 合計コイン数の設定
	void SetCoins(const int& coinNum);
	// 選択中の項目の設定
	void SetSelecting(const int& select) { m_selectingScene = SELECTION(select); }
};

#endif // RESULTUI