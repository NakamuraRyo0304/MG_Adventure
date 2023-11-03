/*
 *	@File	Blocks.h
 *	@Brief	�}�b�v�̃u���b�N�Ǘ��B
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

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

	// �_�̃V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_psCloud;

	// ���C�e�B���O
	DirectX::SimpleMath::Vector3 m_lighting;

private:
	// �}�b�v�T�C�Y
	const float COMMON_SIZE = 0.9f;

	// �R�C���̃T�C�Y
	const float COIN_SIZE = COMMON_SIZE / 3;

	// �_�̃T�C�Y
	const float CLOUD_SIZE = COMMON_SIZE / 1.7f;
	const float CLOUD_SPEED = 0.1f;

	// �_���Z�b�g�G���A�T�C�Y
	const float GRAVITY_SIZE = 0.85f;

	// �Œፂ�x
	const float	COMMON_LOW = COMMON_SIZE / 2;


private:
	// ���f��
	std::unique_ptr<DirectX::Model> m_grassModel;
	std::unique_ptr<DirectX::Model> m_coinModel;
	std::unique_ptr<DirectX::Model> m_cloudModel;
	std::unique_ptr<DirectX::Model> m_gravityModel;

public:
	// �u���b�N�̎��
	enum { GRASS, COIN, CLOWD, GRAVITY, LENGTH };

public:

	Blocks();
	~Blocks();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	/// <returns>�Ȃ�</returns>
	void Initialize(int stageNum);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="states">�R�����X�e�[�g</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�ˉe�s��</param>
	/// <param name="timer">�o�ߎ���</param>
	/// <param name="lightDir">���C�e�B���O</param>
	/// <returns>�Ȃ�</returns>
	void Render(DirectX::CommonStates& states,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, float timer,
		const DirectX::SimpleMath::Vector3& lightDir);

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize();

	/// <summary>
	/// ���f���쐬
	/// </summary>
	/// <param name="model">���f���f�[�^</param>
	/// <param name="modelNum">�Ή��������f���ԍ�</param>
	/// <returns>�Ȃ�</returns>
	void CreateModels(std::unique_ptr<Model> model, int modelName);

public:

	/// <summary>
	/// �V�F�[�_�[�̍쐬
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CreateShader();

	/// <summary>
	/// �R�C���擾����
	/// </summary>
	/// <param name="index">���������R�C���̔ԍ�</param>
	/// <returns>�Ȃ�</returns>
	void CountUpCoin(int index);

	/// <summary>
	/// �_�̈ʒu�����Ƃɖ߂�����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CallGravity();

	// ���C�e�B���O�̃Z�b�^�[
	void InitializeLighting(const DirectX::SimpleMath::Vector3& lightDir) { m_lighting = lightDir; }
public:

	// �擾�R�C�������擾
	const int& GetCoinCount() { return m_coinCount; }
	// �ő�R�C�������擾
	const int& GetMaxCoinCount() { return m_maxCoins; }
	// �R�C���̉������
	const bool& IsCollectedFlag() { return is_collectedFlag; }
	// �v���C���[���W���擾
	const DirectX::SimpleMath::Vector3& GetPlayerPosition() { return m_playerPos; }

	// �}�b�v�S�̂��擾
	const std::vector<Object>& GetMapData() { return m_mapObj; }

	// �u���b�N�P�̂̍��W���擾
	DirectX::SimpleMath::Vector3& GetBlockPosition(const int& index)
	{
		if (index >= m_mapObj.max_size()) throw std::out_of_range("Index out of range");
		return m_mapObj[index].position;
	}
	void SetBlockPosition(const DirectX::SimpleMath::Vector3& newPos, const int& index) { m_mapObj[index].position = newPos; }

	/// <summary>
	/// �T�C�Y�Q�b�^�[
	/// </summary>
	/// <param name="objName">�I�u�W�F�N�g�̖��O(�\����)</param>
	/// <returns>�Ȃ�</returns>
	const float& GetObjSize(const int& objName);
	// �_�̔���A�N�Z�T
	void SetCloudHitFlag(const int& index, bool flag) { m_cloudState[index].moveFlag = flag; }

private:

	/// <summary>
	/// �}�b�v�I��
	/// </summary>
	/// <param name="num">�X�e�[�W�ԍ�</param>
	/// <returns>�t�@�C���p�X</returns>
	std::wstring MapSelect(int num);
};