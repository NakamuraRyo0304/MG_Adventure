/*
 *	@File	RayCast.h
 *	@Brief	���C���΂��ăX�N���[�����W�����[���h���W�ɕϊ�����B
 *	@Date	2023-05-26
 *  @Author NakamuraRyo
 */

class RayCast
{
private:
	// �}�E�X�����[���h���W�ɕϊ������l��ۑ�����ϐ�
	DirectX::SimpleMath::Vector3 m_convertPosition;

	// �s��
	DirectX::SimpleMath::Matrix m_view, m_projection;

	// �X�N���[���T�C�Y
	DirectX::SimpleMath::Vector2 m_screenSize;

public:
	RayCast();
	~RayCast();

	// �X�V����
	void Update(DirectX::Mouse::State& mouseState);

private:

	// ���C���΂��ĖʂƂ̌�_�����߂�
	DirectX::SimpleMath::Vector3 ShotRay(int mx, int my);

	// �}�E�X�̃X�N���[�����W�����[���h���W�ɕϊ�����
	DirectX::SimpleMath::Vector3 ConvertScreenToWorld(int mx, int my, float fz, int width, int height,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

public:
	// �}�E�X�̃��[���h���W�̃Q�b�^�[
	const DirectX::SimpleMath::Vector3& GetWorldMousePosition() { return m_convertPosition; }

	// �X�N���[���T�C�Y�̃Z�b�^�[
	void SetScreenSize(float width, float height) { m_screenSize = { width,height }; }

	// �s��̃Z�b�^�[
	void SetMatrix(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) { m_view = view; m_projection = proj; }
};