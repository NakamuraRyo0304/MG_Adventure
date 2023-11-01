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
	, m_convertPosition{}		// ���[���h���W�ɕϊ��������W
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
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void RayCast::Update()
{
	auto _mouse = Mouse::Get().GetState();

	// ���C���΂��Č�_�����߂�
	m_convertPosition = ShotRay(_mouse.x, _mouse.y);
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
	SimpleMath::Vector3 _nearPos;
	SimpleMath::Vector3 _farPos;
	SimpleMath::Vector3 _ray;

	// �ŋߋ������X�N���[�����烏�[���h�ɕϊ�
	_nearPos = ConvertScreenToWorld(mx, my, 0.0f,
		static_cast<int>(m_screenSize.x), static_cast<int>(m_screenSize.y),
		m_view, m_projection);

	// �ŉ��������X�N���[�����烏�[���h�ɕϊ�
	_farPos = ConvertScreenToWorld(mx, my, 1.0f,
		static_cast<int>(m_screenSize.x), static_cast<int>(m_screenSize.y),
		m_view, m_projection);

	// ���C�̕��������߂�
	_ray = _farPos - _nearPos;
	_ray.Normalize();

	// Y���W�ŏ����̏�����
	SimpleMath::Vector3 _output = SimpleMath::Vector3::Zero;

	// ���Ƃ̌������N���Ă���ꍇ�͌�_�A�N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (_ray.y <= 0)
	{
		// ���Ƃ̌�_�����߂�
		SimpleMath::Vector3 _rayDot = XMVector3Dot(_ray, SimpleMath::Vector3(0, 1, 0));
		SimpleMath::Vector3 _nearDot = XMVector3Dot(-_nearPos, SimpleMath::Vector3(0, 1, 0));
		_output = _nearPos + (_nearDot / _rayDot) * _ray;

		// Y�Ō�_XZ�������āAY�v�f������
		_output.y = 0;
	}
	else
	{
		// �������Ă��Ȃ���΍ŉ������i�����I�Ȗ����j���o��
		_output = _farPos;
	}

	return _output;
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
	XMMATRIX _revView, _revProj, _vPort, _revVPort = XMMATRIX::XMMATRIX();

	// �t�s��ɕϊ�
	_revView = XMMatrixInverse(nullptr, view);
	_revProj = XMMatrixInverse(nullptr, proj);

	// �r���[�|�[�g��������
	_vPort = XMMatrixIdentity();

	XMFLOAT4X4 _mat = XMFLOAT4X4::XMFLOAT4X4();

	// �X�P�[���ƃI�t�Z�b�g��ϊ�
	_mat._11 =   width / 2.0f;
	_mat._22 = -height / 2.0f;
	_mat._41 =   width / 2.0f;
	_mat._42 =  height / 2.0f;

	// �r���[�|�[�g�ɍs��ϊ�
	_vPort += XMLoadFloat4x4(&_mat);

	// �r���[�|�[�g�̋t�s����쐬
	_revVPort = XMMatrixInverse(nullptr,_vPort);

	// �t�ϊ�
	XMMATRIX _revTmp = _revVPort * _revProj * _revView;

	XMVECTOR _value = XMVector3TransformCoord(
		SimpleMath::Vector3(static_cast<float>(mx), static_cast<float>(my), fz), _revTmp);

	return _value;
}
