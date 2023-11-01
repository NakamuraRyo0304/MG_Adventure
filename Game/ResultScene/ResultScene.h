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
class ResultUI;
class ResultScene final : public IScene
{
private:

	// クリアタイム
	float m_clearTime;

	// クリアタイムを保存する変数
	float m_saveTime;

	// セレクト
	int m_selectingScene;

	// 演出時間
	float m_directionTime;

	// ブロック
	std::unique_ptr<Blocks> m_blocks;

	// UI
	std::unique_ptr<ResultUI> m_resultUI;

	// マップ番号
	int m_stageNum;

	// コインの枚数
	int m_coinNum;

	// 最大秒数
	const int MAX_TIME = 60;

public:

	// コンストラクタ
	ResultScene();

	// デストラクタ
	~ResultScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	// 画面依存の初期化
	void CreateWindowDependentResources() override;

private:

	// シーン内の変数初期化関数
	void SetSceneValues() override;

	// 数値演出
	bool AnimationValue();

public:

	// クリアタイムセッター
	void SetClearTime(const float& clearTime) { m_clearTime = clearTime; }

	// マップ番号セッター
	void SetStageNum(const int& num) { m_stageNum = num; }

	// コインセッター
	void SetCoinNum(const int& coinNum);
};

#endif // RESULTSCENE