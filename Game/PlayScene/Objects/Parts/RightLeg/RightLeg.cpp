/*
 *	@File	RightLeg.cpp
 *	@Brief	�v���C���[�̉E���B
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/UserUtility.h"
#include "RightLeg.h"

 // �R���X�g���N�^
RightLeg::RightLeg(std::unique_ptr<Model> model)
	: IParts(std::move(model))	// ���N���X
{
}

// �f�X�g���N�^
RightLeg::~RightLeg()
{
}

// �ʏ�̍X�V
void RightLeg::Update()
{

}

// �`�揈��
void RightLeg::Draw(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	GetModel()->Draw(_context, states, GetMatrix() * GetParentMatrix(), view, proj);
}

