/*
 *	@File	Blocks.cpp
 *	@Brief	�}�b�v�̃u���b�N�Ǘ��B
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

#include "pch.h"

// CSV�ǂݍ���
#include "../../../Libraries/SystemDatas/MapLoad.h"

// ���f���t�@�N�g��
#include "../../../Libraries/Factories/ModelFactory.h"

// ���[�U�[���[�e�B���e�B
#include "../../../Libraries/UserUtility.h"

// �f�[�^�ǂݍ���
#include "Libraries/ReadData.h"

#include "Blocks.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
Blocks::Blocks()
	: m_coinCount{0}							// �R�C���J�E���^
	, m_maxCoins{0}								// �R�C���ő�l
	, m_playerPos{ SimpleMath::Vector3::Zero }	// �v���C���[�|�W�V����
	, m_grassModel{ nullptr }					// ���u���b�N�̃��f��
	, m_coinModel{ nullptr }					// �R�C���u���b�N�̃��f��
	, m_cloudModel{ nullptr }					// �_�u���b�N�̃��f��
	, m_gravityModel{ nullptr }					// �d�̓u���b�N�̃��f��
	, is_collectedFlag{ false }					// �R�C������ςݔ���t���O
	, is_hitCoinFlag{ false }					// ����t���O
{
	m_mapLoad = std::make_unique<MapLoad>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Blocks::~Blocks()
{

}

/// <summary>
/// ����������
/// </summary>
/// <param name="stageNum">�X�e�[�W�ԍ�</param>
/// <returns>�Ȃ�</returns>
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

	// ���W�␳
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		// �C���f�b�N�X�ԍ��̊i�[
		m_mapObj[i].index = i;

		// �u���b�N���Ȃ��Ƃ��͏������Ȃ�
		if (m_mapObj[i].id == MAPSTATE::NONE) continue;

		// �}�b�v�̍��W�ݒ�
		m_mapObj[i].position.x -= static_cast<float>(m_mapLoad->MAP_COLUMN) / 2 * COMMON_SIZE;
		m_mapObj[i].position.y += static_cast<float>(COMMON_LOW);
		m_mapObj[i].position.z -= static_cast<float>(m_mapLoad->MAP_COLUMN) / 2 * COMMON_SIZE;

		// �R�C���̖����̃J�E���g
		if (m_mapObj[i].id == MAPSTATE::COIN)
		{
			m_maxCoins++;
		}
		// �_�t���O�o�^
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			// �X�e�[�^�X�o�^
			m_cloudState[i].moveFlag = false;

			// �n���_��ۑ�
			m_cloudState[i].initPosition = m_mapObj[i].position;

			// �I���_��ۑ�
			m_cloudState[i].endPosition = SimpleMath::Vector3
			{
				m_mapObj[i].position.x,
				m_mapObj[i].position.y + COMMON_SIZE + CLOUD_SIZE,
				m_mapObj[i].position.z
			};
		}
		// �v���C���̍��W����
		if (m_mapObj[i].id == MAPSTATE::PLAYER)
		{
			m_playerPos = SimpleMath::Vector3
			{
				m_mapObj[i].position.x,
				m_mapObj[i].position.y + COMMON_SIZE / 2,
				m_mapObj[i].position.z
			};

			// �����ɊY���}�X���Ȃ��ɕς���(���菜��)
			m_mapObj[i].id = MAPSTATE::NONE;
		}
	}

	// �R�C������ς݃t���O��False�ɂ���
	is_collectedFlag = false;
	// �����蔻��t���O��False�ɂ���
	is_hitCoinFlag = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Blocks::Update()
{
	// �_�͏㉺�ړ�����
	for(int i = 0; i < m_mapObj.size();++i)
	{
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			// �G��Ă���Ƃ��͏I�_�܂�
			if (m_cloudState[i].moveFlag)
			{
				// Y���W���I�_�܂œ�����
				m_mapObj[i].position.y = UserUtility::Lerp
				(
					m_mapObj[i].position.y,
					m_cloudState[i].endPosition.y,
					CLOUD_SPEED
				);
			}
		}
	}

	// �R�C�������������True�ɂ���
	if (m_coinCount == m_maxCoins)
	{
		is_collectedFlag = true;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="context">ID3D11DeviceContext�|�C���^</param>
/// <param name="states">�R�����X�e�[�g</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <param name="timer">�o�ߎ���</param>
/// <returns>�Ȃ�</returns>
void Blocks::Render(ID3D11DeviceContext* context, CommonStates& states,
	SimpleMath::Matrix view, SimpleMath::Matrix proj, float timer)
{
	// ���[���h���W
	SimpleMath::Matrix _world, _rotMat, _revScaleMat;

	// ���C�e�B���O�ݒ�
	std::function<void(IEffect* effect)> _lightSetting;

	// �_�ȊO�̃I�u�W�F�N�g�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id == MAPSTATE::NONE) continue;

		// �ړ��s��
		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		// ��]�s��
		_rotMat = SimpleMath::Matrix::CreateRotationY(timer);

		// �X�P�[���s��
		float _revScaleTimer = fmodf(timer, 2 * XM_PI);
		_revScaleMat = SimpleMath::Matrix::CreateScale(sinf(_revScaleTimer));

		// ���C�g�̐ݒ�
		SimpleMath::Vector3 _lightDir(1.0f, -1.0f, -1.0f);

		// �r���[�s�񂩂�J�����̉�]���擾
		SimpleMath::Matrix _cameraRot;

		// �r���[�s����t�ϊ�
		_cameraRot = view.Invert();

		// �ړ��ʂ��Ȃ���
		_cameraRot._41 = 0.0f;
		_cameraRot._42 = 0.0f;
		_cameraRot._43 = 0.0f;

		// ���C�g�̕������J�����̉�]�ɋt�����ɂ���
		_lightDir = SimpleMath::Vector3::TransformNormal(_lightDir, _cameraRot);
		SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);

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
				_lights->SetLightDirection(0, _lightDir);
				_lights->SetLightDirection(1, _lightDir);
				_lights->SetLightDirection(2, _lightDir);

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
				_fog->SetFogColor(Colors::White);

				// �X�^�[�g
				_fog->SetFogStart(-50.0f);

				// �G���h
				_fog->SetFogEnd(150.0f);
			}
		};

		// ���u���b�N
		if (m_mapObj[i].id == MAPSTATE::GRASS)
		{
			m_grassModel->UpdateEffects(_lightSetting);
			m_grassModel->Draw(context, states, _world, view, proj);
		}

		// �R�C���u���b�N
		if (m_mapObj[i].id == MAPSTATE::COIN)
		{
			m_coinModel->UpdateEffects(_lightSetting);
			m_coinModel->Draw(context, states, _rotMat * _world, view, proj);
		}

		// �d�̓u���b�N
		if (m_mapObj[i].id == MAPSTATE::GRAVITY)
		{
			// ���]�h�~
			if (sinf(_revScaleTimer) < 0.0f)
			{
				_revScaleMat *= SimpleMath::Matrix::CreateScale(1.0f, -1.0f,1.0f);
			}

			m_gravityModel->UpdateEffects(_lightSetting);
			m_gravityModel->Draw(context, states, _revScaleMat * _rotMat * _world, view, proj);
		}
	}

	// �_�u���b�N
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id != MAPSTATE::CLOUD) continue;

		// �ړ��s��
		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		m_cloudModel->UpdateEffects(_lightSetting);
		m_cloudModel->Draw(context, states, _rotMat * _world, view, proj, false, [&]()
			{
				context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xffffffff);
				context->OMSetDepthStencilState(states.DepthDefault(), 0);
				context->PSSetShader(m_psCloud.Get(), nullptr, 0);
			}
		);
	}
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Blocks::Finalize()
{
	// �}�b�v�̉��
	m_mapObj.clear();

	// �Ō�܂Ń��Z�b�g����Ă�����X�L�b�v
	if (!m_gravityModel) return;

	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_cloudModel);
	ModelFactory::DeleteModel(m_gravityModel);
}

/// <summary>
/// ���f���쐬
/// </summary>
/// <param name="model">���f���f�[�^</param>
/// <param name="modelNum">�Ή��������f���ԍ�</param>
/// <returns>�Ȃ�</returns>
void Blocks::CreateModels(std::unique_ptr<Model> model,int modelNum)
{
	switch (modelNum)
	{
	case GRASS:									// ���u���b�N
		m_grassModel = std::move(model);
		break;
	case COIN:									// �R�C��
		m_coinModel = std::move(model);
		break;
	case CLOWD:									// �_�u���b�N
		m_cloudModel = std::move(model);
		break;
	case GRAVITY:								// �d�̓u���b�N
		m_gravityModel = std::move(model);
		break;
	default:
		break;
	}
}

/// <summary>
/// �V�F�[�_�[�̍쐬
/// </summary>
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
void Blocks::CreateShader(ID3D11Device1* device)
{
	// �s�N�Z���V�F�[�_�[
	std::vector<uint8_t> _psCloud = DX::ReadData(L"Resources/Shaders/PS_Cloud.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(_psCloud.data(), _psCloud.size(), nullptr,
			m_psCloud.ReleaseAndGetAddressOf()));
}

/// <summary>
/// �R�C���擾����
/// </summary>
/// <param name="index">���������R�C���̔ԍ�</param>
/// <returns>�Ȃ�</returns>
void Blocks::CountUpCoin(int index)
{
	m_coinCount++;
	m_mapObj[index].id = MAPSTATE::NONE;
}

/// <summary>
/// �R�C�����������
/// </summary>
/// <param name="��������"></param>
/// <returns>����I�������True</returns>
const bool& Blocks::IsCollectedFlag()
{
	return is_collectedFlag;
}

/// <summary>
/// �T�C�Y�Q�b�^�[
/// </summary>
/// <param name="objName">�I�u�W�F�N�g�̖��O(�\����)</param>
/// <returns>�Ȃ�</returns>
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

/// <summary>
/// �_�̈ʒu�����Ƃɖ߂�����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Blocks::CallGravity()
{
	for (auto& i : m_mapObj)
	{
		// �_�݂̂�ΏۂƂ���
		if (i.id == MAPSTATE::CLOUD)
		{
			// Y���W���n�_�܂œ�����
			i.position.y = UserUtility::Lerp
			(
				i.position.y,
				m_cloudState[i.index].initPosition.y,
				CLOUD_SPEED * 0.5f
			);
			m_cloudState[i.index].moveFlag = false;
		}
	}
}

/// <summary>
/// �}�b�v�I��
/// </summary>
/// <param name="num">�X�e�[�W�ԍ�</param>
/// <returns>�t�@�C���p�X</returns>
std::wstring Blocks::MapSelect(int num)
{
	std::wstring _filePath;

	// TODO: [�X�e�[�W�ԍ�]�}�b�v�ǉ��͂�������I
	// �}�b�v�̕ύX
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