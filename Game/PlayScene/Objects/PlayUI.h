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
private:
	// シェアポインタ
	std::shared_ptr<SystemManager> m_system;

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// タイマーの座標(1の位)
	DirectX::SimpleMath::Vector2 m_oneSecPos;
	// タイマーの座標(10の位)
	DirectX::SimpleMath::Vector2 m_tenSecPos;
	// タイマーの座標(最初のカウントダウン)
	DirectX::SimpleMath::Vector2 m_countDownPos;

	// 太陽の位置
	DirectX::SimpleMath::Vector2 m_sunPos;

	// 制限時間
	int m_gameTimer;

	// 落下エフェクトフラグ
	bool is_effectFlag;

	// 操作説明表示フラグ
	bool is_helpFlag;

	// 画面下の説明文の座標
	DirectX::SimpleMath::Vector2 m_underFontPos;


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

public:
	PlayUI(const DirectX::SimpleMath::Vector2& windowSize);
	~PlayUI();

	// 作成処理
	void Create(std::shared_ptr<SystemManager> system ,ID3D11DeviceContext1* context, ID3D11Device1* device);

	// 更新処理
	void Update(const float& timelimit);

	// 描画処理
	void Render();

	// カウントダウン
	void RenderCountDown(const float& countDown);

	// タイマーの描画
	void RenderTimer(DirectX::SimpleMath::Vector2 scale);

	// 太陽の描画関数
	void RenderSunny(DirectX::SimpleMath::Vector2 scale);

	// 終了処理
	void Finalize();

public:

	// エフェクトフラグのセット
	void SetEffectFlag(const bool& flag) { is_effectFlag = flag; }

	// ヘルプフラグのセット
	void SetHelpFlag(const bool& flag) { is_helpFlag = flag; }
};

#endif //PLAYUI
