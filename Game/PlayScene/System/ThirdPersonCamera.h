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

private:
	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// �摜�p�ɃV�X�e����Ⴄ
	std::shared_ptr<SystemManager> m_system;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="system">�V�X�e���}�l�[�W��</param>
	/// <returns>�Ȃ�</returns>
	ThirdPersonCamera(std::shared_ptr<SystemManager> system);
	~ThirdPersonCamera();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="pos">��_</param>
	/// <param name="rotate">��]��</param>
	/// <param name="distance">��_����̋���</param>
	/// <returns>�Ȃ�</returns>
	void UpdateFollow(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Quaternion& rotate ,
		const DirectX::SimpleMath::Vector3& distance);

	/// <summary>
	/// �_�̕t����`��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void DrawAdhesion();

	// �r���[�s��̎擾
	const DirectX::SimpleMath::Matrix& GetFollowView() { return m_followView; }
};

#endif // THIRDPERSONCAMERA