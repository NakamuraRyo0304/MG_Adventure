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

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update();

	/// <summary>
	/// �I�u�W�F�N�g�̐U������
	/// </summary>
	/// <param name="duration">�h��p������</param>
	/// <param name="tremor">�h�ꕝ</param>
	/// <param name="pos">�ΏۃI�u�W�F�̃|�W�V����</param>
	/// <returns>�Ȃ�</returns>
	void ShakeObject(float duration, float tremor, DirectX::SimpleMath::Vector3* pos);


private:
	/// <summary>
	/// �}�E�X�̃h���b�O�����������v�Z
	/// </summary>
	/// <param name="x">�X�N���[�����WX</param>
	/// <param name="y">�X�N���[�����WY</param>
	/// <returns>�Ȃ�</returns>
	void DraggedDistance(int x, int y);

	/// <summary>
	/// �r���[�s��v�Z
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CalculateViewMatrix();

	/// <summary>
	/// �J�����̊g�嗦�̕ύX
	/// </summary>
	/// <param name="state">�}�E�X�̃X�e�[�g</param>
	/// <returns>�Ȃ�</returns>
	void RollWheelToRate(DirectX::Mouse::State state);

//--------------------------------------------------------//
//�A�N�Z�T                                                //
//--------------------------------------------------------//
public:
	/// <summary>
	/// �r���[�s����擾
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }

	/// <summary>
	/// �J�����̍��W���擾
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�J�����̖ڐ��̈ʒu</returns>
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

	/// <summary>
	/// �J�����̖ڐ��̈ʒu��ύX����
	/// </summary>
	/// <param name="pos">�ړ���</param>
	/// <returns>�Ȃ�</returns>
	void AddEyePosition(const DirectX::SimpleMath::Vector3& pos){ m_addPos = pos;}

	/// <summary>
	/// �J�����̒����_���擾
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�����_</returns>
	const DirectX::SimpleMath::Vector3& GetTarget() { return m_target; }

	/// <summary>
	/// �ˉe�s����쐬
	/// </summary>
	/// <param name="width">��ʉ���</param>
	/// <param name="height">��ʏc��</param>
	/// <param name="angle">�J������p</param>
	/// <returns>�ˉe�s��</returns>
	const DirectX::SimpleMath::Matrix& CreateProjection(float width, float height,float angle = 45.0f);

	/// <summary>
	/// �ˉe�s����擾
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�ˉe�s��</returns>
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	/// <summary>
	/// �}�E�X�h���b�O�Ŏ��_����]���郂�[�h��؂�ւ���
	/// </summary>
	/// <param name="flag">�h���b�O�ŉ�]���邩���t���O�ŃZ�b�g</param>
	/// <returns>�Ȃ�</returns>
	void SetEagleMode(const bool& flag = true) { is_eagleMode = flag; }

	/// <summary>
	/// �\���L�[�Ŏ��_����]���郂�[�h��؂�ւ���
	/// </summary>
	/// <param name="flag">�\���L�[�Ŏ��_����]���邩���t���O�ŃZ�b�g</param>
	/// <returns>�Ȃ�</returns>
	void SetArrowMode(const bool& flag = true) { is_allowMode = flag; }

};

#endif // CAMERA
