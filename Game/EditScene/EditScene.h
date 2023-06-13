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
	int state;									// マップのステータス
};

class EditScene : public IScene
{
private:
	// ボックスとスフィア
	std::unique_ptr<DirectX::GeometricPrimitive> m_sphere,m_box;
	DirectX::SimpleMath::Vector3 m_spherePos;
	EditObject m_grassObj[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];
	EditObject m_coinObj[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];
	EditObject m_clowdObj[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];
	EditObject m_mapObj[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];

	// マップ
	MapLoad m_map;

	// 当たり判定
	Collider::BoxCollider m_boxCol;
	Collider::AABBCollider m_aabbCol;

	// モデル
	std::unique_ptr<DirectX::Model>m_grassBlockModel,m_grassBlockModel_D;	// 草モデル
	std::unique_ptr<DirectX::Model>m_coinModel;								// コインモデル
	std::unique_ptr<DirectX::Model>m_clowdModel;							// 雲モデル
	
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

private:
	// マップサイズ(Stage)
	const float	COMMON_SIZE = 1.0f;

	// 最低高度
	const float COMMON_LOW = COMMON_SIZE / 2;

	// カメラアングル
	const float	CAMERA_ANGLE = 45.0f;

	// 画像の中心位置
	const float	IMAGE_CENTER = 128;

	// ボックスの最大値と最小値
	const int	MIN_BOX = 1;
	const int	MAX_BOX = 15;

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
	void ChangeState(const int& State);

	// マップを編集
	void EditMap();

	// 当たり判定関数
	void ChoiceObj(EditObject (&obj)[15][15], int x, int y , int State);

	// 高さのクランプ
	void ClampHeight(int& states, int id);

	// マップ読み込み
	void LoadMap(std::wstring filename);

	// ファイルをセーブする
	void SaveFile();

	// ボックスを描画
	void DrawObj(EditObject(&obj)[15][15],int State,DirectX::SimpleMath::Matrix[2], std::unique_ptr<DirectX::Model>&&);

	// 画像の描画
	void DrawImages();

	// UIのクリック
	void ClickUserInterface(DirectX::Mouse::State& mouseState);

};

#endif // EDITSCENE