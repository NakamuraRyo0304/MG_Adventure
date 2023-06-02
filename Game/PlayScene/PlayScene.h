/*
 *	@File	PlayScene.h
 *	@Brief  プレイシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYSCENE
#define PLAYSCENE

#include "../IScene.h"

// CSV読み込み
#include "../../Libraries/SystemDatas/MapLoad.h"

#include "../../Libraries/SystemDatas/Collider.h"

class PlayScene : public IScene
{
private:
	// ボックスのモデル
	std::unique_ptr<DirectX::GeometricPrimitive> m_sphere;
	DirectX::SimpleMath::Vector3 m_boxesPos[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];

	// マップリーダー
	MapLoad m_map;

	// マップ用配列
	int m_mapData[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN] = { 0 };

	DirectX::SimpleMath::Vector3 m_spherePos;

	float m_size;

	std::unique_ptr<DirectX::Model>m_boxModel;

public:

	// コンストラクタ
	PlayScene();

	// デストラクタ
	~PlayScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const float& elapsedTime,DirectX::Keyboard::State& keyState,DirectX::Mouse::State& mouseState) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	// 画面依存の初期化
	void CreateWindowDependentResources() override;

	// デバッグ表示
	void DebugLog(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);

	// マップ読み込み
	void LoadMap(int num);

};

#endif // PLAYSCENE