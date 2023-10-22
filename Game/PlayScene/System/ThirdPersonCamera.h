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

class SystemManager;
class ThirdPersonCamera final : public Camera
{
private:

	// �Ǐ]�p�r���[�s��
	DirectX::SimpleMath::Matrix m_followView;

	// �摜�X�V�p�ϐ�
	float m_adhesionTimer;
	const float CHANGE_SPAN = 120.0f;
	bool is_changeFlag;

private:
	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// �摜�p�ɃV�X�e����Ⴄ
	std::shared_ptr<SystemManager> m_system;

public:
	ThirdPersonCamera(std::shared_ptr<SystemManager> system,ID3D11DeviceContext1* context, ID3D11Device1* device);
	~ThirdPersonCamera();

	// �Ǐ]����
	void UpdateFollow(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Quaternion& rotate ,
		const DirectX::SimpleMath::Vector3& distance, const float& timer);

	// �r���[�s��̎擾
	const DirectX::SimpleMath::Matrix& GetFollowView() { return m_followView; }

	// �摜�̕`��
	void DrawAdhesion();
};

#endif // THIRDPERSONCAMERA