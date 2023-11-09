/*
 *	@File	Stage.cpp
 *	@Brief	�^�C�g���̃X�e�[�W�B
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Stage.h"

// �R���X�g���N�^
Stage::Stage(std::shared_ptr<FactoryManager> factory,const wchar_t* path)
	: ITitleObject()
	, m_accelerate{ 0.0f }		// �I��ύX���̃X�e�[�W�̉�]����
	, is_accelerateFlag{ false }// ��]�������Ă��邩�̔���t���O
{
	auto _mf = factory;
	_mf->BuildModelFactory();

	auto _model = _mf->VisitModelFactory()->GetCreateModel(path);

	_mf->LeaveModelFactory();

	m_model = std::move(_model);
}

// �f�X�g���N�^
Stage::~Stage()
{

}

// �X�V����
void Stage::Update()
{
	// �ύX���̓���
	is_accelerateFlag = m_accelerate >= MAX_ACCELERATE_TIME ? false : is_accelerateFlag;
	m_accelerate += is_accelerateFlag ? 0.1f : 0.0f;
	m_accelerate = !is_accelerateFlag ? 0.0f : m_accelerate;
}

// �`�揈��
void Stage::Render(CommonStates& states, const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj)
{
	// �R���e�L�X�g���擾
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	// �^�C�}�[���擾
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �s����v�Z
	SimpleMath::Matrix _world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix _trans, _rotX, _rotY, _scale;
	_rotX = SimpleMath::Matrix::CreateRotationX(0.3f);
	_rotY = SimpleMath::Matrix::CreateRotationY(_timer * 0.5f + m_accelerate);
	_trans = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -10.0f);
	_scale = SimpleMath::Matrix::CreateScale(1.2f);

	// ���ʂ𔽉f
	_world *= _scale * _rotY * _rotX * _trans;

	// ���C�g�̐ݒ�
	SimpleMath::Vector3	 _lightDir(-1.0f, 1.0f, 1.0f);
	SimpleMath::Color	 _lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	// �A�}�`���A���f���ݒ�
	m_model->UpdateEffects([&](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				for (int i = 0; i < 3; ++i)
				{
					// ���C�g�̎g�p��ݒ�
					_lights->SetLightEnabled(i, true);

					// ���C�g�̕�����ݒ�
					_lights->SetLightDirection(i, _lightDir);

					// ���C�g�̐F��ݒ�
					_lights->SetLightDiffuseColor(i, _lightColor);
				}
			}
		});
	m_model->Draw(_context, states, _world, view, proj);
}

// �I������
void Stage::Finalize()
{
	m_model.reset();
}
