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
	PlayCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~PlayCamera();

	// �r���[�s��쐬�֐��i�r���[�̏����̍Ō�ɌĂԁj
	const DirectX::SimpleMath::Matrix& CreateView();

	// �v���W�F�N�V�����s��Q�b�^�[
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	// ���W�Q�b�^�[
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
	// ���W�Z�b�^�[
	void SetPosition(const DirectX::SimpleMath::Vector3& pos = { 0.f,1.f,1.f }) { m_position = pos; }

	// �����_�Q�b�^�[
	const DirectX::SimpleMath::Vector3& GetTarget() { return m_target; }
	// �����_�Z�b�^�[
	void SetTarget(const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero) { m_target = target; }

private:
	// �v���W�F�N�V�����쐬�N���X
	void CreateProjection();

};

#endif // PLAYCAMERA