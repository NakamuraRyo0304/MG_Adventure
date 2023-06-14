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
class PlayScene : public IScene
{
private:
	// プレイヤ
	DirectX::SimpleMath::Vector3 m_playerPos;
	// ブロック
	std::vector<Object> m_mapObj;

	// マップ
	MapLoad m_map;

	// プレイヤ
	std::unique_ptr<Player> m_player;

	// 当たり判定
	Collider::BoxCollider is_boxCol;
	std::vector<Object> m_colObjList;

	// モデル
	std::unique_ptr<DirectX::Model> m_grassModel, m_coinModel,m_clowdModel;

	// 重力
	float m_gravity;

	// マップサイズ(Stage)
	const float COMMON_SIZE = 1.0f;

	// 最低高度
	const float	COMMON_LOW = COMMON_SIZE / 2;

	// カメラアングル
	const float	CAMERA_ANGLE = 45.0f;

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

	// 当たり判定
	void DoBoxCollision();

	// 押し戻し処理
	void ApplyPushBack(const Object& obj);

	// デバッグ表示
	void DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// マップ読み込み
	void LoadMap(int num);

};

#endif // PLAYSCENE