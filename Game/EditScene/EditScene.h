/*
 *	@File	EditScene.h
 *	@Brief  エディットシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef EDITSCENE
#define EDITSCENE

#include "../IScene.h"

// CSV読み込み
#include "../../Libraries/SystemDatas/MapLoad.h"

#include "../../Libraries/SystemDatas/Collider.h"

struct EditObject
{
	DirectX::SimpleMath::Vector3 position;		// 座標
	bool hitFlag;								// 当たり判定フラグ
	UINT state;									// マップのステータス
};

class EditScene : public IScene
{
private:
	// ボックスとスフィア
	std::unique_ptr<DirectX::GeometricPrimitive> m_sphere,m_box;
	DirectX::SimpleMath::Vector3 m_spherePos;
	EditObject m_obj[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];

	// マップ
	MapLoad m_map;

	// 当たり判定
	Collider::BoxCollider m_boxCol;
	Collider::AABBCollider m_aabbCol;

	// モデル
	std::unique_ptr<DirectX::Model>m_grassBlockModel,m_grassBlockModel_D,m_coinModel;
	
	// 画像の座標
	DirectX::SimpleMath::Vector2 m_saveTexPos;
	DirectX::SimpleMath::Vector2 m_cameraTexPos;
	DirectX::SimpleMath::Vector2 m_penTexPos;

	// フラグ
	bool is_saveFlag;
	bool is_cameraFlag;
	bool is_upFlag;

	// 現在のブロックステータス
	int m_nowState;

public:

	// コンストラクタ
	EditScene();

	// デストラクタ
	~EditScene();

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

	// ステータスを変更する
	void ChangeState(UINT State);

	// マップを編集
	void EditMap(DirectX::Keyboard::State& keyState);

	// マップ読み込み
	void LoadMap(int num);

	// ファイルをセーブする
	void SaveFile();

	// 画像の描画
	void DrawImages();

	// UIのクリック
	void ClickUserInterface(DirectX::Mouse::State& mouseState);

};

#endif // EDITSCENE