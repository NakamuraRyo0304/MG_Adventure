/*
 *	@File	SelectCamera.h
 *	@Brief	�Z���N�g�V�[���̃J�����N���X�B
 *	@Date	2023-11-11
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTCAMERA
#define SELECTCAMERA

#include "../../CommonObjects/IGameCamera.h"

class SelectCamera : public IGameCamera
{
private:

	// �؂�ւ��\�����肷��
	bool is_canChangeFlag;

private:

	// �J������Y���W
	const float POS_Y = 30.0f;

	// �J�����̉�]���a
	const float CAMERA_RADIUS = 4.0f;

	// �ڋߑ��x
	const float APPROACH_SPEED = 0.085f;

	// �㉺����
	const float VERTICAL_MOTION = 1.5f;

	// ���_�̓��쑬�x
	const float LOOK_VALUE = 60.0f;
	const float LOOK_SPAN = 0.25f;
	const float LOOK_SPEED = 0.75f;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="screenSize">�X�N���[���T�C�Y</param>
	/// <returns>�Ȃ�</returns>
	SelectCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~SelectCamera();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update() override;
	/// <summary>
	/// �J�����̃^�[�Q�b�g�̍X�V
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void MoveTarget();
	// �؂�ւ��\�������Ԃ�
	const bool& IsCanChange(){ return is_canChangeFlag;}

};

#endif // SELECTCAMERA