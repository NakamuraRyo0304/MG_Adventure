/*
 *	@File	FontObject.cpp
 *	@Brief	�I�𒆂̃t�H���g��\������N���X�B
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ���f���t�@�N�g���[
#include "Libraries/Factories/ModelFactory.h"

#include "FontObject.h"

// �R���X�g���N�^
FontObject::FontObject(const int& safeStage, const int& maxStage)
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	for (int i = 0; i < maxStage - safeStage; ++i)
	{
		// 0�Ԗڂ̓G�f�B�^�̕���
		if (i == 0)
		{
			m_fonts[0] = std::move(ModelFactory::GetCreateModel(_device, L"Resources/Models/StageEdit.cmo"));
		}
		else
		{
			std::wstring _path = L"Resources/Models/Stage" + std::to_wstring(i) + L".cmo";
			m_fonts[i] = std::move(ModelFactory::GetCreateModel(_device, _path.c_str()));
		}
	}
}

// �f�X�g���N�^
FontObject::~FontObject()
{
	for (int i = 0; i < 6; ++i)
	{
		m_fonts[i].reset();
	}
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
