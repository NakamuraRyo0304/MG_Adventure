/*
 *	@File	TitleSky.cpp
 *	@Brief	�^�C�g���̋�̕`��B
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "TitleSky.h"

// �R���X�g���N�^
TitleSky::TitleSky(std::shared_ptr<FactoryManager> factory, const wchar_t* path)
	: ITitleObject()
	, m_position{}				// ���W
	, m_timer{}					// �^�C�}�[
	, is_startFlag{ false }		// �J�n�t���O
{
	auto _mf = factory;
	_mf->BuildModelFactory();

	auto _model = _mf->VisitModelFactory()->GetCreateModel(path);
	m_model = std::move(_model);

	_mf->LeaveModelFactory();


	m_model->UpdateEffects([](IEffect* effect)
		{
			// ���C�e�B���O
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				_lights->SetLightEnabled(0, false);
				_lights->SetLightEnabled(1, true);
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
TitleSky::~TitleSky()
{

}

// �X�V����
void TitleSky::Update()
{
	m_timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());
}

// �`�揈��
void TitleSky::Render(CommonStates& states, const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj)
{
	// �R���e�L�X�g���擾
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	// �s����v�Z
	SimpleMath::Matrix _world = SimpleMath::Matrix::Identity;

	// ��]�s��
	SimpleMath::Matrix _rotX = SimpleMath::Matrix::CreateRotationX(m_timer * 2.0f);

	// �ړ��s��
	SimpleMath::Matrix _trans = SimpleMath::Matrix::CreateTranslation(0.0f, m_position.y ,8.0f);

	// �X�J�C�h�[��
	_world *= _rotX;

	// �X�^�[�g�������ɏオ��A��]����������
	if (is_startFlag)
	{
		_world *= _rotX * _trans;
	}

	m_model->Draw(_context, states, _world, view, proj);
}

// �I������
void TitleSky::Finalize()
{
	m_model.reset();
}
