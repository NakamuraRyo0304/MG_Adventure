/*
 *	@File	ThurdPersonCamera.h
 *	@Brief	�O�l�̎��_�̃J�����B
 *	@Date	2023-08-05
 *  @Author NakamuraRyo
 */

#include "Libraries/SystemDatas/Camera.h"

#pragma once
#ifndef THIRDPERSONCAMERA
#define THIRDPERSONCAMERA

class ThirdPersonCamera final : public Camera
{
private:
	DirectX::SimpleMath::Matrix m_followView;
public:
	ThirdPersonCamera();
	~ThirdPersonCamera();

	// �Ǐ]����
	void UpdateFollow(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Quaternion& rotate ,const DirectX::SimpleMath::Vector3& distance);

	// �r���[�s��̎擾
	const DirectX::SimpleMath::Matrix& GetFollowView() { return m_followView; }

};

#endif // THIRDPERSONCAMERA