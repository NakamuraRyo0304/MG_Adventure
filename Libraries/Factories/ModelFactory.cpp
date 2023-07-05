/*
 *	@File	ModelFactory.cpp
 *	@Brief	���f�����쐬����t�@�N�g���[�N���X�B
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ModelFactory.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
ModelFactory::ModelFactory()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
ModelFactory::~ModelFactory()
{
}

/// <summary>
/// ���f�����쐬���A�ԋp����(std::unique_ptr<Model>�Ŋi�[��)
/// </summary>
/// <param name="device">ID3D11Device1�|�C���^</param>
/// <param name="filename">�t�@�C���p�X�@�g���q�́u.cmo�v</param>
// ��P�����F�f�o�C�X ��Q�����F�t�@�C���̖��O
std::unique_ptr<DX11::Model> ModelFactory::GetModel(ID3D11Device1* device, const wchar_t* filename)
{
	// ���f���̍쐬(�쐬���݂̂̎g���̂�)
	std::unique_ptr<EffectFactory> fx =
		std::make_unique<EffectFactory>(device);

	// �f�B���N�g���̐ݒ� (�����Ƀ��f��������p�X��ݒ�)
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

/// <summary>
/// ���f���̉��
/// </summary>
/// <param name="��������"></param>
void ModelFactory::DeleteModel()
{
	// ��̈�����n��(��O�X���[�h�~�p�I�[�o�[���[�h)
	std::unique_ptr<Model> nullModel = nullptr;
	DeleteModel(nullModel);
}

/// <summary>
/// ���f���̉��
/// </summary>
/// <param name="model">������������f���f�[�^</param>
void ModelFactory::DeleteModel(std::unique_ptr<Model>& model)
{
	model.reset();
}
