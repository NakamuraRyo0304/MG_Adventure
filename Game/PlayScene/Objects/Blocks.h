/*
 *	@File	Blocks.h
 *	@Brief	�}�b�v�̃u���b�N�Ǘ��B
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

// �}�b�v�̎g�p
#include <map>

// �G�C���A�X
using MAPSTATE = MapLoad::BOXSTATE;

class Blocks
{
private:

	// �}�b�v���[�_�[
	std::unique_ptr<MapLoad> m_mapLoad;

	// �u���b�N
	std::vector<Object> m_mapObj;

	// �_�X�e�[�^�X
	struct Cloud
	{
		bool moveFlag = false;
		float timer = 0.0f;
		DirectX::SimpleMath::Vector3 initPosition = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 endPosition = DirectX::SimpleMath::Vector3::Zero;
	};

	// �t���O�Ǘ�
	std::map<int, Cloud> m_cloudState;

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

	// �u���b�N�̃V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_psCloud;

private:
	// �}�b�v�T�C�Y
	const float COMMON_SIZE = 0.9f;

	// �R�C���̃T�C�Y
	const float COIN_SIZE = COMMON_SIZE / 3;

	// �_�̃T�C�Y
	const float CLOUD_SIZE = COMMON_SIZE / 1.7f;
	const float CLOUD_SPEED = 0.1f;

	// �_���Z�b�g�G���A�T�C�Y
	const float CLOUD_RESET_SIZE = 0.85f;

	// �Œፂ�x
	const float	COMMON_LOW = COMMON_SIZE / 2;


private:
	// ���f��
	std::unique_ptr<DirectX::Model> m_grassModel;
	std::unique_ptr<DirectX::Model> m_coinModel;
	std::unique_ptr<DirectX::Model> m_cloudModel;
	std::unique_ptr<DirectX::Model> m_resetPtModel;

public:
	// �u���b�N�̎��
	enum { GRASS, COIN, CLOWD, RECLOWD, LENGTH };

public:

	// �R���X�g���N�^
	Blocks();

	// �f�X�g���N�^
	~Blocks();

	// ������
	void Initialize(int stageNum);

	// �X�V
	void Update();

	// �`��
	void Render(ID3D11DeviceContext* context, DirectX::CommonStates& states,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, float timer);

	// �I������
	void Finalize();

	// ���f���쐬
	void CreateModels(std::unique_ptr<Model> model, int modelName);

	// �V�F�[�_�[�̍쐬
	void CreateShader(ID3D11Device1* device);

public:
	// �R�C���̃J�E���g�A�b�v
	void CountUpCoin(int index);
	const int& GetCoinCount() { return m_coinCount; }
	const int& GetMaxCoinCount() { return m_maxCoins; }
	const bool& IsCollectedFlag();

	// �v���C���[���W�Q�b�^�[
	const DirectX::SimpleMath::Vector3& GetPlayerPosition() { return m_playerPos; }

	// �}�b�v�S�̂̃Q�b�^�[
	const std::vector<Object>& GetMapData() { return m_mapObj; }

	// �u���b�N�P�̂̍��W�Q�b�^�[
	DirectX::SimpleMath::Vector3& GetBlockPosition(const int& index)
	{
		if (index >= m_mapObj.max_size()) throw std::out_of_range("Index out of range");
		return m_mapObj[index].position;
	}
	void SetBlockPosition(const DirectX::SimpleMath::Vector3& newPos, const int& index) { m_mapObj[index].position = newPos; }

	// �u���b�N�̑傫���Q�b�^�[
	const float& GetObjSize(const int& objName);

	// �_�����̏ꏊ�ɖ߂�����
	void RestoreCloudPosition();

	// �_�̔���A�N�Z�T
	void SetCloudHitFlag(const int& index, bool flag) { m_cloudState[index].moveFlag = flag; }

private:

	// �}�b�v�Z���N�g
	std::wstring MapSelect(int num);
	// ���f���̐F��ύX
	void ChangeModelColors(std::unique_ptr<Model>& model,DirectX::SimpleMath::Vector4 color);
};