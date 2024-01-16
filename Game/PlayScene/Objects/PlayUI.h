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
class ShowKey;
class PlayUI
{
public:

	// 遷移番号
	enum GO{ NONE, RETRY, SELECT, TITLE, EXIT, LENGTH};
private:

	// システムポインタ
	std::shared_ptr<SystemManager> m_system;

	// キー入力表示
	std::unique_ptr<ShowKey> m_showKey;

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// タイマーの座標(1の位)
	DirectX::SimpleMath::Vector2 m_oneSecPos;

	// タイマーの座標(10の位)
	DirectX::SimpleMath::Vector2 m_tenSecPos;

	// タイマーの座標(最初のカウントダウン)
	DirectX::SimpleMath::Vector2 m_countDownPos;

	// スタートが消えるタイマー
	float m_countDownEnds;

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

	// カウントダウン表示終了速度
	const float COUNT_END_SPEED = 0.025f;

	// スタートの文字の上移動速度
	const float COUNT_MOVE_SPEED = 25.0f;

public:
	PlayUI();
	~PlayUI();

	/// <summary>
	/// 作成処理
	/// </summary>
	/// <param name="system">システムマネージャ</param>
	/// <param name="windowSize">ウィンドウサイズ</param>
	/// <returns>なし</returns>
	void Create(const std::shared_ptr<SystemManager>& system, const DirectX::SimpleMath::Vector2& windowSize);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="timelimit">残り時間</param>
	/// <returns>なし</returns>
	void Update(const float& timelimit);

	/// <summary>
	/// ページをめくる
	/// </summary>
	/// <param name="leftArrow">左orAキー</param>
	/// <param name="rightArrow">右orDキー</param>
	/// <returns>なし</returns>
	void UpdatePage(const bool& leftArrow, const bool& rightArrow);

	/// <summary>
	/// 遷移の更新
	/// </summary>
	/// <param name="upArrow">上orWキー</param>
	/// <param name="downArrow">下orSキー</param>
	/// <returns>なし</returns>
	void UpdateTransition(const bool& upArrow, const bool& downArrow);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Render();

	/// <summary>
	/// 開始のカウントダウン
	/// </summary>
	/// <param name="countDown">カウントダウン</param>
	/// <returns>なし</returns>
	void RenderCountDown(const float& countDown);

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize();

private:


	/// <summary>
	/// タイマーの描画
	/// </summary>
	/// <param name="scale">拡大率</param>
	/// <returns>なし</returns>
	void RenderTimer(DirectX::SimpleMath::Vector2 scale);

	/// <summary>
	/// 太陽の描画
	/// </summary>
	/// <param name="scale">画像の拡大率</param>
	/// <returns>なし</returns>
	void DrawSun(DirectX::SimpleMath::Vector2 scale);

	/// <summary>
	/// ページの描画
	/// </summary>
	/// <param name="scale">画面の拡大率</param>
	/// <returns>なし</returns>
	void DrawPage(DirectX::SimpleMath::Vector2 scale);

	/// <summary>
	/// 座標を移動させる
	/// </summary>
	/// <param name="*pos">座標</param>
	/// <param name="end">エンド座標</param>
	/// <returns>なし</returns>
	void MovePositions(DirectX::SimpleMath::Vector2* pos,const DirectX::SimpleMath::Vector2& end);

	/// <summary>
	/// 登録した座標を呼び出す
	/// </summary>
	/// <param name="key">登録キー</param>
	/// <returns>登録座標</returns>
	const DirectX::SimpleMath::Vector2& GetHelpPosition(const wchar_t* key);

public:

	// カウントダウン終了判定
	bool IsCountDownEnd() { return (m_countDownEnds < -0.5f); };
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
