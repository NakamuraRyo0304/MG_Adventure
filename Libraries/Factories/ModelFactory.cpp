/*
 *	@File	ModelFactory.cpp
 *	@Brief	���f�����쐬����t�@�N�g���[�N���X�B
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ModelFactory.h"

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
ModelFactory::~ModelFactory()
{
	DeleteModel();
}

//--------------------------------------------------------//
//���f�����쐬����t�@�N�g���[                            //
//--------------------------------------------------------//
// ��P�����F�f�o�C�X ��Q�����F�t�@�C���̖��O
std::unique_ptr<DX11::Model> ModelFactory::GetModel(ID3D11Device1* device, const wchar_t* filename)
{
	// ���f���̍쐬(�쐬���݂̂̎g���̂�)
	std::unique_ptr<EffectFactory> fx =
		std::make_unique<EffectFactory>(device);

	// �f�B���N�g���̐ݒ�
	fx->SetDirectory(L"Resources/Models");

	// ���f���f�[�^�̓ǂݍ���
	std::unique_ptr<DX11::Model> model = Model::CreateFromCMO(
		device,
		filename,
		*fx
	);

	// �쐬�������f����ԋp
	return model;
}

//--------------------------------------------------------//
//�󂾂����ꍇ�Anull��ݒ肵�ĉ������̂�����̏���������  //
//--------------------------------------------------------//
void ModelFactory::DeleteModel()
{
	// ��̈�����n��
	std::unique_ptr<Model> nullModel = nullptr;
	DeleteModel(nullModel);
}

//--------------------------------------------------------//
//���f����j������                                        //
//--------------------------------------------------------//
void ModelFactory::DeleteModel(std::unique_ptr<Model>& model)
{
	model.reset();
}
