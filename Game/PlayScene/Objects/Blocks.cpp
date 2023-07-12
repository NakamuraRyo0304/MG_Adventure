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
Blocks::Blocks():
	m_coinCount{0},							// �R�C���J�E���^
	m_maxCoins{0},							// �R�C���ő�l
	is_collectedFlag{ false },				// �R�C���t���O
	is_hitCoinFlag{ false },				// ����t���O
	m_playerPos{SimpleMath::Vector3::Zero},	// �v���C���[�|�W�V����
	m_grassModel{ nullptr },				// ���u���b�N�̃��f��
	m_coinModel{ nullptr },					// �R�C���u���b�N�̃��f��
	m_moveModel{ nullptr }					// �_�u���b�N�̃��f��
{
	m_map = std::make_unique<MapLoad>();
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
	m_map->LoadMap(filePath);

	// �}�b�v�̊i�[
	m_mapObj = m_map->GetMapData();

	// �������̉��
	m_map->ReleaseMemory();

	// ���W�␳
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		// �C���f�b�N�X�ԍ��̊i�[
		m_mapObj[i].index = i;
		
		// �}�b�v�̍��W�ݒ�
		m_mapObj[i].position.x -= static_cast<float>(m_map->MAP_COLUMN) / 2 * COMMON_SIZE;
		m_mapObj[i].position.y += static_cast<float>(COMMON_LOW);
		m_mapObj[i].position.z -= static_cast<float>(m_map->MAP_COLUMN) / 2 * COMMON_SIZE;

		// �R�C���̖����̃J�E���g
		if (m_mapObj[i].id == MapState::CoinBox)
		{
			m_maxCoins++;
		}
		// �_�t���O�o�^
		if (m_mapObj[i].id == MapState::ClowdBox)
		{
			// �X�e�[�^�X�o�^
			m_clowdState[i].moveFlag = false;

			// �n���_��ۑ�
			m_clowdState[i].initPosition = m_mapObj[i].position;

			// �I���_��ۑ�
			m_clowdState[i].endPosition = SimpleMath::Vector3
			{
				m_mapObj[i].position.x, 
				m_mapObj[i].position.y + COMMON_SIZE + CLOWD_SIZE,
				m_mapObj[i].position.z 
			};
		}
		// �v���C���̍��W����
		if (m_mapObj[i].id == MapState::PlayerPos)
		{
			m_playerPos = SimpleMath::Vector3
			{
				m_mapObj[i].position.x,
				m_mapObj[i].position.y + COMMON_SIZE / 2,
				m_mapObj[i].position.z
			};
			
			// �����ɊY���}�X����C�ɕς���(���菜��)
			m_mapObj[i].id = MapState::None;
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
		if (m_mapObj[i].id == MapState::ClowdBox)
		{
			// �G��Ă���Ƃ��͏I�_�܂�
			if (m_clowdState[i].moveFlag)
			{
				// Y���W���I�_�܂œ�����
				m_mapObj[i].position.y = UserUtility::Lerp
				(
					m_mapObj[i].position.y,
					m_clowdState[i].endPosition.y,
					0.1f
				);
			}
			else
			{
				// Y���W���I�_�܂œ�����
				m_mapObj[i].position.y = UserUtility::Lerp
				(
					m_mapObj[i].position.y,
					m_clowdState[i].initPosition.y,
					0.1f
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

		if (m_mapObj[i].id == MapState::GrassBox)
		{
			m_grassModel->Draw(context, states, world, view, proj);
		}
		if (m_mapObj[i].id == MapState::CoinBox)
		{
			m_coinModel->Draw(context, states, rotateY * world, view, proj);
		}
		if (m_mapObj[i].id == MapState::ClowdBox)
		{
			m_moveModel->Draw(context, states, rotateY * world, view, proj);
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
	ModelFactory::DeleteModel(m_moveModel);
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
		m_moveModel = std::move(model);
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
	m_mapObj[index].id = MapState::None;
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
/// �}�b�v�I��
/// </summary>
/// <param name="num">�X�e�[�W�ԍ�</param>
/// <returns>�t�@�C���p�X</returns>
std::wstring Blocks::MapSelect(int num)
{
	std::wstring filePath;

	// �}�b�v�̕ύX
	switch (num) // �}�b�v�ǉ��͂�������I
	{
	case 1:
		filePath = L"Resources/Maps/Stage1.csv";
		break;
	case 2:
		filePath = L"Resources/Maps/Stage2.csv";
		break;
	case 3:
		filePath = L"Resources/Maps/Stage3.csv";
		break;
	default:
		filePath = L"NoStage";
		break;
	}

	return filePath;
}

/// <summary>
/// �_�̍��W��߂�
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Blocks::ResetClowdPosition()
{
	for (int i = 0; i < m_mapObj.size(); ++i)
	{
		// �_�������珉���ʒu�ɖ߂�
		if (m_mapObj[i].id != MapState::ClowdBox) return;

		// �~�܂��Ă����珈�����Ȃ�
		if (!m_clowdState[i].moveFlag)return;

		// ���W��߂�
		m_mapObj[i].position.y = UserUtility::Lerp(
			m_mapObj[i].position.y,
			m_clowdState[i].initPosition.y,
			0.1f
		);

		// �n�_�ɂ�����~�߂�
		if (m_mapObj[i].position.y == m_clowdState[i].initPosition.y)
		{
			m_clowdState[i].moveFlag = false;
		}
	}
}
