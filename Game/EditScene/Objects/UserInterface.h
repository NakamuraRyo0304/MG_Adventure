/*
 *	@File	UserInterface.h
 *	@Brief	UIの表示。
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef USERINTERFACE
#define USERINTERFACE

#include "Libraries/SystemDatas/Collider.h"

#include "../../../Libraries/SystemManager/SystemManager.h"

class UserInterface
{
private:
	// シェアポインタ
	std::shared_ptr<SystemManager> m_systemManager;


	// 画像の座標
	DirectX::SimpleMath::Vector2 m_saveTexPos;
	DirectX::SimpleMath::Vector2 m_openTexPos;
	DirectX::SimpleMath::Vector2 m_cameraTexPos;
	DirectX::SimpleMath::Vector2 m_penTexPos;

	// フラグ
	bool is_saveFlag;
	bool is_openFlag;
	bool is_cameraFlag;
	bool is_drawFlag;

	// 画像の中心位置
	const float	IMAGE_CENTER = 128;

	Collider::AABBCollider m_aabbCol;

public:
	UserInterface();
	~UserInterface();

	void Initialize(std::shared_ptr<SystemManager> shareSystem,ID3D11DeviceContext1* context,ID3D11Device1* device);
	void Update(DirectX::Mouse::State& mouseState);
	void Render();



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

	// 描画フラグ
	const bool& GetDrawFlag() { return is_drawFlag; }
	void SetDrawFlag(const bool flag) { is_drawFlag = flag; }
};

#endif // USERINTERFACE