/*
 *	@File	TitleScene.h
 *	@Brief	タイトルシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TITLESCENE
#define	TITLESCENE

#include "../IScene.h"

class TitleScene final : public IScene
{
private:
	// タイマー
	float m_timer;

	// タイトルロゴモデル
	std::unique_ptr<DirectX::Model> m_titleLogoModel;
	// タイトルステージモデル
	std::unique_ptr<DirectX::Model> m_miniatureModel;
	// スカイドーム
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// カメラ演出
	float m_moveY;
	const float MAX_HEIGHT = 10.0f;
	float m_logoMoveScale;

	// 決定判定
	bool is_startFlag;

public:

	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,DirectX::Mouse::State&) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	// 画面依存の初期化
	void CreateWindowDependentResources() override;
};

#endif // TITLESCENE