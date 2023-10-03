/*
 *	@File	RayCast.cpp
 *	@Brief	���C���΂��ăX�N���[�����W�����[���h���W�ɕϊ�����B
 *	@Date	2023-05-26
 *  @Author NakamuraRyo
 */

 // �g�p�֐��Q�̎Q�l����
 //-------------------------------------------------------------------------------------//
 //   �܂�؂�
 //   ���̂S�W�@�X�N���[�����W�Ń��[���h��Ԃ̒n�ʂ��w��
 //   http://marupeke296.com/DXG_No48_PointGroundInScreen.html
 // �@�F ���J�A�X�N���[�����W�_�ƒn�ʂ̌�_�Z�o�֐����
 //-------------------------------------------------------------------------------------//

#include "pch.h"

#include "RayCast.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
RayCast::RayCast()
	: m_screenSize{}			// �X�N���[���̃T�C�Y
	, m_conScreenPos{}			// ���[���h���W�ɕϊ������l��ۑ�����ϐ�
	, is_clickFlag{}			// �N���b�N����
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
RayCast::~RayCast()
{

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mouseState">�}�E�X��n��</param>
/// <returns>�Ȃ�</returns>
void RayCast::Update(Mouse::State& mouseState)
{
	// �N���b�N���Ă�Ƃ���True��Ԃ�
	is_clickFlag = mouseState.leftButton;

	// ���C���΂�����
	m_conScreenPos = ShotRay(mouseState.x, mouseState.y);
}

/// <summary>
/// �X�N���[�����W�����[���h���W�ɕϊ�����
/// </summary>
/// <param name="mx">�}�E�XX</param>
/// <param name="my">�}�E�XY</param>
/// <param name="fz">�C�ӂ̒l</param>
/// <param name="width">��ʉ���</param>
/// <param name="height">��ʏc��</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <returns>�ϊ���̍��W</returns>
SimpleMath::Vector3 RayCast::ConvertScreenToWorld(int mx, int my, float fz,
	int width, int height, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// �e�s��̋t�s����Z�o
	XMMATRIX revView, revProj, viewport, revViewport = XMMATRIX::XMMATRIX();

	// �t�s��ɕϊ�
	revView = XMMatrixInverse(nullptr, view);
	revProj = XMMatrixInverse(nullptr, proj);

	// �r���[�|�[�g��������
	viewport = XMMatrixIdentity();

	XMFLOAT4X4 matrix = XMFLOAT4X4::XMFLOAT4X4();

	// �X�P�[���ƃI�t�Z�b�g��ϊ�
	matrix._11 =   width / 2.0f;
	matrix._22 = -height / 2.0f;
	matrix._41 =   width / 2.0f;
	matrix._42 =  height / 2.0f;

	// �r���[�|�[�g�ɍs��ϊ�
	viewport += XMLoadFloat4x4(&matrix);

	// �r���[�|�[�g�̋t�s����쐬
	revViewport = XMMatrixInverse(nullptr,viewport);

	// �t�ϊ�
	SimpleMath::Matrix tmp = revViewport * revProj * revView;

	SimpleMath::Vector3 value = XMVector3TransformCoord(
		SimpleMath::Vector3(static_cast<float>(mx), static_cast<float>(my), fz), tmp);

	return value;
}

/// <summary>
/// ���C���΂��Ēn�ʂƂ̌�_�����
/// </summary>
/// <param name="mx">�}�E�XX</param>
/// <param name="my">�}�E�XY</param>
/// <returns>���������n�ʂƂ̌�_</returns>
SimpleMath::Vector3 RayCast::ShotRay(int mx, int my)
{
	// �ŋ߁A�ŉ��A���C���`
	SimpleMath::Vector3 nearpos;
	SimpleMath::Vector3 farpos;
	SimpleMath::Vector3 ray;

	// �ŋߋ������X�N���[�����烏�[���h�ɕϊ�
	nearpos = ConvertScreenToWorld(mx, my, 0.0f,
		static_cast<int>(m_screenSize.x),  static_cast<int>(m_screenSize.y),
		m_view, m_projection);

	// �ŉ��������X�N���[�����烏�[���h�ɕϊ�
	farpos  = ConvertScreenToWorld(mx, my, 1.0f,
		static_cast<int>(m_screenSize.x),  static_cast<int>(m_screenSize.y),
		m_view, m_projection);

	// ���C�̒��������߂Đ��K������
	ray = farpos - nearpos;
	ray.Normalize();

	// Y���W�ŏ����̏�����
	SimpleMath::Vector3 output = SimpleMath::Vector3::Zero;

	// ���Ƃ̌������N���Ă���ꍇ�͌�_�A�N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.y <= 0)
	{
		// ���Ƃ̌�_�����߂�
		SimpleMath::Vector3 rDot = XMVector3Dot(     ray, SimpleMath::Vector3(0, 1, 0));
		SimpleMath::Vector3 nDot = XMVector3Dot(-nearpos, SimpleMath::Vector3(0, 1, 0));
  		output = nearpos + (nDot / rDot) * ray;

		// Y���ړ��Ȃ������߂�0���
		// �{�Q�[���ł́A�}�E�X�z�C�[����Y���ړ�
		output.y = 0;
	}
	else
	{
		output = farpos;
	}

	return output;
}