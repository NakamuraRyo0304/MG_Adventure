/*
 *	@File	ModelFactory.cpp
 *	@Brief	���f�����쐬����t�@�N�g���[�N���X�B
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ModelFactory.h"

 //--------------------------------------------------------//
 //���f�����쐬����t�@�N�g���[                            //
 //--------------------------------------------------------//
 // ��P�����F�f�o�C�X ��Q�����F�t�@�C���̖��O
std::unique_ptr<DirectX::DX11::Model> ModelFactory::GetModel(ID3D11Device1* device, const wchar_t* filename)
{
	// ���f���̍쐬(�쐬���݂̂̎g���̂�)
	std::unique_ptr<DirectX::EffectFactory> fx =
		std::make_unique<DirectX::EffectFactory>(device);

	// �f�B���N�g���̐ݒ�
	fx->SetDirectory(L"Resources/Models");

	// ���f���f�[�^�̓ǂݍ���
	std::unique_ptr<DirectX::DX11::Model> model = DirectX::Model::CreateFromCMO(
		device,
		filename,
		*fx
	);

	// fx �C���X�^���X�͎g�p��ɉ������
	fx.reset();

	// �쐬�������f����ԋp
	return model;
}