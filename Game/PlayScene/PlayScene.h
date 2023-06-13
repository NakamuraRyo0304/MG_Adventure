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

 // CSV読み込み
#include "../../Libraries/SystemDatas/MapLoad.h"

#include "../../Libraries/SystemDatas/Collider.h"

struct Object
{
	DirectX::SimpleMath::Vector3 position;		// 座標
	bool hitFlag;								// 当たり判定フラグ
	UINT state;									// マップのステータス
};

class PlayScene : public IScene
{
private:
	// プレイヤ
	DirectX::SimpleMath::Vector3 m_playerPos;
	// ボックス
	Object m_obj[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];

	// マップ
	MapLoad m_map;

	// 当たり判定
	Collider::BoxCollider m_boxCol;

	// モデル
	std::unique_ptr<DirectX::Model> m_player, m_grassBox;

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

	// デバッグ表示
	void DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// マップ読み込み
	void LoadMap(int num);

};

#endif // PLAYSCENE