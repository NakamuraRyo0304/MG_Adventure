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
class FontObject;
class SelectCamera;
class SelectSky;
class SelectUI;
class SelectScene final : public IScene
{
private:

	// ステージ番号/未開放ステージ数
	int m_stageNum, m_safeNum;

	// 合計コイン数
	int m_allCoins;

	// ブロックとステージ
	std::unique_ptr<Blocks> m_blocks[6];

	// 切り替え時読み込み演出
	float m_targetY;

	// ブロックのローディング
	std::future<void> m_loadTask;

	// UI
	std::unique_ptr<SelectUI> m_selectUI;

	// ステージ番号
	std::unique_ptr<FontObject> m_fontObject;

	// スカイドーム
	std::unique_ptr<SelectSky> m_skyDome;

	// カメラ
	std::unique_ptr<SelectCamera> m_selectCamera;

	// コイン使用演出
	float m_useCoins;
	int m_initCoins;
	bool is_selectEdit;

private:
	// 定数-----------------------------------------------------------------------------------

	const float MAX_FLASH = 180.0f;

	// 最大ステージ数
	const int MAX_STAGE_NUM = 6;

	// ステージ制作価格
	const int STAGE_CREATE_PRICE = 10;

	// カウントスピード
	const float COUNT_SPEED = (10.0f / 60.0f);

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	/// <param name="safeNum">未開放ステージ数</param>
	/// <param name="coins">コインの数</param>
	/// <returns>なし</returns>
	SelectScene(const int& stageNum,const int& safeNum,const int& coins);
	~SelectScene();

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

private:

	/// <summary>
	/// ステージモデルのロード
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateStages();
	void CreateFirstStage();

	/// <summary>
	/// ステージの選択
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void ChangeStageNumber();

public:
	// ステージ番号の取得
	const int& GetStageNum() { return m_stageNum; }
	// ステージ番号の設定
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }
	// 未開放ステージの設定
	void SetSafeStages(const int& stageNum) { m_safeNum = stageNum; }
	// 合計コイン数の取得
	const int& GetAllCoins() { return m_allCoins; }
	// 合計コイン数の設定
	void SetAllCoins(const int& num) { m_allCoins = num; }
};

#endif // SELECTSCENE