/*
 *	@File	EditUI.h
 *	@Brief	UIの表示。
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef EDITUI
#define EDITUI

// 構造体を使用するためのインクルード
#include "Libraries/SystemDatas/MapLoad.h"
#include "Libraries/SystemDatas/Collider.h"
#include "Libraries/SystemDatas/DrawSprite.h"

// エイリアスを宣言
using MAPSTATE = MapLoad::BOXSTATE;

class SystemManager;
class EditUI
{
private:

	// シェアポインタ
	std::shared_ptr<SystemManager> m_system;

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// フルスクリーンサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// 現在のステータス
	int m_nowState;

private:

	// フラグ
	bool is_anyHitFlag;
	bool is_saveFlag;
	bool is_openFlag;
	bool is_cameraFlag;
	bool is_goPlayFlag;
	bool is_toolFlag;
	bool is_backFlag;
	bool is_boxState[MAPSTATE::LENGTH];
private:

	// 画像の座標
	DirectX::SimpleMath::Vector2 m_toolTexPos[4];
	DirectX::SimpleMath::Vector2 m_toolButtonTexPos;
	DirectX::SimpleMath::Vector2 m_backTexPos;
	DirectX::SimpleMath::Vector2 m_imagePos[MAPSTATE::LENGTH];

	// テクスチャの切り取りパス
	enum Cut256 { _0 = 0, _1 = 256, _2 = 512, _3 = 768, _4 = 1024, _5 = 1280 };

	// 画像切り取り位置
	RECT_U m_texRect[MAPSTATE::LENGTH];
	RECT_U m_modeRect[3];
	RECT_U m_toolRect[5];

	// 拡大
	float m_boxHover[MAPSTATE::LENGTH];

private:

	// 画像の中心位置
	const float	FONT_HEIGHT = 128.0f;

	// 画像サイズ
	const float IMAGE_RATE = 0.55f;
	const float HALF = 0.5f;
	const float BAR_HEIGHT = 170.0f;
	const RECT_U DO_BAR_RECT = { 0L,0L,1420L,150L };

	// 当たり判定オブジェクト
	Collider::AABBCollider m_imageHitter;

	// 判定サイズ
	const DirectX::SimpleMath::Vector2 MOUSE_SIZE = { 5.0f,5.0f };
	const DirectX::SimpleMath::Vector2 BACK_SIZE = { 80.0f,80.0f };
	const DirectX::SimpleMath::Vector2 ICON_SIZE = { 100.0f,100.0f };

public:

	EditUI();
	~EditUI();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="shareSystem">システムデータ</param>
	/// <param name="windowSize">ウィンドウサイズ</param>
	/// <returns>なし</returns>
	void Create(const std::shared_ptr<SystemManager>& system, const DirectX::SimpleMath::Vector2& windowSize);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Render();

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize();

	/// <summary>
	/// アイコンの描画
	/// </summary>
	/// <param name="imageScale">拡大率</param>
	/// <returns>なし</returns>
	void DrawIcon(const DirectX::SimpleMath::Vector2& imageScale);

	/// <summary>
	/// ステータスの変更
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void ChangeState();

	// アクセサ
	// カメラフラグの取得
	const bool& GetCameraFlag() { return is_cameraFlag; }
	// カメラフラグの設定
	void SetCameraFlag(const bool flag) { is_cameraFlag = flag; }
	// ファイル開くフラグの取得
	const bool& GetOpenFlag() { return is_openFlag; }
	// ファイル開くフラグの設定
	void SetOpenFlag(const bool flag) { is_openFlag = flag; }
	// 保存フラグの取得
	const bool& GetSaveFlag() { return is_saveFlag; }
	// プレイシーンに行くフラグの取得
	const bool& GetGoPlayFlag() { return is_goPlayFlag; }
	// 保存フラグの設定
	void SetSaveFlag(const bool flag) { is_saveFlag = flag; }
	// セレクトへ戻るフラグの設定
	const bool& GetBackSelectFlag() { return is_backFlag; }
	// 今のステータスを取得
	const int& GetNowState() { return m_nowState; }
};

#endif // EDITUI