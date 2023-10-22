/*
 *	@File	ThurdPersonCamera.h
 *	@Brief	三人称視点のカメラ。
 *	@Date	2023-08-05
 *  @Author NakamuraRyo
 */

#include "Libraries/SystemDatas/Camera.h"

#pragma once
#ifndef THIRDPERSONCAMERA
#define THIRDPERSONCAMERA

class SystemManager;
class ThirdPersonCamera final : public Camera
{
private:

	// 追従用ビュー行列
	DirectX::SimpleMath::Matrix m_followView;

	// 画像更新用変数
	float m_adhesionTimer;
	const float CHANGE_SPAN = 120.0f;
	bool is_changeFlag;

private:
	// フルスクリーンサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// 画像用にシステムを貰う
	std::shared_ptr<SystemManager> m_system;

public:
	ThirdPersonCamera(std::shared_ptr<SystemManager> system,ID3D11DeviceContext1* context, ID3D11Device1* device);
	~ThirdPersonCamera();

	// 追従処理
	void UpdateFollow(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Quaternion& rotate ,
		const DirectX::SimpleMath::Vector3& distance, const float& timer);

	// ビュー行列の取得
	const DirectX::SimpleMath::Matrix& GetFollowView() { return m_followView; }

	// 画像の描画
	void DrawAdhesion();
};

#endif // THIRDPERSONCAMERA