/*
 *	@File	Blocks.h
 *	@Brief	�}�b�v�̃u���b�N�Ǘ��B
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

// �}�b�v�̎g�p
#include <map>

class Blocks
{
private:

	// �}�b�v���[�_�[
	std::unique_ptr<MapLoad> m_map;

	// �u���b�N
	std::vector<Object> m_mapObj;

	// �_�X�e�[�^�X
	struct Clowd 
	{
		bool moveFlag = false;
		DirectX::SimpleMath::Vector3 endPosition = DirectX::SimpleMath::Vector3::Zero;
	};

	// �t���O�Ǘ�
	std::map<int, Clowd> m_clowdState;

	// �v���C���[���W
	DirectX::SimpleMath::Vector3 m_playerPos;

	// �R�C���J�E���^
	int m_coinCount;
	// �R�C���̍ő�l
	int m_maxCoins;
	// �R�C������ς݃t���O
	bool is_collectedFlag;

	// �����������ǂ����̔���
	bool is_hitCoinFlag;

private:
	// �}�b�v�T�C�Y
	const float COMMON_SIZE = 0.9f;

	// �R�C���̃T�C�Y
	const float COIN_SIZE = COMMON_SIZE / 3;

	// �_�̃T�C�Y
	const float CLOWD_SIZE = COMMON_SIZE / 2;

	// �Œፂ�x
	const float	COMMON_LOW = COMMON_SIZE / 2;


private:
	// ���f��
	std::unique_ptr<DirectX::Model> m_grassModel;
	std::unique_ptr<DirectX::Model> m_coinModel;
	std::unique_ptr<DirectX::Model> m_moveModel;

public:
	// �u���b�N�̎��
	enum { GRASS, COIN, CLOWD , LENGTH };
public:

	// �R���X�g���N�^
	Blocks();

	// �f�X�g���N�^
	~Blocks();

	// ������
	void Initialize(int stageNum);

	// �X�V
	void Update(float elapsedTime);

	// �`��
	void Render(ID3D11DeviceContext* context, DirectX::CommonStates& states,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, float timer);

	// �I������
	void Finalize();

	// ���f���쐬
	void CreateModels(std::unique_ptr<Model> model,int modelName);

	// �R�C���̃J�E���g�A�b�v
	void CountUpCoin(int index);
	const int& GetCoinCount() { return m_coinCount; }
	const bool& IsCollectedFlag();

	// �v���C���[���W�Q�b�^�[
	const DirectX::SimpleMath::Vector3& GetPlayerPosition() { return m_playerPos; }

	// �}�b�v�S�̂̃Q�b�^�[
	const std::vector<Object>& GetMapData() { return m_mapObj; }

	// �u���b�N�̑傫���Q�b�^�[
	const float& GetObjSize(const int& objName)
	{
		if (objName == MapLoad::BoxState::CoinBox)
		{
			// �R�C���͏����߃T�C�Y
			return COIN_SIZE;
		}
		else if (objName == MapLoad::BoxState::ClowdBox)
		{
			// �_�������߃T�C�Y
			return CLOWD_SIZE;
		}
		else
		{
			// �f�t�H���g�T�C�Y
			return COMMON_SIZE;
		}
	}

	// �}�b�v�̔���Z�b�^�[
	void SetClowdHitFlag(int index,bool flag){ m_clowdState[index].moveFlag = flag;	}

private:

	// �}�b�v�Z���N�g
	std::wstring MapSelect(int num);
};