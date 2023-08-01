/*
 *	@File	SelectScene.h
 *	@Brief  セレクトシーン。
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTSCENE
#define	SELECTSCENE

#include "../IScene.h"

class Blocks;
class SelectScene : public IScene
{
private:

	// タイマー
	float m_timer;

	// カメラアングル
	const float CAMERA_ANGLE = 45.0f;

	// ステージ
	std::unique_ptr<Blocks> m_blocks;
	// スカイドーム
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// ステージ番号
	int m_stageNum;

	// 最大ステージ数
	const int MAX_MODEL_NUM = 10;
	std::unique_ptr<DirectX::Model> m_stageModels[10];

	// カメラの回転半径
	const float CAMERA_RADIUS = 4.0f;

	// スカイドームの回転速度
	const float SKY_ROT_SPEED = 2.0f;

	// 見上げ値
	const float UP_VALUE = 60.0f;
	// 見下げ速度
	const float DOWN_SPEED = 0.8f;

	// 切り替え時読み込み演出
	float m_changeMapMove;

public:

	// コンストラクタ
	SelectScene();

	// デストラクタ
	~SelectScene();

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

	// ステージ番号のアクセサ
	const int& GetStageNum() { return m_stageNum; }
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }
};

#endif // SELECTSCENE