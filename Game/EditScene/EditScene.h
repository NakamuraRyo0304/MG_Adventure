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

// エイリアス
using MapState = MapLoad::BoxState;

class UserInterface;
class EditScene : public IScene
{
private:
	// タイマー
	float m_timer;

	// ファイル読み込みモード
	enum { WRITE, READ };

	// カーソル位置
	DirectX::SimpleMath::Vector3 m_cursorPos;
	const float CURSOR_MIN = -2.0f;
	const float CURSOR_MAX = 15.0f;

	// ブロックの配列
	std::vector<Object> m_mapObj;

	// マップ
	MapLoad m_map;

	// 当たり判定
	Collider::BoxCollider is_boxCol;

	// UI
	std::unique_ptr<UserInterface> m_userInterface;
	std::shared_ptr<SystemManager> m_sharedSystem;

	// モデル
	std::unique_ptr<DirectX::Model>m_grassModel;		// 草モデル
	std::unique_ptr<DirectX::Model>m_noneModel;			// 消しゴムモデル
	std::unique_ptr<DirectX::Model>m_coinModel;			// コインモデル
	std::unique_ptr<DirectX::Model>m_moveModel;			// 動くモデル
	std::unique_ptr<DirectX::Model>m_switchModel;		// スイッチモデル
	std::unique_ptr<DirectX::Model>m_playerModel;		// プレイヤモデル

	// 現在のブロックステータス
	int m_nowState;

	// パスの格納
	std::wstring m_filePath;

private:
	// マップサイズ(Stage)
	const float	COMMON_SIZE = 1.0f;

	// 最低高度
	const float COMMON_LOW = COMMON_SIZE / 2; 

	// カメラアングル
	const float	CAMERA_ANGLE = 44.5f;

	// ホイールのスパン
	const int WHEEL_SPAWN = 640;
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

	// 座標補正関数
	void OffsetPosition(std::vector<Object>* obj , int mode);
	
	// マップ読み込み
	void LoadMap(std::wstring filename);

	// ファイルをセーブする
	void SaveFile();

};

#endif // EDITSCENE