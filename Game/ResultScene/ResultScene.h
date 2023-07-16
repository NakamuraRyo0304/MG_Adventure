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
class ResultScene : public IScene
{
private:
	// モード
	enum { RETRY, SELECT, TITLE };

	// タイマー
	float m_timer;

	// セレクト
	int m_selectNum;

	// クリアタイム
	float m_clearTime;

	// ブロック
	std::unique_ptr<Blocks> m_blocks;

	// マップ番号
	int m_stageNum;
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

};

#endif // RESULTSCENE