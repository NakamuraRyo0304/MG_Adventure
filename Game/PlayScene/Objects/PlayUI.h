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

	// フルスクリーンサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// タイマーのスプライトのサイズ
	const int NUMBER_SPRITE_SIZE = 100;

	// タイマーの座標(1の位)
	DirectX::SimpleMath::Vector2 m_oneSecPos;
	// タイマーの座標(10の位)
	DirectX::SimpleMath::Vector2 m_tenSecPos;
	// タイマーの座標(最初のカウントダウン)
	DirectX::SimpleMath::Vector2 m_countDownPos;

	// 制限時間
	int m_gameTimer;

	// 落下エフェクトフラグ
	bool is_effectFlag;

	// 操作説明表示フラグ
	bool is_helpFlag;


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

	// ディジットの計算
	void RenderDigit(int digit, const DirectX::SimpleMath::Vector2& position, DirectX::SimpleMath::Vector2 scale, int digitWidth, int digitHeight);

	// 終了処理
	void Finalize();

public:

	// エフェクトフラグのセット
	void SetEffectFlag(const bool& flag) { is_effectFlag = flag; }

	// ヘルプフラグのセット
	void SetHelpFlag(const bool& flag) { is_helpFlag = flag; }
};

#endif //PLAYUI
