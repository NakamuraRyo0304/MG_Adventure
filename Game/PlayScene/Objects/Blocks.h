/*
 *	@File	Blocks.h
 *	@Brief	�}�b�v�̃u���b�N�Ǘ��B
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

// �}�b�v�̎g�p
#include <map>

// �G�C���A�X
using MapState = MapLoad::BoxState;

class Blocks
{
private:

	// �}�b�v���[�_�[
	std::unique_ptr<MapLoad> m_mapLoad;

	// �u���b�N
	std::vector<Object> m_mapObj;

	// �_�X�e�[�^�X
	struct Clowd
	{
		bool moveFlag = false;
		float timer = 0.0f;
		DirectX::SimpleMath::Vector3 initPosition = DirectX::SimpleMath::Vector3::Zero;
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
	const float CLOWD_SIZE = COMMON_SIZE / 1.7f;
	const float CLOWD_SPEED = 0.1f;

	// �Œፂ�x
	const float	COMMON_LOW = COMMON_SIZE / 2;


private:
	// ���f��
	std::unique_ptr<DirectX::Model> m_grassModel;
	std::unique_ptr<DirectX::Model> m_coinModel;
	std::unique_ptr<DirectX::Model> m_moveModel;

public:
	// �u���b�N�̎��
	enum { GRASS, COIN, CLOWD, LENGTH };

	// ���C�e�B���O�ݒ�p�\����
	struct BOOL3
	{
		bool _1 = false;
		bool _2 = false;
		bool _3 = false;
	};

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
	void CreateModels(std::unique_ptr<Model> model, int modelName);

	// �R�C���̃J�E���g�A�b�v
	void CountUpCoin(int index);
	const int& GetCoinCount() { return m_coinCount; }
	const bool& IsCollectedFlag();

	// �v���C���[���W�Q�b�^�[
	const DirectX::SimpleMath::Vector3& GetPlayerPosition() { return m_playerPos; }

	// �}�b�v�S�̂̃Q�b�^�[
	const std::vector<Object>& GetMapData() { return m_mapObj; }
	// �u���b�N�P�̂̍��W�Q�b�^�[
	DirectX::SimpleMath::Vector3& GetBlockPosition(const int& index) { return m_mapObj[index].position; }
	void SetBlockPosition(const DirectX::SimpleMath::Vector3& newPos, const int& index) { m_mapObj[index].position = newPos; }

	// �u���b�N�̑傫���Q�b�^�[
	const float& GetObjSize(const int& objName)
	{
		if (objName == MapState::CoinBox)
		{
			// �R�C���͏����߃T�C�Y
			return COIN_SIZE;
		}
		else if (objName == MapState::ClowdBox)
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

	// �_�����̏ꏊ�ɖ߂�����
	void RestoreClowdPosition(const int& index);

	// �_�̔���A�N�Z�T
	void SetClowdHitFlag(const int& index, bool flag) { m_clowdState[index].moveFlag = flag; }

private:

	// �}�b�v�Z���N�g
	std::wstring MapSelect(int num);

private:

	// ���f���̐F��ύX
	void ChangeModelColors(std::unique_ptr<Model>& model,DirectX::SimpleMath::Vector4 color);

	// ���f���̌���ύX
	void ChangeModelLights(std::unique_ptr<Model>& model, BOOL3 flag);
};