/*
 *	@File	ResultScene.h
 *	@Brief  リザルトシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef RESULTSCENE
#define	RESULTSCENE

#include "../IScene.h"

class Blocks;
class ResultScene final : public IScene
{
private:
	// モード
	enum { RETRY, SELECT, TITLE };

	// タイマー
	float m_timer;

	// セレクト
	int m_selectingScene;

	// クリアタイム
	float m_clearTime;

	// クリアタイムを保存する変数
	float m_saveTime;

	// 演出時間
	float m_directionTime;

	// ブロック
	std::unique_ptr<Blocks> m_blocks;

	// マップ番号
	int m_stageNum;

	// コインの枚数
	int m_coinNum;

	// 画像座標
	DirectX::SimpleMath::Vector2 m_retryPos;
	DirectX::SimpleMath::Vector2 m_selectPos;
	DirectX::SimpleMath::Vector2 m_titlePos;
	DirectX::SimpleMath::Vector2 m_clearPos;
	DirectX::SimpleMath::Vector2 m_coinsPos;

	// タイマーの座標(1の位)
	DirectX::SimpleMath::Vector2 m_oneSecPos;
	// タイマーの座標(10の位)
	DirectX::SimpleMath::Vector2 m_tenSecPos;
	// コインの座標(1の位)
	DirectX::SimpleMath::Vector2 m_oneCoiPos;
	// コインの座標(10の位)
	DirectX::SimpleMath::Vector2 m_tenCoiPos;

	// 透明度
	float m_retryAlpha;
	float m_selectAlpha;
	float m_titleAlpha;

	// 拡大率
	float m_retryScale;
	float m_selectScale;
	float m_titleScale;

	// 画面サイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

private:
	// フルスクリーンサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = {1920.0f,1080.0f};

	// 画像拡大率
	const float IMAGE_RATE = 0.55f;
	const float DEFAULT_RATE = 1.0f;
	// 画像の中心位置
	const float	IMAGE_CENTER = 128.0f;
	const float FONT_WIDTH = 512.0f;

	// 画像の１スプライトサイズ
	const float SPRITE_SIZE = 100.0f;

	// フォントサイズ
	const float DEFAULT_FONT_SCALE = 1.0f;
	const float SELECT_FONT_SCALE = 1.1f;

	// フォント透明度
	const float DEFAULT_FONT_ALPHA = 0.5f;
	const float SELECT_FONT_ALPHA = 1.0f;

public:

	// コンストラクタ
	ResultScene();

	// デストラクタ
	~ResultScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,DirectX::Mouse::State& mouseState) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	// 画面依存の初期化
	void CreateWindowDependentResources() override;

	// クリアタイムセッター
	void SetClearTime(const float& clearTime) { m_clearTime = clearTime; }

	// マップ番号セッター
	void SetStageNum(const int& num) { m_stageNum = num; }

	// ディジットの計算
	void RenderDigit(int digit, const DirectX::SimpleMath::Vector2& position, DirectX::SimpleMath::Vector2 scale, int digitWidth, int digitHeight);

	// コインセッター
	void SetCoinNum(const int& coinNum) { m_coinNum = coinNum; }

private:

	// 文字の描画
	void DrawTextFonts(DirectX::SimpleMath::Vector2 imageScale);
};

#endif // RESULTSCENE