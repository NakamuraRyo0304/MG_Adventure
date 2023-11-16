/*
 *	@File	PlayScene.h
 *	@Brief  プレイシーン。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYSCENE
#define PLAYSCENE

#include "../IScene.h"

class Blocks;
class Player;
class PlayerBill;
class PlayUI;
class PlayCamera;
class PlaySky;
class ThirdPersonCamera;
class PlayScene final : public IScene
{
private:

	// スタートタイマー
	float m_startTimer;

	// 制限時間
	float m_gameTimer;
	float m_clearTime;

	// マップ番号
	int m_stageNum;

	// 落下演出変数
	float m_fallValue;

	// オブジェクトの判定
	std::vector<Object> m_hitObj;
	Object m_lastObj;

	// 前回のインデックス
	std::deque<int> m_prevIndex;

	// ヘルプ表示フラグ
	bool is_helpFlag;

	// サードパーソンモード
	bool is_thirdPersonMode;

	// 合計コイン数
	int m_allCoins;

	// ライティング
	DirectX::SimpleMath::Vector3 m_lighting;

private:

	// 当たり判定
	std::unique_ptr<Collider::BoxCollider> m_boxCollider;

	// プレイヤ
	std::unique_ptr<Player> m_player;

	// ブロック
	std::unique_ptr<Blocks> m_blocks;

	// プレイヤーの目印（ビルボード）
	std::unique_ptr<PlayerBill> m_playerBill;

	// タイマー表示とコイン表示
	std::unique_ptr<PlayUI> m_playUI;

	// サードパーソンカメラ
	std::unique_ptr<ThirdPersonCamera> m_thirdCamera;

	// スタートカメラ
	std::unique_ptr<PlayCamera> m_camera;

	// スカイドーム
	std::unique_ptr<PlaySky> m_playSky;

private:

	// 制限時間(秒数)
	const int TIME_LIMIT = 60;

	// フレームレート
	const float FLAME_RATE = 60.0f;

	// マップサイズ(Stage)
	const float COMMON_SIZE = 0.9f;

	// 最低高度
	const float	COMMON_LOW = COMMON_SIZE / 2;

	// 当たり判定の半径
	const float JUDGE_AREA = COMMON_SIZE * 1.5f;

	// カメラアングル
	const float	CAMERA_ANGLE = 44.5f;

	// ブロック崩壊ライン
	const float DURATION_FLOOR_LINE = -15.0f;

	// 死亡後落下速度
	const float FALL_SPEED = 0.005f;

	// 振動設定
	const float SHAKE_DURATION = 4.0f;		// 持続時間
	const float SHAKE_TREMOR = 1.0f;		// 振動範囲

	// カウントダウンの速度
	const float COUNT_SPEED = 0.5f;

	// 最初のカメラ演出終点
	const float MOVE_CAMERA_SPEED = 0.05f;
	const DirectX::SimpleMath::Vector3 START_CAMERA_POS = { 0.0f,50.0f,40.0f };

	// 三人称カメラの距離
	const DirectX::SimpleMath::Vector3 THIRD_DISTANCE = DirectX::SimpleMath::Vector3(0.0f, 2.5f, 6.0f);

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	/// <param name="coins">コインの数</param>
	/// <returns>なし</returns>
	PlayScene(const int& stageNum,const int& coins);
	~PlayScene();

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
	/// ライティングの更新
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void InitializeLighting();

	/// <summary>
	/// プレイヤーの作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void MakePlayer();

	/// <summary>
	/// ブロックの作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void MakeBlocks();

	/// <summary>
	/// スタートのカウントダウン
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>終わっていたらTrueを返す</returns>
	bool StartTimer();

	/// <summary>
	/// スタート演出
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void MoveStart();

	/// <summary>
	/// UIの更新
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>ヘルプフラグ</returns>
	bool UpdateUI();

	/// <summary>
	/// ヘルプシーンで選択されたシーンへの遷移
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void HelpNext();

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Judgement();

	/// <summary>
	/// 押し戻し処理
	/// </summary>
	/// <param name="obj">当たったオブジェクト</param>
	/// <returns>なし</returns>
	void ApplyPushBack(Object& obj);

public:

	// タイムリミットの取得
	const float& GetClearTime() { return m_clearTime; }
	/// 獲得したコインを取得
	const int& GetCoinNum();
	/// ステージの最大コインを取得
	const int& GetMaxCoinCount();
	// マップ番号の取得
	const int& GetStageNum() { return m_stageNum; }

};

#endif // PLAYSCENE