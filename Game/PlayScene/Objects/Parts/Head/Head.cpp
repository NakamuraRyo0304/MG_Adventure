/*
 *	@File	Head.cpp
 *	@Brief	�v���C���[�̓��B
 *	@Date	2023-01-18
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/UserUtility.h"
#include "Head.h"

// �R���X�g���N�^
Head::Head(std::unique_ptr<Model> model)
	: IParts(std::move(model))	// ���N���X
	, m_moveFB{ 0.0f }			// �O��^��
	, m_normalRotate{}			// ��̉�]�s��
	, m_rotate{}				// ��̉�]��
{
}

// �f�X�g���N�^
Head::~Head()
{
}

// �ʏ�̍X�V
void Head::Update()
{
	auto _key = Keyboard::Get().GetState();

	// �O��^��
	(_key.W || _key.S) ? m_moveFB++ : m_moveFB = 0.0f;

	SimpleMath::Matrix _mat =
		SimpleMath::Matrix::CreateTranslation(
			SimpleMath::Vector3(0.0f, 0.0f, sinf(m_moveFB * FB_SPEED) * 0.05f));

	// �s����Z�b�g
	SetMatrix(_mat);
}

// �`�揈��
void Head::Draw(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	GetModel()->Draw(_context, states, GetMatrix() * GetParentMatrix(), view, proj);
}
