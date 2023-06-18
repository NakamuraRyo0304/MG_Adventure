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
public:
	// �J�����̋���
	static const float DEFAULT_CAMERA_DISTANCE;

	// �J�����̈ړ����x
	static const float DEFAULT_CAMERA_SPEED;

	// �J�����z�C�[���㉺��
	static const int MAX_SCROLL_VALUE;
	static const int MIN_SCROLL_VALUE;

private:
	// ��]�p
	DirectX::SimpleMath::Vector2 m_angle;

	// �O��̃}�E�X���W(X,Y)
	DirectX::SimpleMath::Vector2 m_prevMouse;	
	
	// �}�E�X�z�C�[���̃X�N���[���l
	int m_scrollWheelValue;
	// �ꎞ�I�ȃX�N���[���l�̒~�ϗp�ϐ�
	int m_tempScrollValue;
	// �O��̃X�N���[���l�̕ۑ��ϐ�
	int m_prevScrollWheelValue;


	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;

	// �J�����̍��W
	DirectX::SimpleMath::Vector3 m_eye;	
	DirectX::SimpleMath::Vector3 m_eyePos;	

	// �����_
	DirectX::SimpleMath::Vector3 m_target;

	// ���_�ړ����[�h
	bool is_eagleMode;
	bool is_prevEagleMode;

	// �X�N���[���T�C�Y
	int m_screenWidth, m_screenHeight;

public:
	Camera();
	~Camera();

	// �X�V����
	void Update();

	// �}�E�X�̈ړ������̌v�Z
	void DraggedDistance(int x, int y);

	// �r���[�s����v�Z����
	void CalculateViewMatrix();
		
	// �J������h�炷����
	void ShakeCamera(float duration, float tremor, DirectX::SimpleMath::Vector3* pos);

	// �J�����ƃI�u�W�F�N�g�̋��������߂�
	float CalculateDistanceToObject(const DirectX::SimpleMath::Vector3& objPos);


//--------------------------------------------------------//
//�A�N�Z�T                                                //
//--------------------------------------------------------//
	// �r���[�s����擾����
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }

	// �J�����̍��W���擾����
	const DirectX::SimpleMath::Vector3& GetEyePosition() { return m_eye; }
	void SetEyePosition(const DirectX::SimpleMath::Vector3& pos){ m_eye = pos;}
	
	// �J�����̒����_���W���擾����
	const DirectX::SimpleMath::Vector3& GetTargetPosition() { return m_target; }
	void SetTargetPosition(const DirectX::SimpleMath::Vector3& pos) { m_target = pos; }

	// �ˉe�s��̎擾
	const DirectX::SimpleMath::Matrix& CreateProjection(float width, float height,float angle = 45.0f);
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_proj; }

	// �J�������_�ړ����[�h�ؑ�
	void SetEagleMode(bool flag) { is_eagleMode = flag; }
	const bool GetEagleMode() { return is_eagleMode; }

};

#endif // CAMERA
