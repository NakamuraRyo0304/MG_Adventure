/*
 *	@File	ModelFactory.cpp
 *	@Brief	���f�����쐬����t�@�N�g���[�N���X�B
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ModelFactory.h"

// �R���X�g���N�^
ModelFactory::ModelFactory()
{
}

// �f�X�g���N�^
ModelFactory::~ModelFactory()
{
}

// ���f���쐬�֐�
std::unique_ptr<Model> ModelFactory::GetCreateModel(const wchar_t* filename)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// ���f���̍쐬(�쐬���݂̂̎g���̂�)
	std::unique_ptr<EffectFactory> _fx =
		std::make_unique<EffectFactory>(_device);

	// �f�B���N�g���̐ݒ� (�����Ƀ��f��������p�X��ݒ�)
	_fx->SetDirectory(L"Resources/Models");

	// ���f���f�[�^�̓ǂݍ���
	std::unique_ptr<Model> _model = Model::CreateFromCMO(
		_device,
		filename,
		*_fx
	);

	// �쐬�������f����ԋp
	return _model;
}

// ���f���j���֐�
void ModelFactory::DeleteModel(std::unique_ptr<Model>& model)
{
	model.reset();
}
