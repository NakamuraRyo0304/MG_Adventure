/*
 *	@File	Camera.h
 *	@Brief	�J�����̐ݒ�B
 *	@Date	2023-04-28
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef CAMERA
#define CAMERA

class Camera
{
public:	//-------------------------------------------------------------------------------------//
	// �X�N���[���T�C�Y
	int m_screenWidth, m_screenHeight;

	// �J�����̋���
	static const float DEFAULT_CAMERA_DISTANCE;

	// �J�����̈ړ����x
	static const float DEFAULT_CAMERA_SPEED;
	const float ALLOW_SPEED = 0.01f;

	// �J�����z�C�[���㉺��
	static const int MAX_SCROLL_VALUE;
	static const int MIN_SCROLL_VALUE;

	// �J�����\������
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 245.0f;

	// �J������p�㉺��
	const float ANGLE_X_MIN = 44.0f;
	const float ANGLE_X_MAX = 45.5f;

private: //-------------------------------------------------------------------------------------//

	struct PrevMouse { DirectX::SimpleMath::Vector2 pos; int wheel; bool eagle; };

	// �O��̃}�E�X���
	PrevMouse m_prevMouse;
	// �}�E�X�z�C�[���̃X�N���[���l
	int m_scrollWheelValue, m_tempScrollValue;

	// ��]�p
	DirectX::SimpleMath::Vector2 m_angle;

	// �r���[�s��E�v���W�F�N�V�����s��E��]�s��
	DirectX::SimpleMath::Matrix m_view, m_projection, m_rotateMatrix;

	// �J�����̍��W�E���Z�p���W�E�����_
	DirectX::SimpleMath::Vector3 m_position, m_addPos, m_target;

	// ���_�ړ����[�h
	bool is_eagleMode, is_allowMode;

public:
	Camera();
	~Camera();

	// �X�V����
	void Update();

	// �I�u�W�F�N�g��h�炷����
	void ShakeObject(float duration, float tremor, DirectX::SimpleMath::Vector3* pos);


private:
	// �}�E�X�̈ړ������̌v�Z
	void DraggedDistance(int x, int y);

	// �r���[�s����v�Z����
	void CalculateViewMatrix();

	// �}�E�X�z�C�[���̊g�k����
	void RollWheelToRate(DirectX::Mouse::State state);

//--------------------------------------------------------//
//�A�N�Z�T                                                //
//--------------------------------------------------------//
public:
	// �r���[�s����擾����
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }

	// �J�����̍��W���擾����
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

	// �J�����̍��W�ړ��p�Z�b�^�[(�w����W�Ɉړ�)
	void AddEyePosition(const DirectX::SimpleMath::Vector3& pos){ m_addPos = pos;}

	// �^�[�Q�b�g���擾����
	const DirectX::SimpleMath::Vector3& GetTarget() { return m_target; }

	// �ˉe�s��̎擾
	const DirectX::SimpleMath::Matrix& CreateProjection(float width, float height,float angle = 45.0f);
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	// �}�E�X�h���b�O�Ŏ��_����]
	void SetEagleMode(const bool& flag = true) { is_eagleMode = flag; }
	// �\���L�[�Ŏ��_����]
	void SetArrowMode(const bool& flag = true) { is_allowMode = flag; }

};

#endif // CAMERA
