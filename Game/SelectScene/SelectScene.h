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

class SelectScene : public IScene
{
private:
	static const float CAMERA_ANGLE;
	std::unique_ptr<DirectX::GeometricPrimitive> m_tea;

	// 回転用変数
	float m_angle;

	// ステージ番号
	int m_stageNum;

	// 最大ステージ数
	const int MAX_STAGE_NUM = 3;

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