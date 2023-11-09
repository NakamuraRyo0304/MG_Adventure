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

	// アニメーションフラグ
	bool is_animEnd;

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

	ResultScene(const int& stageNum, const int& coins, const float& clearTime);
	~ResultScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize() override;

	/// <summary>
	/// 画面依存、デバイス依存の初期化
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateWindowDependentResources() override;

private:

	/// <summary>
	/// シーン内の変数初期化関数
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void SetSceneValues() override;

	/// <summary>
	/// モデル作成を行う関数
	/// </summary>
	/// <param name="fm">ファクトリマネージャのポインタ</param>
	/// <returns>なし</returns>
	void CreateModels(std::shared_ptr<FactoryManager> fm);

	/// <summary>
	/// 数字のアニメーション
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>終わったらTrue</returns>
	bool AnimationValue();

	/// <summary>
	/// シーン切り替え処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void NextScene();

public:
	// マップ番号の取得
	const int& GetStageNum() { return m_stageNum; }
};

#endif // RESULTSCENE