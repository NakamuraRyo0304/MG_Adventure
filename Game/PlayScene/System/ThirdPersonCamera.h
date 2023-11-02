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

private:
	// フルスクリーンサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// 画像用にシステムを貰う
	std::shared_ptr<SystemManager> m_system;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="system">システムマネージャ</param>
	/// <returns>なし</returns>
	ThirdPersonCamera(std::shared_ptr<SystemManager> system);
	~ThirdPersonCamera();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="pos">基準点</param>
	/// <param name="rotate">回転量</param>
	/// <param name="distance">基準点からの距離</param>
	/// <returns>なし</returns>
	void UpdateFollow(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Quaternion& rotate ,
		const DirectX::SimpleMath::Vector3& distance);

	/// <summary>
	/// 雲の付着を描画
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void DrawAdhesion();

	// ビュー行列の取得
	const DirectX::SimpleMath::Matrix& GetFollowView() { return m_followView; }
};

#endif // THIRDPERSONCAMERA