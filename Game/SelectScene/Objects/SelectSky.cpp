/*
 *	@File	SelectSky.cpp
 *	@Brief	�Z���N�g�V�[���̃X�J�C�h�[���B
 *	@Date	2023-11-06
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �t�@�N�g���}�l�[�W��
#include "Libraries/FactoryManager/FactoryManager.h"

#include "SelectSky.h"

// �R���X�g���N�^
SelectSky::SelectSky(std::shared_ptr<FactoryManager> fm, const wchar_t* path)
{
	fm->BuildModelFactory();
	m_model = fm->VisitModelFactory()->GetCreateModel(path);
	fm->LeaveModelFactory();

	m_model->UpdateEffects([](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				_lights->SetLightEnabled(0, false);
				_lights->SetLightEnabled(1, false);
				_lights->SetLightEnabled(2, false);
			}
			// ���Ȕ�������
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				_basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);
}

// �f�X�g���N�^
SelectSky::~SelectSky()
{
}

// �`��֐�
void SelectSky::Draw(CommonStates& states,
	const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj, const float& timer)
{
	// ��]������
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(timer * ROT_SPEED);
	m_model->Draw(_context, states, skyMat, view, proj);
}