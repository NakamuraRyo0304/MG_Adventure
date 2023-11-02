/*
 *	@File	FileName.h
 *	@Brief	�������e�B
 *	@Date	2023-00-00
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Logo.h"

// �R���X�g���N�^
Logo::Logo(const wchar_t* path)
	: m_logoMoveScale{}		// �g�嗦
	, m_logoMoveY{}			// �ړ���
	, is_startFlag{ false } // �X�^�[�g�������ǂ���
	, is_endFlag{ false }   // �ړI�n���z������True
{
	// ���S�̑傫��
	m_logoMoveY = 10.0f;
	m_logoMoveScale = 1.0f;

	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	m_model = std::move(ModelFactory::GetCreateModel(_device, path));
	m_model->UpdateEffects([](IEffect* effect)
		{
			// ���C�e�B���O
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				// ���C�g�̎w��
				_lights->SetLightEnabled(0, true);
				_lights->SetLightEnabled(1, true);
				_lights->SetLightEnabled(2, false);

				// ���C�g�̕�����ݒ�
				SimpleMath::Vector3 _lightDir = -SimpleMath::Vector3::UnitZ;
				_lights->SetLightDirection(0, _lightDir);
				_lights->SetLightDirection(1, _lightDir);
			}
		}
	);
}

// �f�X�g���N�^
Logo::~Logo()
{
	m_model.reset();
}

// �X�V����
void Logo::Update()
{
	// �N�����̃��S�̓���
	m_logoMoveY = UserUtility::Lerp(m_logoMoveY, END_MOVE_POS, 0.1f);

	// �ړI�n���z������True�ɂ���
	if (static_cast<int>(m_logoMoveY) == static_cast<int>(END_MOVE_POS))
	{
		is_endFlag = true;
	}
}

// �`�揈��
void Logo::Render(CommonStates& states, const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	float _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �s����v�Z
	SimpleMath::Matrix _world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix _rotMat = SimpleMath::Matrix::CreateRotationX(sinf(_timer) * 0.5f);
	SimpleMath::Matrix _transMat = SimpleMath::Matrix::CreateTranslation(0.0f, m_logoMoveY, cosf(_timer) * 0.5f);

	// ���S���ꏏ�ɓ���
	if (is_startFlag)
	{
		m_logoMoveScale *= LOGO_CHANGE_SCALE;
		_world *= SimpleMath::Matrix::CreateScale(m_logoMoveScale);
	}

	// ���ʂ𔽉f
	_world *= _rotMat * _transMat;

	m_model->Draw(_context, states, _world, view, proj);
}
