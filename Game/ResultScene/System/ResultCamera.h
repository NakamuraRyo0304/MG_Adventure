/*
 *	@File	ResultCamera.h
 *	@Brief	���U���g�V�[���̃J�����N���X�B
 *	@Date	2023-11-12
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef RESULTCAMERA
#define RESULTCAMERA

#include "../../CommonObjects/IGameCamera.h"

class ResultCamera : public IGameCamera
{
private:

	// �^�C�}�[
	float m_timer;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="screenSize">�X�N���[���T�C�Y</param>
	/// <returns>�Ȃ�</returns>
	ResultCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~ResultCamera();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update() override;
};

#endif // RESULTCAMERA