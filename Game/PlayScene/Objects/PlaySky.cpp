/*
 *	@File	PlaySky.h
 *	@Brief	�v���C�V�[���̃X�J�C�h�[���B
 *	@Date	2023-11-09
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/FactoryManager/FactoryManager.h"
#include "PlaySky.h"

// �R���X�g���N�^
PlaySky::PlaySky(std::shared_ptr<FactoryManager> fm, const wchar_t* path)
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
	// �F��������
	m_skyColor = Color::Color();
}

// �f�X�g���N�^
PlaySky::~PlaySky()
{
}

// �X�V�֐�
void PlaySky::Update(const float& gameTimer)
{
	m_skyColor.red = gameTimer;
	m_skyColor.green = gameTimer;
	m_skyColor.blue = gameTimer;
}

// �`��֐�
void PlaySky::Draw(CommonStates& states,
	const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj, const float& timer)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(timer * SKY_ROT_SPEED);
	m_model->Draw(_context, states, skyMat, view, proj);
	m_model->UpdateEffects([&](IEffect* effect)
		{
			// �F�����X�ɈÂ�����
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				// ���X�ɈÂ��Ȃ��Ă���
				_basicEffect->SetEmissiveColor(
					SimpleMath::Color{
						m_skyColor.red,		 // ��
						m_skyColor.green,	 // ��
						m_skyColor.blue,	 // ��
						1.0f				 // �����x
					}
				);
			}
		}
	);
}