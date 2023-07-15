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
class Blocks;
class PlayScene : public IScene
{
private:
	// タイマー
	float m_timer;

	// 制限時間
	float m_timeLimit;
	float m_returnTimeVal;

	// マップ
	MapLoad m_mapLoad;
	// マップ番号
	int m_mapNum;

	// プレイヤ
	std::unique_ptr<Player> m_player;

	// ブロック
	std::unique_ptr<Blocks> m_blocks;

	// 落下演出変数
	float m_fallValue;

	// 当たり判定
	Collider::BoxCollider is_boxCol;
	std::vector<Object> m_colObjList;
	std::deque<int> m_prevIndex;

	// モデル
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// プレイヤーの目印（ビルボード）
	std::unique_ptr<PlayerBill> m_playerBill;

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
	const float JUDGE_AREA = 1.8f;

	// カメラアングル
	const float	CAMERA_ANGLE = 44.5f;

	// ブロック崩壊ライン
	const float DURATION_FLOOR_LINE = -7.5f;

	// 死亡後落下速度
	const float FALL_SPEED = 0.002f;

	// 振動設定
	const float SHAKE_DURATION = 4.0f;		// 持続時間
	const float SHAKE_TREMOR = 1.0f;		// 振動範囲


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

	// デバッグ表示
	void DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// タイムリミットのゲッター
	const float& GetTimeLimit() { return m_returnTimeVal; }

	// マップ番号のセッター
	void SetStageNum(const int& stageNum) { m_mapNum = stageNum; }

};

#endif // PLAYSCENE