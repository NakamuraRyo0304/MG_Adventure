/*
 *	@File	LocateBillBoard.cpp
 *	@Brief	�v���C���[�̈ʒu�������r���{�[�h�`��N���X�B
 *	@Date	2023-07-04
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "LocateBillBoard.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
LocateBillBoard::LocateBillBoard()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
LocateBillBoard::~LocateBillBoard()
{

}

/// <summary>
/// �r���{�[�h�쐬�֐�
/// </summary>
/// <param name="device">�f�o�C�X���\�[�X��ID3D11Device1�|�C���^</param>
void LocateBillBoard::Create(ID3D11Device1* device)
{
	device;
}

/// <summary>
/// �r���{�[�h�쐬�֐�
/// </summary>
/// <param name="eye">�J�����̍��W</param>
/// <param name="target">�����_</param>
/// <param name="up">������̎��w��(��{)</param>
void LocateBillBoard::CreateBillboard(SimpleMath::Vector3 eye, SimpleMath::Vector3 target,SimpleMath::Vector3 up)
{
	eye, target, up;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�V�[����elapsedTime</param>
void LocateBillBoard::Update(float elapsedTime)
{
	elapsedTime;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void LocateBillBoard::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	view, proj;
}
