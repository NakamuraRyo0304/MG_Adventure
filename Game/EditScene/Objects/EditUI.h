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
	bool is_toolFlag;
	bool is_backFlag;
	bool is_boxState[MAPSTATE::LENGTH];
private:

	// 画像の座標
	DirectX::SimpleMath::Vector2 m_toolTexPos[3];
	DirectX::SimpleMath::Vector2 m_toolButtonTexPos;
	DirectX::SimpleMath::Vector2 m_backTexPos;
	DirectX::SimpleMath::Vector2 m_imagePos[MAPSTATE::LENGTH];

	// テクスチャの切り取りパス
	enum Cut256 { _0 = 0, _1 = 256, _2 = 512, _3 = 768, _4 = 1024 };

	// 画像切り取り位置
	RECT_U m_texRect[MAPSTATE::LENGTH];
	RECT_U m_modeRect[3];
	RECT_U m_saveRect[4];

	// 拡大
	float m_boxHover[MAPSTATE::LENGTH];

private:

	// 画像の中心位置
	const float	IMAGE_CENTER = 128.0f;

	// 画像サイズ
	const float IMAGE_RATE = 0.55f;
	const float HALF = 0.5f;
	const float BAR_HEIGHT = 170.0f;
	const RECT_U DO_BAR_RECT = { 0L,0L,1420L,150L };

	// 当たり判定オブジェクト
	Collider::AABBCollider m_imageHitter;

public:

	EditUI();
	~EditUI();

	void Create(const std::shared_ptr<SystemManager>& system, ID3D11Device1* device, const SimpleMath::Vector2& windowSize);
	void Update(DirectX::Mouse::State& mouseState);
	void Render();
	void Finalize();

	// アイコンの描画
	void DrawIcon(const DirectX::SimpleMath::Vector2& imageScale);

	// ステータスの変更
	void ChangeState(DirectX::Mouse::State& mouseState);

	// アクセサ
	// カメラフラグ
	const bool& GetCameraFlag() { return is_cameraFlag; }
	void SetCameraFlag(const bool flag) { is_cameraFlag = flag; }

	// オープンフラグ
	const bool& GetOpenFlag() { return is_openFlag; }
	void SetOpenFlag(const bool flag) { is_openFlag = flag; }

	// セーブフラグ
	const bool& GetSaveFlag() { return is_saveFlag; }
	void SetSaveFlag(const bool flag) { is_saveFlag = flag; }

	// バックボタンフラグ
	const bool& GetBackSelectFlag() { return is_backFlag; }

	// 今のステータスを取得
	const int& GetNowState() { return m_nowState; }
};

#endif // EDITUI