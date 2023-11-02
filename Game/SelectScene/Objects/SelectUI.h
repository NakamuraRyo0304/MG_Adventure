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

private:

	// キー判定
	float m_rightAlpha;
	float m_leftAlpha;

	// 合計コイン数
	int m_oneCoins;
	int m_tenCoins;
	int m_hanCoins;

	// プッシュスペース点滅用フラグ
	bool is_pushToFlag;

	// 演出用Y軸上下変数
	float m_moveY;

private:

	// 移動速度
	const float MOVE_SPEED = 5.0f;

	// 移動幅
	const float MOVE_WIDTH = 6.0f;

	// 画像幅
	const int NUM_WIDTH = 100;

	// プッシュ画像のサイズ
	const DirectX::SimpleMath::Vector2 PUSH_SIZE = SimpleMath::Vector2{ 1280.0f,128.0f };

public:
	SelectUI();
	~SelectUI();

	/// <summary>
	/// 作成関数
	/// </summary>
	 /// <param name="system">システムマネージャ</param>
	/// <param name="windowSize">画面サイズ</param>
	/// <returns>なし</returns>
	void Create(std::shared_ptr<SystemManager> system, const DirectX::SimpleMath::Vector2& windowSize);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="rightFlag">右キーを押した判定</param>
	/// <param name="leftFlag">左キーを押した判定</param>
	/// <returns>なし</returns>
	void Update(const bool& rightFlag, const bool& leftFlag);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="fadeValue">フェードの値</param>
	/// <param name="selectNum">選択中のステージ番号</param>
	/// <param name="maxNum">最大のステージ番号</param>
	/// <returns>なし</returns>
	void Render(const float& fadeValue, const int& selectNum , const int& maxNum);

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize();

private:

	/// <summary>
	/// 矢印を描画する
	/// </summary>
	/// <param name="windowRate">ウィンドウ比率</param>
	/// <param name="selectNum">選択中のステージ番号</param>
	/// <param name="maxNum">ステージ最大数</param>
	/// <returns>なし</returns>
	void DrawArrow(DirectX::SimpleMath::Vector2 windowRate, const int& selectNum, const int& maxNum);

	/// <summary>
	/// 数字の描画
	/// </summary>
	/// <param name="windowRate">ウィンドウ比率</param>
	/// <param name="texScale">テクスチャのスケール</param>
	/// <returns>なし</returns>
	void DrawNumber(DirectX::SimpleMath::Vector2 windowRate, DirectX::SimpleMath::Vector2 texScale);

	/// <summary>
	/// 外枠フレーム
	/// </summary>
	/// <param name="windowRate">ウィンドウ比率</param>
	/// <returns>なし</returns>
	void DrawFrame(DirectX::SimpleMath::Vector2 windowRate);

public:

	/// <summary>
	/// 合計コイン数をセットして計算
	/// </summary>
	/// <param name="totalCoinNum">コイン数</param>
	/// <returns>なし</returns>
	void SetAllCoins(const int& totalCoinNum);

	// コイン使用中の演出
	void MoveCoins(const float& moveY) { m_moveY = moveY; };
};

#endif // SELECTUI