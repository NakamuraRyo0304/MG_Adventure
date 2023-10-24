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

// 当たり判定用
#include <deque>

class Player;
class PlayerBill;
class PlayUI;
class PlayCamera;
class Blocks;
class ThirdPersonCamera;
class PlayScene final : public IScene
{
private:
	// タイマー
	float m_timer;

	// スタートタイマー
	float m_startTimer;

	// 制限時間
	float m_gameTimer;
	float m_clearTime;

	// 空の色遷移
	struct Color
	{
		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;
	};
	Color m_skyColor;

	// マップ
	MapLoad m_mapLoad;
	// マップ番号
	int m_stageNum;

	// 落下演出変数
	float m_fallValue;

	// 当たり判定
	Collider::BoxCollider is_hitCol;
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

	// プレイヤ
	std::unique_ptr<Player> m_player;

	// ブロック
	std::unique_ptr<Blocks> m_blocks;

	// モデル
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// プレイヤーの目印（ビルボード）
	std::unique_ptr<PlayerBill> m_playerBill;

	// タイマー表示とコイン表示
	std::unique_ptr<PlayUI> m_userInterFace;

	// サードパーソンカメラ
	std::unique_ptr<ThirdPersonCamera> m_thirdCamera;

	// スタートカメラ
	std::unique_ptr<PlayCamera> m_playCamera;

private:

	// 制限時間(秒数)
	const int TIME_LIMIT = 60;

	// フレームレート
	const float FLAME_RATE = 60.0f;

	// スカイドームの回転速度
	const float SKY_ROT_SPEED = 0.02f;

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

public:

	// コンストラクタ
	PlayScene();

	// デストラクタ
	~PlayScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState, DirectX::Mouse::State& mouseState) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	// 画面依存の初期化
	void CreateWindowDependentResources() override;

private:

	// シーン内の変数初期化関数
	void SetSceneValues() override;

private:

	// 当たり判定処理
	void Judgement();

	// 押し戻し処理
	void ApplyPushBack(Object& obj);

	// ライティングの設定
	void InitializeLighting();

	// 空の更新
	void UpdateSky();

	// スタートのカウントダウン
	bool StartTimer();

	// スタート演出
	void MoveStart();

	// プレイヤーの作成
	void MakePlayer(ID3D11Device1* device);

public:

	// タイムリミットのゲッター
	const float& GetClearTime() { return m_clearTime; }

	// コインの枚数ゲッター
	const int& GetCoinNum();
	const int& GetMaxCoinCount();

	// マップ番号のセッター
	const int& GetStageNum() { return m_stageNum; }
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }

	// 合計コイン数セッター
	void SetAllCoinNum(const int& allCoins) { m_allCoins = allCoins; }

};

#endif // PLAYSCENE