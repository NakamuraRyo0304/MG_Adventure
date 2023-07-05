/*
 *	@File	LocateBillBoard.h
 *	@Brief	�v���C���[�̈ʒu�������r���{�[�h�`��N���X�B
 *	@Date	2023-07-04
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef LOCATEBILLBOARD
#define LOCATEBILLBOARD

class LocateBillBoard
{
private:

public:
	// �R���X�g���N�^�f�X�g���N�^
	LocateBillBoard();
	~LocateBillBoard();

	// �쐬�֐�
	void Create(ID3D11Device1* device);

	// �r���{�[�h�ݒ�
	void CreateBillboard(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 up);

	// �X�V�֐�
	void Update(float elapsedTime);

	// �`��֐�
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
};

#endif // LOCATEBILLBOARD