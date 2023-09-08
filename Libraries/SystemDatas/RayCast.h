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
	DirectX::SimpleMath::Vector3 m_conScreenPos;

	// �s��
	DirectX::SimpleMath::Matrix m_view, m_proj;

	// �X�N���[���T�C�Y
	DirectX::SimpleMath::Vector2 m_screenSize;

	// �N���b�N����
	bool is_clickFlag;

public:
	RayCast();
	~RayCast();

	// �X�V����
	void Update(DirectX::Mouse::State& mouseState);

private:
	// �}�E�X�̃X�N���[�����W�����[���h���W�ɕϊ�����
	DirectX::SimpleMath::Vector3 ConvertScreenToWorld(
												 int mx,										// �}�E�XX
												 int my,										// �}�E�XY
												 float fz,										// �P�ʒl
												 int width, 									// ��ʉ���
												 int height,									// ��ʏc��
												 DirectX::SimpleMath::Matrix view,				// �r���[�s��
												 DirectX::SimpleMath::Matrix proj);		    	// �ˉe�s��

	// ���C���΂��ĖʂƂ̌�_�����߂�
	DirectX::SimpleMath::Vector3 ShotRayToWorld(int mx, int my);

public:
	// �}�E�X�̃��[���h���W�̃Q�b�^�[
	const DirectX::SimpleMath::Vector3& GetWorldMousePosition() { return m_conScreenPos; }

	// �N���b�N����̃Q�b�^�[
	const bool GetClickFlag() { return is_clickFlag; }

	// �X�N���[���T�C�Y�̃Z�b�^�[
	void SetScreenSize(float width, float height) { m_screenSize = { width,height }; }

	// �s��̃Z�b�^�[
	void SetMatrix(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) { m_view = view; m_proj = proj; }
};