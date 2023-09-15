/*
 *	@File	SelectScene.h
 *	@Brief  セレクトシーン。
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTSCENE
#define	SELECTSCENE

#include <future>
#include "../IScene.h"

class Blocks;
class SelectUI;
class SelectScene final : public IScene
{
private:

	// タイマー
	float m_timer;

	// 点滅カウンタ
	float m_flashCount;

	// スカイドーム
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// ステージ番号
	int m_stageNum;
	// 未開放ステージ数
	int m_noStageNum;

	// 合計コイン数
	int m_totalCoins;

	// ブロックとステージ
	std::unique_ptr<Blocks> m_blocks[10];
	std::unique_ptr<DirectX::Model> m_stageModels[10];

	// 切り替え時読み込み演出
	float m_targetY;

	// ブロックのローディング
	std::future<void> m_loadTask;
	std::mutex m_mutex;

	// UI
	std::unique_ptr<SelectUI> m_userInterface;

private:
	// 定数-----------------------------------------------------------------------------------

	const float MAX_FLASH = 180.0f;

	// カメラアングル
	const float CAMERA_ANGLE = 45.0f;
	const float CAMERA_POS_Y = 30.0f;

	// 最大ステージ数
	const int MAX_STAGE_NUM = 10;
	// カメラの回転半径
	const float CAMERA_RADIUS = 4.0f;

	// スカイドームの回転速度
	const float SKY_ROT_SPEED = 2.0f;

	// 見上げ値
	const float UP_VALUE = 60.0f;
	// 見下げ速度
	const float DOWN_SPEED = 0.7f;

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

private:
	// ステージのローディング
	void CreateStages(ID3D11Device1* device);
	void CreateFirstStage(ID3D11Device1* device);

	// ステージ番号の変更
	void ChangeStageNumber(DirectX::Keyboard::State keyState);

	// セレクト演出
	void DirectionSelectChange();

	// テキストのマトリックス
	DirectX::SimpleMath::Matrix CreateTextMatrix(const float& rotValue);

public:
	// ステージ番号のアクセサ
	const int& GetStageNum() { return m_stageNum; }
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }

	// 未開放ステージセッター
	void SetNoStageNum(const int& stageNum) { m_noStageNum = stageNum; }

	// 合計コインアクセサ
	const int& GetTotalCoins() { return m_totalCoins; }
	void SetTotalCoins(const int& num);
};

#endif // SELECTSCENE