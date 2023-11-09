/*
 *	@File	FontObject.cpp
 *	@Brief	�I�𒆂̃t�H���g��\������N���X�B
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/FactoryManager/FactoryManager.h"
#include "FontObject.h"

// �R���X�g���N�^
FontObject::FontObject(std::shared_ptr<FactoryManager> factory, const int& safeStage, const int& maxStage)
	: m_factory{ factory }
{
	m_factory->BuildModelFactory();

	for (int i = 0; i < maxStage - safeStage; ++i)
	{
		std::unique_ptr<Model> _m;
		// 0�Ԗڂ̓G�f�B�^�̕���
		if (i == 0)
		{
			_m = std::move(m_factory->VisitModelFactory()->GetCreateModel(L"Resources/Models/StageEdit.cmo"));
		}
		else
		{
			std::wstring _path = L"Resources/Models/Stage" + std::to_wstring(i) + L".cmo";
			_m = std::move(m_factory->VisitModelFactory()->GetCreateModel(_path.c_str()));
		}
		m_fonts[i] = std::move(_m);
	}

	m_factory->LeaveModelFactory();
}

// �f�X�g���N�^
FontObject::~FontObject()
{
	m_factory->BuildModelFactory();
	for (int i = 0; i < 6; ++i)
	{
		m_factory->VisitModelFactory()->DeleteModel(m_fonts[i]);
	}
	m_factory->LeaveModelFactory();
}

// �`�揈��
void FontObject::Render(CommonStates& states, const int& selectNum, const float& rotate,
	const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj)
{
	// �R���e�L�X�g�̎擾
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	// �e�L�X�g�̈ړ��A�j���[�V����
	SimpleMath::Matrix stageMat = CreateTextMatrix(rotate);

	// �X�e�[�W�ԍ��\��
	m_fonts[selectNum]->Draw(_context, states, stageMat, view, proj);
}

// �e�L�X�g�̉�]
SimpleMath::Matrix FontObject::CreateTextMatrix(const float& rotValue)
{
	SimpleMath::Matrix _mat = SimpleMath::Matrix::Identity;
	_mat *= SimpleMath::Matrix::CreateRotationY(rotValue);
	_mat *= SimpleMath::Matrix::CreateScale(10.0f);
	_mat *= SimpleMath::Matrix::CreateTranslation(sinf(rotValue), 10.0f, cosf(rotValue));

	return _mat;
}