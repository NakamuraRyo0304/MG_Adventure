/*
 *	@File	RayCast.cpp
 *	@Brief	���C���΂��ăX�N���[�����W�����[���h���W�ɕϊ�����B
 *	@Date	2023-05-26
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "RayCast.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
RayCast::RayCast():
	m_screenSize{},				// �X�N���[���̃T�C�Y
	is_clickFlag{},				// �N���b�N����
	m_conScreenPos{}			// ���[���h���W�ɕϊ������l��ۑ�����ϐ�
{

}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
RayCast::~RayCast()
{

}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
void RayCast::Update(DirectX::Mouse::State& mouseState)
{
	// �N���b�N���Ă�Ƃ���True��Ԃ�
	is_clickFlag = mouseState.leftButton;

	// ���C���΂�����
	m_conScreenPos = ShotRayToWorld(mouseState.x, mouseState.y);
}

//--------------------------------------------------------//
//�}�E�X�̃X�N���[�����W�����[���h���W�ɕϊ�����          //
//--------------------------------------------------------//
DirectX::SimpleMath::Vector3 RayCast::ConvertScreenToWorld(int mx, int my, float fz,
	int width, int height, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	// �e�s��̋t�s����Z�o
	DirectX::XMMATRIX InvView, InvProj, VP, InvViewport = DirectX::XMMATRIX::XMMATRIX();

	// �t�s��ɕϊ�
	InvView = DirectX::XMMatrixInverse(nullptr, view);
	InvProj = DirectX::XMMatrixInverse(nullptr, proj);

	VP = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT4X4 matrix = DirectX::XMFLOAT4X4::XMFLOAT4X4();

	// �X�P�[���ƃI�t�Z�b�g��ϊ�
	matrix._11 =   width / 2.0f;
	matrix._22 = -height / 2.0f;
	matrix._41 =   width / 2.0f;
	matrix._42 =  height / 2.0f;

	VP += DirectX::XMLoadFloat4x4(&matrix);

	InvViewport = DirectX::XMMatrixInverse(nullptr,VP);

	// �t�ϊ�
	DirectX::SimpleMath::Matrix tmp = InvViewport * InvProj * InvView;

	DirectX::SimpleMath::Vector3 value = DirectX::XMVector3TransformCoord(
		DirectX::SimpleMath::Vector3(static_cast<float>(mx), static_cast<float>(my), fz), tmp);

	return value;
}

//--------------------------------------------------------//
//���C���΂��ĖʂƂ̌�_�����߂�                        //
//--------------------------------------------------------//
DirectX::SimpleMath::Vector3 RayCast::ShotRayToWorld(int mx, int my)
{
	// �ŋ߁A�ŉ��A���C���`
	DirectX::SimpleMath::Vector3 nearpos;
	DirectX::SimpleMath::Vector3 farpos;
	DirectX::SimpleMath::Vector3 ray;

	// �ŋߋ������R���o�[�g
	nearpos = ConvertScreenToWorld(mx, my, 0.0f,
		static_cast<int>(m_screenSize.x),  static_cast<int>(m_screenSize.y), 
		m_view, m_proj);

	// �ŉ��������R���o�[�g
	farpos  = ConvertScreenToWorld(mx, my, 1.0f,
		static_cast<int>(m_screenSize.x),  static_cast<int>(m_screenSize.y), 
		m_view, m_proj);
	
	// ���C�̒��������߂Đ��K������
	ray = farpos - nearpos;
	ray.Normalize();

	// Y���W�ŏ����̏�����
	DirectX::SimpleMath::Vector3 output = DirectX::SimpleMath::Vector3::Zero;

	// ���Ƃ̌������N���Ă���ꍇ�͌�_�A�N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.y <= 0) 
	{
		// ����_
		DirectX::SimpleMath::Vector3 rDot = DirectX::XMVector3Dot(     ray, DirectX::SimpleMath::Vector3(0, 1, 0));
		DirectX::SimpleMath::Vector3 nDot = DirectX::XMVector3Dot(-nearpos, DirectX::SimpleMath::Vector3(0, 1, 0));
  		output = nearpos + (nDot / rDot) * ray;


		// Y���ړ��Ȃ������߂�0���
		output.y = 0;
	}
	else 
	{
		output = farpos;
	}

	return output;
}