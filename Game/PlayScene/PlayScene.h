/*
 *	@File	PlayScene.h
 *	@Brief  �v���C�V�[���B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYSCENE
#define PLAYSCENE

#include "../IScene.h"

class Blocks;
class Player;
class PlayerBill;
class PlayUI;
class PlayCamera;
class ThirdPersonCamera;
class PlayScene final : public IScene
{
private:

	// �X�^�[�g�^�C�}�[
	float m_startTimer;

	// ��������
	float m_gameTimer;
	float m_clearTime;

	// ��̐F�J��
	struct Color
	{
		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;
	};
	Color m_skyColor;

	// �}�b�v
	MapLoad m_mapLoad;
	// �}�b�v�ԍ�
	int m_stageNum;

	// �������o�ϐ�
	float m_fallValue;

	// �����蔻��
	Collider::BoxCollider is_hitCol;
	std::vector<Object> m_hitObj;
	Object m_lastObj;

	// �O��̃C���f�b�N�X
	std::deque<int> m_prevIndex;

	// �w���v�\���t���O
	bool is_helpFlag;

	// �T�[�h�p�[�\�����[�h
	bool is_thirdPersonMode;

	// ���v�R�C����
	int m_allCoins;

	// ���C�e�B���O
	DirectX::SimpleMath::Vector3 m_lighting;

private:

	// �v���C��
	std::unique_ptr<Player> m_player;

	// �u���b�N
	std::unique_ptr<Blocks> m_blocks;

	// ���f��
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// �v���C���[�̖ڈ�i�r���{�[�h�j
	std::unique_ptr<PlayerBill> m_playerBill;

	// �^�C�}�[�\���ƃR�C���\��
	std::unique_ptr<PlayUI> m_playUI;

	// �T�[�h�p�[�\���J����
	std::unique_ptr<ThirdPersonCamera> m_thirdCamera;

	// �X�^�[�g�J����
	std::unique_ptr<PlayCamera> m_playCamera;

private:

	// ��������(�b��)
	const int TIME_LIMIT = 60;

	// �t���[�����[�g
	const float FLAME_RATE = 60.0f;

	// �X�J�C�h�[���̉�]���x
	const float SKY_ROT_SPEED = 0.02f;

	// �}�b�v�T�C�Y(Stage)
	const float COMMON_SIZE = 0.9f;

	// �Œፂ�x
	const float	COMMON_LOW = COMMON_SIZE / 2;

	// �����蔻��̔��a
	const float JUDGE_AREA = COMMON_SIZE * 1.5f;

	// �J�����A���O��
	const float	CAMERA_ANGLE = 44.5f;

	// �u���b�N���󃉃C��
	const float DURATION_FLOOR_LINE = -15.0f;

	// ���S�㗎�����x
	const float FALL_SPEED = 0.005f;

	// �U���ݒ�
	const float SHAKE_DURATION = 4.0f;		// ��������
	const float SHAKE_TREMOR = 1.0f;		// �U���͈�

	// �J�E���g�_�E���̑��x
	const float COUNT_SPEED = 0.5f;

	// �ŏ��̃J�������o�I�_
	const float MOVE_CAMERA_SPEED = 0.05f;
	const DirectX::SimpleMath::Vector3 START_CAMERA_POS = { 0.0f,50.0f,40.0f };

public:

	PlayScene();
	~PlayScene();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update() override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Draw() override;

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize() override;

	/// <summary>
	/// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CreateWindowDependentResources() override;

private:

	/// <summary>
	/// �V�[�����̕ϐ��������֐�
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void SetSceneValues() override;

private:

	/// <summary>
	/// ���C�e�B���O�̍X�V
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void InitializeLighting();

	/// <summary>
	/// �v���C���[�̍쐬
	/// </summary>
	/// <param name="device">�f�o�C�X�|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void MakePlayer(ID3D11Device1* device);

	/// <summary>
	/// �u���b�N�̍쐬
	/// </summary>
	/// <param name="device">�f�o�C�X�|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void MakeBlocks(ID3D11Device1* device);

	/// <summary>
	/// �X�^�[�g�̃J�E���g�_�E��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�I����Ă�����True��Ԃ�</returns>
	bool StartTimer();

	/// <summary>
	/// �X�^�[�g���o
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void MoveStart();

	/// <summary>
	/// ��̍X�V
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void UpdateSky();

	/// <summary>
	/// UI�̍X�V
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�w���v�t���O</returns>
	bool UpdateUI();

	/// <summary>
	/// �w���v�V�[���őI�����ꂽ�V�[���ւ̑J��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void HelpNext();

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Judgement();

	/// <summary>
	/// �����߂�����
	/// </summary>
	/// <param name="obj">���������I�u�W�F�N�g</param>
	/// <returns>�Ȃ�</returns>
	void ApplyPushBack(Object& obj);

public:

	// �^�C�����~�b�g�̎擾
	const float& GetClearTime() { return m_clearTime; }
	/// �l�������R�C�����擾
	const int& GetCoinNum();
	/// �X�e�[�W�̍ő�R�C�����擾
	const int& GetMaxCoinCount();
	// �}�b�v�ԍ��̎擾
	const int& GetStageNum() { return m_stageNum; }
	// �}�b�v�ԍ��̐ݒ�
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }
	// ���v�R�C�����̐ݒ�
	void SetAllCoinNum(const int& allCoins) { m_allCoins = allCoins; }

};

#endif // PLAYSCENE