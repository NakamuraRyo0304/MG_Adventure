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
 /// <returns>�Ȃ�</returns>
ModelFactory::ModelFactory()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
ModelFactory::~ModelFactory()
{
}

/// <summary>
/// ���f�����쐬���A�ԋp����(std::unique_ptr<Model>�Ŋi�[��)
/// </summary>
/// <param name="device">ID3D11Device1�|�C���^</param>
/// <param name="filename">�t�@�C���p�X�@�g���q�́u.cmo�v</param>
/// <returns>�쐬�������f���f�[�^</returns>
std::unique_ptr<DX11::Model> ModelFactory::GetCreateModel(ID3D11Device1* device, const wchar_t* filename)
{
	// ���f���̍쐬(�쐬���݂̂̎g���̂�)
	std::unique_ptr<EffectFactory> _fx =
		std::make_unique<EffectFactory>(device);

	// �f�B���N�g���̐ݒ� (�����Ƀ��f��������p�X��ݒ�)
	_fx->SetDirectory(L"Resources/Models");

	// ���f���f�[�^�̓ǂݍ���
	std::unique_ptr<DX11::Model> _model = Model::CreateFromCMO(
		device,
		filename,
		*_fx
	);

	// �쐬�������f����ԋp
	return _model;
}

/// <summary>
/// ���f���̉��
/// </summary>
/// <param name="model">������������f���f�[�^</param>
/// <returns>�Ȃ�</returns>
void ModelFactory::DeleteModel(std::unique_ptr<Model>& model)
{
	model.reset();
}
