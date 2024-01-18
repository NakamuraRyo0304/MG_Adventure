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
{
}

// �f�X�g���N�^
LeftLeg::~LeftLeg()
{
}

// �ʏ�̍X�V
void LeftLeg::Update()
{

}

// �`�揈��
void LeftLeg::Draw(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	GetModel()->Draw(_context, states, GetMatrix() * GetParentMatrix(), view, proj);
}

