/*
 *	@File	PlayCamera.h
 *	@Brief	�ŏ��̃J�������o�B
 *	@Date	2023-10-03
 *  @Author NakamuraRyo
 */

#ifndef PLAYCAMERA
#define PLAYCAMERA

class PlayCamera
{
private:

	// �J�����s��
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	// �J�����|�W�V����
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_target;

	// �X�N���[���T�C�Y
	DirectX::SimpleMath::Vector2 m_screenSize;

private:

	// �J�����̍ŋߋ���
	const float NEAR_PLANE = 0.1f;

	// �J�����̍ŉ�����
	const float FAR_PLANE = 245.0f;

	// �J�����A���O��
	const float ANGLE = 45.0f;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="screenSize">�X�N���[���T�C�Y</param>
	/// <returns>�Ȃ�</returns>
	PlayCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~PlayCamera();

	/// <summary>
	/// �r���[���쐬
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�r���[�s��</returns>
	const DirectX::SimpleMath::Matrix& CreateView();
	// �v���W�F�N�V�����s����擾
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }
	// ���W���擾
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
	// ���W��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos = { 0.f,1.f,1.f }) { m_position = pos; }
	// �����_���擾
	const DirectX::SimpleMath::Vector3& GetTarget() { return m_target; }
	// �����_��ݒ�
	void SetTarget(const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero) { m_target = target; }

private:

	/// <summary>
	/// �v���W�F�N�V�������쐬
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CreateProjection();

};

#endif // PLAYCAMERA