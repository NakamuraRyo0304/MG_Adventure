/*
 *	@File	LeftLeg.cpp
 *	@Brief	�v���C���[�̍����B
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/UserUtility.h"
#include "LeftLeg.h"

 // �R���X�g���N�^
LeftLeg::LeftLeg(std::unique_ptr<Model> model)
	: IParts(std::move(model))	// ���N���X
	, m_move{ 0.0f }			// ���̓���
{
}

// �f�X�g���N�^
LeftLeg::~LeftLeg()
{
}

// �ʏ�̍X�V
void LeftLeg::Update()
{
	auto _key = Keyboard::Get().GetState();

	// �r�̓���
	(_key.W || _key.A || _key.S || _key.D) ? m_move++ : m_move = 0.0f;

	// ���W�̍X�V
	SimpleMath::Vector3 _pos = SimpleMath::Vector3(0.0f, 0.0f, sinf(m_move) * SPEED);

	// �s��̍쐬
	SimpleMath::Matrix _trans =	SimpleMath::Matrix::CreateTranslation(_pos);

	// �s��̐ݒ�
	SetMatrix(_trans);
}

// �`�揈��
void LeftLeg::Draw(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	GetModel()->Draw(_context, states, GetMatrix() * GetParentMatrix(), view, proj);
}

