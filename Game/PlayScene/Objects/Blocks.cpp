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
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="stageNum">�X�e�[�W�ԍ�</param>
/// <returns>�Ȃ�</returns>
void Blocks::Initialize(int stageNum)
{
	// �t�@�C�����̐錾
	std::wstring filePath = L"";

	// �X�e�[�W�Z���N�g
	filePath = MapSelect(stageNum);

	// �}�b�v�̓ǂݍ���
	m_mapLoad->LoadMap(filePath);

	// �}�b�v�̊i�[
	m_mapObj = m_mapLoad->GetMapData();

	// �������̉��
	m_mapLoad->ReleaseMemory();

	// ���W�␳
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		// �C���f�b�N�X�ԍ��̊i�[
		m_mapObj[i].index = i;

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
				m_mapObj[i].position.y + COMMON_SIZE + CLOWD_SIZE,
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

			// �����ɊY���}�X����C�ɕς���(���菜��)
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
/// <param name="elapsedTime">����</param>
/// <returns>�Ȃ�</returns>
void Blocks::Update(float elapsedTime)
{
	elapsedTime;

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
					CLOWD_SPEED
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
	// �I�u�W�F�N�g�̕`��
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		SimpleMath::Matrix world =
			SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		// ��]�s��
		SimpleMath::Matrix rotateY = SimpleMath::Matrix::CreateRotationY(timer);

		// �X�P�[���s��
		float restrictedTimer = fmodf(timer, 2 * XM_PI);
		SimpleMath::Matrix cl_scale = SimpleMath::Matrix::CreateScale(sinf(restrictedTimer));

		// ���C�g�̐ݒ�
		SimpleMath::Vector3 lightDirection(1.0f, -1.0f, -1.0f);

		// �r���[�s�񂩂�J�����̉�]���擾
		SimpleMath::Matrix cameraRotation;

		// �r���[�s����t�ϊ�
		cameraRotation = view.Invert();

		// �ړ��ʂ��Ȃ���
		cameraRotation._41 = 0.0f;
		cameraRotation._42 = 0.0f;
		cameraRotation._43 = 0.0f;

		// ���C�g�̕������J�����̉�]�ɋt�����ɂ���
		lightDirection = SimpleMath::Vector3::TransformNormal(lightDirection, cameraRotation);
		SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);

		auto setLightForModel = [&](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				// ���C�g�I��
				lights->SetLightEnabled(0, true);
				lights->SetLightEnabled(1, true);
				lights->SetLightEnabled(2, true);

				// ���C�g�̕�����ݒ�
				lights->SetLightDirection(0, lightDirection);
				lights->SetLightDirection(1, lightDirection);
				lights->SetLightDirection(2, lightDirection);

				// ���C�g�̐F���Â߂̃O���[�ɐݒ�
				lights->SetLightDiffuseColor(0, lightColor);
				lights->SetLightDiffuseColor(1, lightColor);
				lights->SetLightDiffuseColor(2, lightColor);
			}

			// �t�H�O
			auto fog = dynamic_cast<IEffectFog*>(effect);
			if (fog)
			{
				// �����g���V�F�[�_�[�ɐ؂�ւ���
				fog->SetFogEnabled(true);

				// �t�H�O�̐F�����߂�
				fog->SetFogColor(Colors::White);

				// �X�^�[�g
				fog->SetFogStart(-50.0f);

				// �G���h
				fog->SetFogEnd(150.0f);
			}
		};


		// ���u���b�N
		if (m_mapObj[i].id == MAPSTATE::GRASS)
		{
			m_grassModel->UpdateEffects(setLightForModel);
			m_grassModel->Draw(context, states, world, view, proj);
		}

		// �R�C���u���b�N
		if (m_mapObj[i].id == MAPSTATE::COIN)
		{
			m_coinModel->UpdateEffects(setLightForModel);
			m_coinModel->Draw(context, states, rotateY * world, view, proj);
		}

		// �_�u���b�N
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			if (m_cloudState[i].moveFlag)
			{
				// �F���Â��ύX
				ChangeModelColors(m_cloudModel, static_cast<SimpleMath::Vector4>(Colors::DarkGray));
			}
			else
			{
				// �F���f�t�H���g�ɖ߂�
				ChangeModelColors(m_cloudModel,static_cast<SimpleMath::Vector4>(Colors::White));
			}

			m_cloudModel->UpdateEffects(setLightForModel);
			m_cloudModel->Draw(context, states, rotateY * world, view, proj);
		}

		// �_�̃��Z�b�g�|�C���g
		if (m_mapObj[i].id == MAPSTATE::RESET)
		{
			// ���]�h�~
			if (sinf(restrictedTimer) < 0.0f)
			{
				cl_scale *= SimpleMath::Matrix::CreateScale(1.0f, -1.0f,1.0f);
			}

			m_resetPtModel->UpdateEffects(setLightForModel);
			m_resetPtModel->Draw(context, states, cl_scale * rotateY * world, view, proj);
		}
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

	// ���f���̉��
	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_cloudModel);
	ModelFactory::DeleteModel(m_resetPtModel);
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
	case RECLOWD:								// �_���Z�b�g
		m_resetPtModel = std::move(model);
		break;
	default:
		break;
	}
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
		return CLOWD_SIZE;
	}
	else if (objName == MAPSTATE::RESET)
	{
		// �_���Z�b�g�G���A�͑傫��s�T�C�Y
		return CLOWD_RESET_SIZE;
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
void Blocks::RestoreCloudPosition()
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
				CLOWD_SPEED * 0.5f
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
	std::wstring filePath;

	// TODO: [�X�e�[�W�ԍ�]�}�b�v�ǉ��͂�������I
	// �}�b�v�̕ύX
	switch (num)
	{
	case 0:
		filePath = L"Resources/Maps/StageEdit.csv";
		break;
	case 1:
		filePath = L"Resources/Maps/Stage1.csv";
		break;
	case 2:
		filePath = L"Resources/Maps/Stage2.csv";
		break;
	case 3:
		filePath = L"Resources/Maps/Stage3.csv";
		break;
	case 4:
		filePath = L"Resources/Maps/Stage4.csv";
		break;
	case 5:
		filePath = L"Resources/Maps/Stage5.csv";
		break;
	case 6:
		filePath = L"Resources/Maps/Stage6.csv";
		break;
	case 7:
		filePath = L"Resources/Maps/Stage7.csv";
		break;
	case 8:
		filePath = L"Resources/Maps/Stage8.csv";
		break;
	case 9:
		filePath = L"Resources/Maps/Stage9.csv";
		break;
	default:
		filePath = L"NoStage";
		break;
	}

	return filePath;
}

/// <summary>
/// ���f���̐F��ύX
/// </summary>
/// <param name="model">���f���f�[�^</param>
/// <param name="color">�F���</param>
/// <returns>�Ȃ�</returns>
void Blocks::ChangeModelColors(std::unique_ptr<Model>& model, SimpleMath::Vector4 color)
{
	model->UpdateEffects([&](IEffect* effect)
		{
			// �x�[�V�b�N�G�t�F�N�g��錾
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);

			if (basicEffect)
			{
				// �F��ύX����
				basicEffect->SetDiffuseColor(color);
			}
		}
	);
}