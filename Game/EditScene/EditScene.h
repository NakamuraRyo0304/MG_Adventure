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
#include "Libraries/SystemDatas/MapLoad.h"
#include "Libraries/SystemDatas/Collider.h"
#include "System/EditorLog.h"
#include "System/MementoMap.h"

// エイリアス
using MAPSTATE = MapLoad::BOXSTATE;

class ClearChecker;
class EditSky;
class EditUI;
class MouseCursor;
class EditScene final : public IScene
{
private:

	// カーソル位置
	DirectX::SimpleMath::Vector3 m_cursorPos;

	// ブロックの配列
	std::vector<Object> m_mapObj;

	// 現在のブロックステータス
	int m_nowState;

	// パスの格納
	std::wstring m_filePath;

	// ブロックの数を保存する
	int m_blockCount[MAPSTATE::LENGTH];
private:

	// マップ
	MapLoad m_mapLoader;

	// 当たり判定
	std::unique_ptr<Collider::BoxCollider> m_boxCollider;

	// UI
	std::unique_ptr<EditUI> m_editUI;

	// スカイドーム
	std::unique_ptr<EditSky> m_editSky;

	// UndoRedo
	std::unique_ptr<EditorLog> m_editLog;

	// クリアチェッカー
	std::unique_ptr<ClearChecker> m_checker;

	// マウスカーソル
	std::unique_ptr<MouseCursor> m_mouseCursor;

private:
	// モデル
	std::unique_ptr<DirectX::Model>m_grassModel;		// 草モデル
	std::unique_ptr<DirectX::Model>m_noneModel;			// 消しゴムモデル
	std::unique_ptr<DirectX::Model>m_coinModel;			// コインモデル
	std::unique_ptr<DirectX::Model>m_cloudModel;		// 雲モデル
	std::unique_ptr<DirectX::Model>m_gravityModel;		// 雲リセットモデル
	std::unique_ptr<DirectX::Model>m_playerModel;		// プレイヤモデル

private:
	// マップサイズ(Stage)
	const float	COMMON_SIZE = 1.0f;

	// 最低高度
	const float COMMON_LOW = COMMON_SIZE / 2;

	// カメラアングル
	const float	CAMERA_ANGLE = 44.5f;

	// カメラの高さ
	const float CAMERA_HEIGHT = 3.0f;

	// ホイールのスパン
	const int WHEEL_SPAWN = 320;

	// カーソルの移動速度
	const float CURSOR_MOVE_SPEED = 0.1f;

	// カーソルの最低高度と最高高度
	const float CURSOR_MIN = -2.0f;
	const float CURSOR_MAX = static_cast<float>(MapLoad::MAP_HEIGHT + 0.5f);

	// スカイドームの回転量
	const float SKY_ROTATE_RATE = 0.01f;

	// デバッグ文字の位置
	const int DEB_X = 25;
	const int DEB_Y = 200;

	// デバッグ文字サイズ
	const DirectX::SimpleMath::Vector2 DEB_SIZE = DirectX::SimpleMath::Vector2{ 1.5f, 1.5f };

public:

	EditScene();
	~EditScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize() override;

	/// <summary>
	/// 画面依存、デバイス依存の初期化
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateWindowDependentResources() override;

private:

	/// <summary>
	/// シーン変数初期化関数
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void SetSceneValues() override;

	/// <summary>
	/// モデル作成を行う関数
	/// </summary>
	/// <param name="fm">ファクトリマネージャのポインタ</param>
	/// <returns>なし</returns>
	void CreateModels(std::shared_ptr<FactoryManager> fm);

	/// <summary>
	/// モデルの数をリセット
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void ResetObjNum();
	/// <summary>
	/// モデルの数を数える
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CountObjNum();
private:
	/// <summary>
	/// アイコンをクリック
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void ClickIcons();

	/// <summary>
	/// 描画オブジェクト切り替え
	/// </summary>
	/// <param name="objNum">オブジェクト番号</param>
	/// <param name="context">デバイスコンテキスト</param>
	/// <param name="states">コモンステート</param>
	/// <param name="world">ワールド行列</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">射影行列</param>
	/// <returns>なし</returns>
	void SwitchDraw(const int& objNum, ID3D11DeviceContext* context, DirectX::CommonStates& states,
		DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	/// <summary>
	/// マップの編集
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void EditMap();

	/// <summary>
	/// マップ読み込み
	/// </summary>
	/// <param name="filename">ファイルパス</param>
	/// <returns>なし</returns>
	void LoadMap(std::wstring filename);

	/// <summary>
	/// マップ書き出し
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void SaveFile();
	/// <summary>
	/// セーブ可能かを確認する
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	bool IsCanSave();

private:

	/// <summary>
	/// UndoRedoを実行する
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void DoUndoRedo();

	/// <summary>
	/// UndoRedo用に保存する
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void SaveModification();

	/// <summary>
	/// UndoRedoを適用する
	/// </summary>
	/// <param name="mement"></param>
	/// <returns>なし</returns>
	void RestoreHistory(MementoMap mement);
};

#endif // EDITSCENE