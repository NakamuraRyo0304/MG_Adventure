/*
 *	@File	TitleScene.cpp
 *	@Brief	�^�C�g���V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Objects/TitleUI.h"

#include "TitleScene.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
TitleScene::TitleScene()
	: IScene()					// ���N���X�̏�����
	, m_titleLogoModel{}		// �^�C�g�����S�̃��f��
	, m_miniatureModel{}		// �����X�e�[�W�̃��f��
	, m_cameraMoveY{0.0f}		// �J�������o(�X�^�[�g��)
	, m_logoMoveY{0.0f}			// ���S�̓���(�ړ�)
	, m_logoMoveScale{}			// ���S�̓���(�T�C�Y)
	, m_accelerate{ 0.0f }		// �I��ύX���̃X�e�[�W�̉�]����
	, is_startFlag{ false }		// �J�n�t���O
	, is_menuFlag{ true }		// �I���t���O
	, is_accelerateFlag{ false }// ��]�������Ă��邩�̔���t���O
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
TitleScene::~TitleScene()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleScene::Initialize()
{
	// ��ʈˑ��̏�����
	CreateWindowDependentResources();

	// �J�������_�ړ�
	GetSystemManager()->GetCamera()->SetEagleMode(false);

	// �ϐ��̏�����
	SetSceneValues();

	// BGM��炷
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_TITLESELECT, true);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleScene::Update()
{
	// �C���v�b�g�̍X�V
	auto _key = Keyboard::Get().GetState();
	GetSystemManager()->GetStateTrack()->Update(_key);

	// �J�����̍X�V
	GetSystemManager()->GetCamera()->Update();

	// �T�E���h�̍X�V
	GetSystemManager()->GetSoundManager()->Update();

	// �G�X�P�[�v�ŏI��
	if(GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::ENDGAME);}

	// �N�����̃��S�̓���
	m_logoMoveY = UserUtility::Lerp(m_logoMoveY, END_MOVE_POS, 0.1f);

	// �I�����ڂ�ύX����
	if (!is_startFlag)
	{
		if (GetFadeValue() >= 0.7f) return;

		if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Left) ||
			GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Right) ||
			GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::A) ||
			GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::D))
		{
			is_menuFlag = !is_menuFlag;
			is_accelerateFlag = true;
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
		}

		// �ύX���̓���
		is_accelerateFlag = m_accelerate >= MAX_ACCELERATE_TIME ? false : is_accelerateFlag;
		m_accelerate += is_accelerateFlag ? 0.1f : 0.0f;
		m_accelerate = !is_accelerateFlag ? 0.0f : m_accelerate;
	}

	// �J�ڐ�����肷��(�Q�[���J�n OR �Q�[���I��)
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		// �t�F�[�h���͏������Ȃ�
		if (GetFadeValue() >= 0.7f) return;
		is_startFlag = true;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}
	// �Z���N�g
	if (FlyStartObjects())
	{
		ChangeScene(is_menuFlag ? SCENE::SELECT : SCENE::ENDGAME);
	}

	// UI�̍X�V
	m_titleUI->Update(is_menuFlag);

}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleScene::Draw()
{
	// �`��֘A
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& _states = *GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �J�����p�s��
	SimpleMath::Matrix _logoMat, _stageMat, _skyMat, _view, _projection;

	// �ړ��A��]�s��
	SimpleMath::Matrix
		_logoTrans, _logoRot,								// ���S
		_stageTrans, _stageRotX, _stageRotY, _stageScale,	// �X�e�[�W
		_skyTrans, _skyRotX;								// �X�J�C�h�[��

	// ���[���h�s��
	_logoMat = _stageMat = _skyMat = SimpleMath::Matrix::Identity;

	//-------------------------------------------------------------------------------------//

	// ��]�s��
	_logoRot = SimpleMath::Matrix::CreateRotationX(sinf(_timer) * 0.5f);
	_stageRotX = SimpleMath::Matrix::CreateRotationX(0.3f);
	_stageRotY = SimpleMath::Matrix::CreateRotationY(_timer * 0.5f + m_accelerate);
	_skyRotX = SimpleMath::Matrix::CreateRotationX(_timer * 2.0f);

	//-------------------------------------------------------------------------------------//

	// �ړ��s��
	_logoTrans = SimpleMath::Matrix::CreateTranslation(0.0f, m_logoMoveY, cosf(_timer) * 0.5f);
	_stageTrans = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -10.0f);
	_skyTrans = SimpleMath::Matrix::CreateTranslation(0.0f, m_cameraMoveY, 0.0f);

	//-------------------------------------------------------------------------------------//

	// �X�P�[���s��
	_stageScale = SimpleMath::Matrix::CreateScale(1.2f);

	//-------------------------------------------------------------------------------------//

	// ���S
	_logoMat *= _logoRot * _logoTrans;
	// �X�e�[�W
	_stageMat *= _stageScale * _stageRotY * _stageRotX * _stageTrans;
	// �X�J�C�h�[��
	_skyMat *= _skyRotX;

	//-------------------------------------------------------------------------------------//

	// �X�^�[�g���o�̏����͂��̒�����������
	if (is_startFlag)
	{
		// �X�J�C�h�[���̉�]�ƈړ�
		_skyMat *= _skyRotX;
		_skyMat *= _skyTrans;

		// ���S���ꏏ�ɓ���
		m_logoMoveScale *= LOGO_CHANGE_SCALE;
		_logoMat *= SimpleMath::Matrix::CreateScale(m_logoMoveScale);
	}

	// �r���[�s��
	SimpleMath::Vector3 eye(0.0f, 0.1f + m_cameraMoveY, 8.0f);
	_view = SimpleMath::Matrix::CreateLookAt(eye, SimpleMath::Vector3::Zero, SimpleMath::Vector3::Up);

	// �v���W�F�N�V�����s��
	_projection = GetSystemManager()->GetCamera()->GetProjection();

	// ���C�g�̐ݒ�
	SimpleMath::Vector3	 _lightDir(-1.0f, 1.0f, 1.0f);
	SimpleMath::Color	 _lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	// �A�}�`���A���f���ݒ�
	m_miniatureModel->UpdateEffects([&](IEffect* effect)
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
	m_miniatureModel->Draw(_context, _states, _stageMat, _view, _projection);	// �X�e�[�W
	m_titleLogoModel->Draw(_context, _states, _logoMat, _view, _projection);	// ���S
	m_skyDomeModel->Draw(_context, _states, _skyMat, _view, _projection);  		// �X�J�C�h�[��

	// UI�̕`��
	m_titleUI->Render(GetFadeValue(), static_cast<int>(m_logoMoveY) == static_cast<int>(END_MOVE_POS));
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleScene::Finalize()
{
	// ���f���̉��
	ModelFactory::DeleteModel(m_titleLogoModel);
	ModelFactory::DeleteModel(m_miniatureModel);
	ModelFactory::DeleteModel(m_skyDomeModel);

	// UI�̏I������
	m_titleUI->Finalize();
}

/// <summary>
/// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleScene::CreateWindowDependentResources()
{
	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	auto _device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// ���C�N���j�[�N
	GetSystemManager()->CreateUnique();

	// �J�����̐ݒ�
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// UI�̏�����
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch(_context);
	m_titleUI = std::make_unique<TitleUI>();
	m_titleUI->Create(GetSystemManager(), _device, GetScreenSize());

	// ���f���̍쐬---------------------------------------------------------------------------------

	// �^�C�g�����S
	m_titleLogoModel = ModelFactory::GetCreateModel(_device, L"Resources/Models/TitleLogoVer2.cmo");
	m_titleLogoModel->UpdateEffects([](IEffect* effect)
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

	// �X�e�[�W���f��
	m_miniatureModel = ModelFactory::GetCreateModel(_device, L"Resources/Models/TitleStage.cmo");

	// �X�J�C�h�[��
	m_skyDomeModel = ModelFactory::GetCreateModel(_device, L"Resources/Models/ShineSky.cmo");
	m_skyDomeModel->UpdateEffects([](IEffect* effect)
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

/// <summary>
/// �V�[�����̕ϐ��������֐�
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void TitleScene::SetSceneValues()
{
	// ���S�̑傫��
	m_logoMoveY = 10.0f;
	m_logoMoveScale = 1.0f;

	// �Q�[�����J�n/�Q�[�����I��
	is_menuFlag = true;

	// �����x��������
	m_accelerate = 0.0f;
}

/// <summary>
/// �X�^�[�g���������Ƃ��̉��o
/// </summary>
/// <param name="��������"></param>
/// <returns>���o������True��Ԃ�</returns>
bool TitleScene::FlyStartObjects()
{
	if (is_startFlag)
	{
		m_cameraMoveY++;

		// ���o���I�������J��
		if (m_cameraMoveY > MAX_HEIGHT)
		{
			return true;
		}
	}

	return false;
}