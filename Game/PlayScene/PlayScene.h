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

class Player;
class PlayerBill;
class Blocks;
class PlayScene : public IScene
{
private:
	// タイマー
	float m_timer;

	// ブロック
	std::vector<Object> m_mapObj;

	// マップ
	MapLoad m_map;

	// プレイヤ
	std::unique_ptr<Player> m_player;

	// ブロック
	std::unique_ptr<Blocks> m_blocks;

	// 当たり判定
	Collider::BoxCollider is_boxCol;
	std::vector<Object> m_colObjList;

	// モデル
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// プレイヤーの目印（ビルボード）
	std::unique_ptr<PlayerBill> m_playerBill;

	// スカイドームの回転速度
	const float SKY_ROT_SPEED = 0.02f;

private:
	// マップサイズ(Stage)
	const float COMMON_SIZE = 0.9f;

	// 最低高度
	const float	COMMON_LOW = COMMON_SIZE / 2;

	// 当たり判定の半径
	const float JUDGE_AREA = 1.8f;

	// カメラアングル
	const float	CAMERA_ANGLE = 44.5f;

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

};

#endif // PLAYSCENE