/*
 *	@File	EditSky.cpp
 *	@Brief	�G�f�B�^�V�[���̃X�J�C�h�[���B
 *	@Date	2023-11-09
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/FactoryManager/FactoryManager.h"
#include "EditSky.h"

// �R���X�g���N�^
EditSky::EditSky(std::shared_ptr<FactoryManager> fm, const wchar_t* path)
{
	fm->BuildModelFactory();
	m_model = fm->VisitModelFactory()->GetCreateModel(path);
	fm->LeaveModelFactory();

	m_model->UpdateEffects([](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				// ���C�g�̐�����
				for (int i = 0; i < 3; ++i)
				{
					_lights->SetLightEnabled(i, false);
				}
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
EditSky::~EditSky()
{
}

// �`��֐�
void EditSky::Draw(CommonStates& states,
	const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj, const float& timer)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	SimpleMath::Matrix world = SimpleMath::Matrix::CreateRotationY(timer * ROT_SPEED);
	m_model->Draw(_context, states, world, view, proj);
}
