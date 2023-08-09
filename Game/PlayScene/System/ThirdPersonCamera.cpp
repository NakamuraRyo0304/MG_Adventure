/*
 *	@File	ThurdPersonCamera.cpp
 *	@Brief	三人称視点のカメラ。
 *	@Date	2023-08-05
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ThirdPersonCamera.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
ThirdPersonCamera::ThirdPersonCamera()
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
ThirdPersonCamera::~ThirdPersonCamera()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="pos">基準点</param>
/// <param name="rotate">回転量</param>
/// <param name="distance">基準点からの距離</param>
/// <returns>なし</returns>
void ThirdPersonCamera::UpdateFollow(const SimpleMath::Vector3& pos, const SimpleMath::Quaternion& rotate,
	const SimpleMath::Vector3& distance)
{
	// カメラの目線の方向をプレイヤーの回転に合わせて計算
	SimpleMath::Vector3 forward =
		SimpleMath::Vector3::Transform(SimpleMath::Vector3::UnitZ, rotate);

	SimpleMath::Vector3 newPos = { pos.x,pos.y - 0.5f,pos.z };

	// カメラの位置と目線を計算
	SimpleMath::Vector3 eye =
		newPos + SimpleMath::Vector3::Transform(distance, rotate);
	SimpleMath::Vector3 target = eye - forward;

	// ビュー行列
	m_followView = SimpleMath::Matrix::CreateLookAt(eye, target, SimpleMath::Vector3::Up);
}
