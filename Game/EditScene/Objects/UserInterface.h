/*
 *	@File	UserInterface.h
 *	@Brief	UIの表示。
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef USERINTERFACE
#define USERINTERFACE

#include "Libraries/SystemDatas/MapLoad.h"
#include "Libraries/SystemDatas/Collider.h"

// エイリアスを宣言
using MapState = MapLoad::BOXSTATE;

class SystemManager;
class UserInterface
{
private:

	// シェアポインタ
	std::shared_ptr<SystemManager> m_system;

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// フルスクリーンサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// 画像の座標
	DirectX::SimpleMath::Vector2 m_saveTexPos;
	DirectX::SimpleMath::Vector2 m_openTexPos;
	DirectX::SimpleMath::Vector2 m_cameraTexPos;
	DirectX::SimpleMath::Vector2 m_toolButtonTexPos;
	DirectX::SimpleMath::Vector2 m_backTexPos;

	// 現在のステータス
	int m_nowState;

	// フラグ
	bool is_saveFlag;
	bool is_openFlag;
	bool is_cameraFlag;
	bool is_toolFlag;
	bool is_backFlag;

private:
	// ボックスステータスの配列
	const wchar_t* m_texName[MapState::LENGTH];					// テクスチャ名
	bool is_boxState[MapState::LENGTH];							// 現在のステート
	float m_boxHover[MapState::LENGTH];							// ホバーチェック
	DirectX::SimpleMath::Vector2 m_imagePos[MapState::LENGTH];	// テクスチャの座標

	// 画像の中心位置
	const float	IMAGE_CENTER = 128.0f;

	// 画像サイズ
	const float IMAGE_RATE = 0.55f;
	const float HALF = 0.5f;

	// 当たり判定オブジェクト
	Collider::AABBCollider m_imageHitter;

public:

	UserInterface(const DirectX::SimpleMath::Vector2& windowSize);
	~UserInterface();

	void Initialize(std::shared_ptr<SystemManager> shareSystem,ID3D11DeviceContext1* context,ID3D11Device1* device);
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

#endif // USERINTERFACE