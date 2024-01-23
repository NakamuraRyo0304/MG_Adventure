/*
 *	@File	Blocks.cpp
 *	@Brief	�}�b�v�̃u���b�N�Ǘ��B
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/MapLoad.h"
#include "Libraries/FactoryManager/FactoryManager.h"
#include "Libraries/UserUtility.h"
#include "Blocks.h"

// �R���X�g���N�^
Blocks::Blocks(std::shared_ptr<FactoryManager> factory)
	: m_factory{factory}						// �t�@�N�g���}�l�[�W��
	, m_coinCount{0}							// �R�C���J�E���^
	, m_maxCoins{0}								// �R�C���ő�l
	, m_playerPos{ SimpleMath::Vector3::Zero }	// �v���C���[�|�W�V����
	, m_grassModel{ nullptr }					// ���u���b�N�̃��f��
	, m_coinModel{ nullptr }					// �R�C���u���b�N�̃��f��
	, m_cloudModel{ nullptr }					// �_�u���b�N�̃��f��
	, m_gravityModel{ nullptr }					// �d�̓u���b�N�̃��f��
	, m_lighting{}								// ���C�e�B���O
	, is_collectedFlag{ false }					// �R�C������ςݔ���t���O
	, is_hitCoinFlag{ false }					// ����t���O
{
	m_mapLoad = std::make_unique<MapLoad>();
}

// �f�X�g���N�^
Blocks::~Blocks()
{

}

// ����������
void Blocks::Initialize(int stageNum)
{
	// �t�@�C�����̐錾
	std::wstring _filePath = L"";

	// �X�e�[�W�Z���N�g
	_filePath = MapSelect(stageNum);

	// �}�b�v�̓ǂݍ���
	m_mapLoad->LoadMap(_filePath);

	// �}�b�v�̊i�[
	m_mapObj = m_mapLoad->GetMapData();

	// �������̉��
	m_mapLoad->ReleaseMemory();

	// ���W��␳����
	MapSwipe();

	// �R�C������ς݃t���O��False�ɂ���
	is_collectedFlag = false;
	// �����蔻��t���O��False�ɂ���
	is_hitCoinFlag = false;

	// ���C�e�B���O�̃��Z�b�g
	m_lighting = SimpleMath::Vector3::Zero;
}

// �X�V����
void Blocks::Update()
{
	// �_�͏㉺�ړ�����
	for (int i = 0; i < m_mapObj.size(); ++i)
	{
		if (m_mapObj[i].GetID() == MAPSTATE::CLOUD)
		{
			// �G��Ă���Ƃ��͏I�_�܂�
			if (not m_cloudState[i].moveFlag) continue;

			// Y���W���I�_�܂œ�����
			float _move = UserUtility::Lerp
			(
				m_mapObj[i].GetPosition().y,
				m_cloudState[i].endPosition.y,
				CLOUD_SPEED
			);

			m_mapObj[i].SetPosition(
				SimpleMath::Vector3(
					m_mapObj[i].GetPosition().x,
					_move,
					m_mapObj[i].GetPosition().z
				)
			);
		}
	}

	// �R�C�������������True�ɂ���
	if (m_coinCount == m_maxCoins)
	{
		is_collectedFlag = true;
	}
}

// �`��֐�
void Blocks::Render(CommonStates& states,SimpleMath::Matrix view, SimpleMath::Matrix proj,
	float timer, const SimpleMath::Vector3& lightDir)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	// ���[���h���W
	SimpleMath::Matrix _world, _rotate, _verticle;

	// ���C�e�B���O�ݒ�
	std::function<void(IEffect* effect)> _lightSetting;

	// �u���b�N�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].GetID() == MAPSTATE::NONE) continue;

		// �ړ��s��
		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].GetPosition());

		// ��]�s��
		_rotate = SimpleMath::Matrix::CreateRotationY(timer) * CLOUD_ROT_SPEED;

		// �r���[�s�񂩂�J�����̉�]���擾
		SimpleMath::Matrix _cameraRot;

		// �r���[�s����t�ϊ�
		_cameraRot = view.Invert();

		// �ړ��ʂ��Ȃ���
		_cameraRot._41 = 0.0f;
		_cameraRot._42 = 0.0f;
		_cameraRot._43 = 0.0f;

		// ���C�g�̕������J�����̉�]�ɋt�����ɂ���
		m_lighting = SimpleMath::Vector3::TransformNormal(lightDir, _cameraRot);
		SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);

		// �㉺�ړ�����
		_verticle *= SimpleMath::Matrix::CreateTranslation(
			SimpleMath::Vector3(0.0f, ((sinf(timer) + 1) * 0.5f) * 2.f, 0.0f));

		_lightSetting = [&](IEffect* effect)
		{
			// �x�[�V�b�N�G�t�F�N�g
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				// �A���r�G���g���C�g�J���[��ݒ�
				_basicEffect->SetAmbientLightColor(SimpleMath::Color(0.2f, 0.2f, 0.2f));
			}

			// ���C�g
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				// ���C�g�I��
				_lights->SetLightEnabled(0, true);
				_lights->SetLightEnabled(1, true);
				_lights->SetLightEnabled(2, true);

				// ���C�g�̕�����ݒ�
				_lights->SetLightDirection(0, m_lighting);
				_lights->SetLightDirection(1, m_lighting);
				_lights->SetLightDirection(2, m_lighting);

				// ���C�g�̐F���Â߂̃O���[�ɐݒ�
				_lights->SetLightDiffuseColor(0, lightColor);
				_lights->SetLightDiffuseColor(1, lightColor);
				_lights->SetLightDiffuseColor(2, lightColor);
			}

			// �t�H�O
			auto _fog = dynamic_cast<IEffectFog*>(effect);
			if (_fog)
			{
				// �����g���V�F�[�_�[�ɐ؂�ւ���
				_fog->SetFogEnabled(true);

				// �t�H�O�̐F�����߂�
				_fog->SetFogColor(Colors::LightGray);

				// �X�^�[�g
				_fog->SetFogStart(-50.0f);

				// �G���h
				_fog->SetFogEnd(150.0f);
			}
		};

		if (m_mapObj[i].GetID() == MAPSTATE::GRASS)
		{
			m_grassModel->UpdateEffects(_lightSetting);
			m_grassModel->Draw(_context, states, _world, view, proj);
		}
	}
	// �R�C���̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].GetID() != MAPSTATE::COIN) continue;

		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].GetPosition());
		m_coinModel->Draw(_context, states, _rotate * _world, view, proj);
	}
	// �_�u���b�N�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].GetID() != MAPSTATE::CLOUD) continue;

		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].GetPosition());
		m_cloudModel->Draw(_context, states, _rotate * _world, view, proj);
	}
	// �d�̓u���b�N�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].GetID() != MAPSTATE::GRAVITY) continue;

		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].GetPosition());
		m_gravityModel->Draw(_context, states, (_rotate * 2.0f) * _world, view, proj, false, [&]()
			{
				_context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xffffffff);
				_context->OMSetDepthStencilState(states.DepthDefault(), 0);
				_context->PSSetShader(m_psGravity.Get(), nullptr, 0);
			}
		);
	}
}

// �I������
void Blocks::Finalize()
{
	// �}�b�v�̉��
	m_mapObj.clear();

	// �Ō�܂Ń��Z�b�g����Ă�����X�L�b�v
	if (not m_gravityModel) return;

	m_factory->BuildModelFactory();

	m_factory->VisitModelFactory()->DeleteModel(m_grassModel);
	m_factory->VisitModelFactory()->DeleteModel(m_coinModel);
	m_factory->VisitModelFactory()->DeleteModel(m_cloudModel);
	m_factory->VisitModelFactory()->DeleteModel(m_gravityModel);

	m_factory->LeaveModelFactory();
	m_factory.reset();
}

// ���f���쐬�֐�
void Blocks::CreateModels(std::unique_ptr<Model> model,int modelNum)
{
	switch (modelNum)
	{
	case GRASS:		m_grassModel = std::move(model);	break;
	case COIN:		m_coinModel = std::move(model);		break;
	case CLOWD:		m_cloudModel = std::move(model);	break;
	case GRAVITY:	m_gravityModel = std::move(model);	break;
	default: break;
	}
}

// �V�F�[�_�[�쐬
void Blocks::CreateShader()
{
	// �t�@�N�g���ŃV�F�[�_�[�𐶐�
	m_factory->BuildShaderFactory();

	m_factory->VisitShaderFactory()->CreatePixelShader(L"Resources/Shaders/PS_Gravity.cso", &m_psGravity);

	m_factory->LeaveShaderFactory();
}

// �R�C���̃J�E���g�A�b�v
void Blocks::CountUpCoin(int index)
{
	m_coinCount++;
	m_mapObj[index].SetID(MAPSTATE::NONE);
}


// ���W���擾����
SimpleMath::Vector3& Blocks::GetBlockPosition(const int& index)
{
	if (index >= m_mapObj.max_size()) throw std::out_of_range("Index out of range");
	return m_mapObj[index].GetPosition();
}

// ���W��ݒ肷��
void Blocks::SetBlockPosition(const DirectX::SimpleMath::Vector3& newPos, const int& index)
{
	m_mapObj[index].SetPosition(newPos);
}

// �I�u�W�F�N�g�̑傫�����擾
const float& Blocks::GetObjSize(const int& objName)

{
	if (objName == MAPSTATE::COIN)
	{
		// �R�C���͏����߃T�C�Y
		return COIN_SIZE;
	}
	else if (objName == MAPSTATE::CLOUD)
	{
		// �_�������߃T�C�Y
		return CLOUD_SIZE;
	}
	else if (objName == MAPSTATE::GRAVITY)
	{
		// �d�͂͏����傫�߃T�C�Y
		return GRAVITY_SIZE;
	}
	else
	{
		// �f�t�H���g�T�C�Y
		return COMMON_SIZE;
	}
}

// �_�����ɖ߂�
void Blocks::CallGravity()
{
	for (auto& obj : m_mapObj)
	{
		// �_�݂̂�ΏۂƂ���
		if (obj.GetID() == MAPSTATE::CLOUD)
		{
			// Y���W���n�_�܂œ�����
			float _move = UserUtility::Lerp
			(
				obj.GetPosition().y,
				m_cloudState[obj.GetIndex()].initPosition.y,
				CLOUD_SPEED * 0.5f
			);

			obj.SetPosition(
				SimpleMath::Vector3(
					obj.GetPosition().x,
					_move,
					obj.GetPosition().z
				)
			);
			m_cloudState[obj.GetIndex()].moveFlag = false;
		}
	}
}

// �}�b�v�I��
std::wstring Blocks::MapSelect(int num)
{
	std::wstring _filePath;

	// TODO: [�X�e�[�W�ԍ�]�}�b�v�ǉ��͂�������I
	// �}�b�v�̕ύX�@case �X�e�[�W�ԍ�
	switch (num)
	{
	case 0:
		_filePath = L"Datas/Maps/StageEdit.csv";
		break;
	case 1:
		_filePath = L"Datas/Maps/Stage1.csv";
		break;
	case 2:
		_filePath = L"Datas/Maps/Stage2.csv";
		break;
	case 3:
		_filePath = L"Datas/Maps/Stage3.csv";
		break;
	case 4:
		_filePath = L"Datas/Maps/Stage4.csv";
		break;
	case 5:
		_filePath = L"Datas/Maps/Stage5.csv";
		break;
	default:
		_filePath = L"NoStage";
		break;
	}

	return _filePath;
}

// �}�b�v�̍��W�␳
void Blocks::MapSwipe()
{
	// ���W�␳
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		// �C���f�b�N�X�ԍ��̊i�[
		m_mapObj[i].SetIndex(i);

		// �u���b�N���Ȃ��Ƃ��͏������Ȃ�
		if (m_mapObj[i].GetID() == MAPSTATE::NONE) continue;

		// �}�b�v�̍��W�ݒ�
		SimpleMath::Vector3 _pos = m_mapObj[i].GetPosition();
		_pos.x -= static_cast<float>(m_mapLoad->MAP_COLUMN) / 2 * COMMON_SIZE;
		_pos.y += static_cast<float>(COMMON_LOW);
		_pos.z -= static_cast<float>(m_mapLoad->MAP_COLUMN) / 2 * COMMON_SIZE;
		m_mapObj[i].SetPosition(_pos);


		// �R�C���̖����̃J�E���g
		if (m_mapObj[i].GetID() == MAPSTATE::COIN)
		{
			m_maxCoins++;
		}
		// �_�t���O�o�^
		if (m_mapObj[i].GetID() == MAPSTATE::CLOUD)
		{
			// �X�e�[�^�X�o�^
			m_cloudState[i].moveFlag = false;

			// �n���_��ۑ�
			m_cloudState[i].initPosition = m_mapObj[i].GetPosition();

			// �I���_��ۑ�
			m_cloudState[i].endPosition = m_mapObj[i].GetPosition() +
			SimpleMath::Vector3(0.0f,COMMON_SIZE + CLOUD_SIZE,0.0f);
		}
		// �v���C���̍��W����
		if (m_mapObj[i].GetID() == MAPSTATE::BIRD)
		{
			m_playerPos = m_mapObj[i].GetPosition() +
				SimpleMath::Vector3(0.0f, COMMON_SIZE / 2, 0.0f);

			// �����ɊY���}�X���Ȃ��ɕς���(���菜��)
			m_mapObj[i].SetID(MAPSTATE::NONE);
		}
	}
}