/*
 *	@File	EditScene.h
 *	@Brief  エディットシーン。
 *	@Date	2023-09-24
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef EDITSCENE
#define EDITSCENE

#include "../IScene.h"

 // CSV読み込み
#include "../../Libraries/SystemDatas/MapLoad.h"

// 当たり判定クラス
#include "../../Libraries/SystemDatas/Collider.h"

// ログ管理クラス
#include "System/EditorLog.h"

// メメント
#include "System/MementMap.h"

// エイリアス
using MAPSTATE = MapLoad::BOXSTATE;

class ClearChecker;
class EditUI;
class MouseCursor;
class EditScene final : public IScene
{
private:
	// タイマー
	float m_timer;

	// カーソル位置
	DirectX::SimpleMath::Vector3 m_cursorPos;

	// ブロックの配列
	std::vector<Object> m_mapObj;

	// 現在のブロックステータス
	int m_nowState;

	// パスの格納
	std::wstring m_filePath;

	// ファイル読み込みモード
	enum MODE { WRITE, READ };

private:

	// マップ
	MapLoad m_mapLoader;

	// 当たり判定
	Collider::BoxCollider is_hitCol;

	// UI
	std::unique_ptr<EditUI> m_userInterface;

	// クリアチェッカー
	std::unique_ptr<ClearChecker> m_checker;

	// マウスカーソル
	std::unique_ptr<MouseCursor> m_mouseCursor;

private:

	// UndoRedo
	EditorLog m_history;

private:
	// モデル
	std::unique_ptr<DirectX::Model>m_grassModel;		// 草モデル
	std::unique_ptr<DirectX::Model>m_noneModel;			// 消しゴムモデル
	std::unique_ptr<DirectX::Model>m_coinModel;			// コインモデル
	std::unique_ptr<DirectX::Model>m_cloudModel;		// 雲モデル
	std::unique_ptr<DirectX::Model>m_gravityModel;		// 雲リセットモデル
	std::unique_ptr<DirectX::Model>m_skyDomeModel;		// スカイドームモデル
	std::unique_ptr<DirectX::Model>m_playerModel;		// プレイヤモデル

private:
	// マップサイズ(Stage)
	const float	COMMON_SIZE = 1.0f;

	// 最低高度
	const float COMMON_LOW = COMMON_SIZE / 2;

	// カメラアングル
	const float	CAMERA_ANGLE = 44.5f;

	// ホイールのスパン
	const int WHEEL_SPAWN = 320;

	// カーソルの移動速度
	const float CURSOR_MOVE_SPEED = 0.1f;

	// カーソルの最低高度と最高高度
	const float CURSOR_MIN = -2.0f;
	const float CURSOR_MAX = static_cast<float>(MapLoad::MAP_HEIGHT + 0.5f);

	// スカイドームの回転量
	const float SKY_ROTATE_RATE = 0.01f;
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

	// Switch簡略文
	void SwitchDraw(const int& objNum, ID3D11DeviceContext* context, DirectX::CommonStates& states,
		DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// 終了処理
	void Finalize() override;

	// 画面依存の初期化
	void CreateWindowDependentResources() override;

private:

	// シーン内の変数初期化関数
	void SetSceneValues() override;

private:

	// マップを編集
	void EditMap();

	// 座標補正関数
	void OffsetPosition(std::vector<Object>* obj , const MODE& mode);

	// マップ読み込み
	void LoadMap(std::wstring filename);

	// ファイルをセーブする
	void SaveFile();

	// 要素チェック
	bool IsCanSave();

private:

	// UndoRedoの実行をする
	void DoUndoRedo();

	// 変更があればUndoRedo用に一時保存する
	void SaveModification();

	// UndoRedoを適用する
	void RestoreHistory(MementMap mement);
};

#endif // EDITSCENE