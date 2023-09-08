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

	// �J�����z�C�[���㉺��
	static const int MAX_SCROLL_VALUE;
	static const int MIN_SCROLL_VALUE;

	// �J�����\������
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 245.0f;

	// �J������p�㉺��
	const float ANGLEX_MIN = 44.0f;
	const float ANGLEX_MAX = 45.5f;

private: //-------------------------------------------------------------------------------------//

	// �O��̃}�E�X���W(X,Y)
	DirectX::SimpleMath::Vector2 m_prevMouse;
	// �}�E�X�z�C�[���̃X�N���[���l
	int m_scrollWheelValue;
	// �ꎞ�I�ȃX�N���[���l�̒~�ϗp�ϐ�
	int m_tempScrollValue;
	// �O��̃X�N���[���l�̕ۑ��ϐ�
	int m_prevScrollWheelValue;
	// ��]�p
	DirectX::SimpleMath::Vector2 m_angle;

	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;
	// ��]��
	DirectX::SimpleMath::Matrix m_rotateMatrix;

	// �J�����̍��W
	DirectX::SimpleMath::Vector3 m_eye;
	DirectX::SimpleMath::Vector3 m_eyePos;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;

	// ���_�ړ����[�h
	bool is_eagleMode;
	bool is_prevEagleMode;
	bool is_allowMode;

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
	const DirectX::SimpleMath::Vector3& GetEye() { return m_eye; }

	// �J�����̍��W�ړ��p�Z�b�^�[(�w����W�Ɉړ�)
	void SetEyePosition(const DirectX::SimpleMath::Vector3& pos){ m_eyePos = pos;}

	// �^�[�Q�b�g���擾����
	const DirectX::SimpleMath::Vector3& GetTargetPosition() { return m_target; }

	// �ˉe�s��̎擾
	const DirectX::SimpleMath::Matrix& CreateProjection(float width, float height,float angle = 45.0f);
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_proj; }

	// �J�������_�ړ����[�h�ؑ�
	void SetEagleMode(const bool& flag) { is_eagleMode = flag; }
	void SetArrowMode(const bool& flag) { is_allowMode = flag; }

};

#endif // CAMERA
