/*
 *	@File	TitleCamera.h
 *	@Brief	�^�C�g���V�[���̃J�����N���X�B
 *	@Date	2023-11-11
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TITLECAMERA
#define TITLECAMERA

#include "../../CommonObjects/IGameCamera.h"

class TitleCamera : public IGameCamera
{
private:

	// ���S�̍��W
	DirectX::SimpleMath::Vector3 m_logoPosition;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="screenSize">�X�N���[���T�C�Y</param>
	/// <returns>�Ȃ�</returns>
	TitleCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~TitleCamera();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update() override;

	// ���S�̍��W�̐ݒ�
	void SetLogoPosition(const DirectX::SimpleMath::Vector3& pos) { m_logoPosition = pos; }
};

#endif // TITLECAMERA