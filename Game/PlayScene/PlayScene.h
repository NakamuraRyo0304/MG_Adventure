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
class PlayerShadow;
class PlayUI;
class Blocks;
class ThirdPersonCamera;
class PlayScene : public IScene
{
private:
	// タイマー
	float m_timer;

	// スタートタイマー
	float m_startTimer;

	// 制限時間
	float m_timeLimit;
	float m_returnTimeVal;

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

	// プレイヤ
	std::unique_ptr<Player> m_player;

	// ブロック
	std::unique_ptr<Blocks> m_blocks;

	// 落下演出変数
	float m_fallValue;

	// 当たり判定
	Collider::BoxCollider is_boxCol;
	std::vector<Object> m_hitObjects;

	// 最後に当たったオブジェクト
	Object m_lastObj;

	// 前回のインデックス
	std::deque<int> m_prevIndex;

private:
	// モデル
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// プレイヤーの目印（ビルボード）
	std::unique_ptr<PlayerBill> m_playerBill;

	// タイマー表示とコイン表示
	std::unique_ptr<PlayUI> m_userInterFace;

	// プレイヤーの影シェーダー
	std::unique_ptr<PlayerShadow> m_playerShadow;

	// サードパーソンカメラ
	std::unique_ptr<ThirdPersonCamera> m_thirdCamera;

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

private:
	// サードパーソンモード
	bool is_thirdPersonMode;

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

	// 当たり判定処理
	void Judgement();

	// 押し戻し処理
	void ApplyPushBack(Object& obj);

	// 空の更新
	void UpdateSky();

	// スタートのカウントダウン
	bool StartTimer();

	// タイムリミットのゲッター
	const float& GetTimeLimit() { return m_returnTimeVal; }

	// コインの枚数ゲッター
	const int& GetCoinNum();

	// マップ番号のセッター
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }

};

#endif // PLAYSCENE