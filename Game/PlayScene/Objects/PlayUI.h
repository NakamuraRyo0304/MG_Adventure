/*
 *	@File	PlayUI.h
 *	@Brief	プレイシーンのUI表示。
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYUI
#define PLAYUI

class SystemManager;
class PlayUI
{
public:

	// 遷移番号
	enum GO{ NONE, RETRY, SELECT, TITLE, EXIT, LENGTH};
private:

	// システムポインタ
	std::shared_ptr<SystemManager> m_system;

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// タイマーの座標(1の位)
	DirectX::SimpleMath::Vector2 m_oneSecPos;

	// タイマーの座標(10の位)
	DirectX::SimpleMath::Vector2 m_tenSecPos;

	// タイマーの座標(最初のカウントダウン)
	DirectX::SimpleMath::Vector2 m_countDownPos;

	// 画面下の説明文の座標
	DirectX::SimpleMath::Vector2 m_underFontPos;

	// 太陽の位置
	DirectX::SimpleMath::Vector2 m_sunPos;

	// 制限時間
	int m_gameTimer;

	// 落下エフェクトフラグ
	bool is_effectFlag;

private:

	// 操作説明表示フラグ
	bool is_helpFlag;

	// 遷移画面フラグ
	bool is_transFlag;

	// 遷移番号
	int m_transNum;

	// 画像の移動座標
	DirectX::SimpleMath::Vector2 m_moveTexPos;

	// ヘルプ表示の座標
	struct HelpPosition { DirectX::SimpleMath::Vector2 initPos; DirectX::SimpleMath::Vector2 nowPos; };
	std::map<const wchar_t*, HelpPosition> m_helpPoses;
	int m_pageNum;

	// 矢印座標
	HelpPosition m_arrowPos;
	float m_targetArrow[GO::LENGTH];

private:

	// フルスクリーンサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// タイマーのスプライトのサイズ
	const int NUM_SIZE = 100;

	// Help表示の横サイズ
	const float HELP_WIDTH = 400.0f;

	// 下の文の流れる速度
	const float UNDER_SPEED = 2.5f;

	// 太陽の画像のサイズ
	const DirectX::SimpleMath::Vector2 SUN_SIZE = { 100.0f,100.0f };

	// 最大秒数
	const int MAX_LIMITS = 3600;

	// 回転速度
	const float ROT_SPEED = 0.005f;

	// 太陽の移動速度
	const float SUN_MOVE_SPEED = (FULL_SCREEN_SIZE.x - SUN_SIZE.x) / MAX_LIMITS;

	// ヘルプページ数
	const int MAX_PAGE = 3;

	// ページをめくる速度
	const float PAGE_SPEED = 0.1f;

	// 矢印のサイズ
	const DirectX::SimpleMath::Vector2 ARROW_SIZE = { 256.0f,256.0f };


public:
	PlayUI();
	~PlayUI();

	// 作成処理
	void Create(const std::shared_ptr<SystemManager>& system, ID3D11Device1* device, const DirectX::SimpleMath::Vector2& windowSize);

	// 更新処理
	void Update(const float& timelimit);

	// ページ更新
	void UpdatePage(const bool& leftArrow, const bool& rightArrow);

	// 遷移更新
	void UpdateTransition(const bool& upArrow, const bool& downArrow);

	// 描画処理
	void Render();

	// カウントダウン
	void RenderCountDown(const float& countDown);

	// 終了処理
	void Finalize();

private:

	// 下線の更新
	void UpdateUnderLine(DirectX::SimpleMath::Vector2 scale);

	// タイマーの描画
	void RenderTimer(DirectX::SimpleMath::Vector2 scale);

	// 太陽の描画関数
	void RenderSunny(DirectX::SimpleMath::Vector2 scale);

	// ヘルプページの描画
	void RenderHelpPage(DirectX::SimpleMath::Vector2 scale);

	// ヘルプ画像の左右移動
	void MovePositions(DirectX::SimpleMath::Vector2* pos,const DirectX::SimpleMath::Vector2& end);

	// ヘルプ画像の座標を呼び出す
	const DirectX::SimpleMath::Vector2& GetHelpPosition(const wchar_t* key);

public:

	// ページ番号ゲッター
	const int& GetPage() { return m_pageNum; }

	// 遷移ページゲッター
	const bool& GetTransitionPage() { return is_transFlag; }

	// 遷移先ゲッター
	const int& GetTransNum() { return m_transNum; }

	// エフェクトフラグのセット
	void SetEffectFlag(const bool& flag) { is_effectFlag = flag; }

	// ヘルプフラグのセット
	void SetHelpFlag(const bool& flag) { is_helpFlag = flag; }
};

#endif //PLAYUI
