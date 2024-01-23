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
class ColliderHelper;
class Player;
class PlayerBill;
class PlayUI;
class PlayCamera;
class PlaySky;
class PlayScene final : public IScene
{
private:

	// �X�^�[�g�^�C�}�[
	float m_startTimer;

	// ��������
	float m_timeLinits;

	// �N���A����
	float m_clearTime;

	// �}�b�v�ԍ�
	int m_stageNum;

	// �������o�ϐ�
	float m_fallValue;

	// �w���v�\���t���O
	bool is_helpFlag;

	// ���v�R�C����
	int m_allCoins;

	// ���C�e�B���O
	DirectX::SimpleMath::Vector3 m_lighting;

private:

	// �����蔻��
	std::unique_ptr<ColliderHelper> m_collider;

	// �v���C��
	std::unique_ptr<Player> m_player;

	// �u���b�N
	std::unique_ptr<Blocks> m_blocks;

	// �v���C���[�̖ڈ�i�r���{�[�h�j
	std::unique_ptr<PlayerBill> m_playerPoint;

	// �^�C�}�[�\���ƃR�C���\��
	std::unique_ptr<PlayUI> m_playUI;

	// �X�^�[�g�J����
	std::unique_ptr<PlayCamera> m_camera;

	// �X�J�C�h�[��
	std::unique_ptr<PlaySky> m_sky;

private:

	// ��������(�b��)
	const int TIME_LIMIT = 60;

	// �t���[�����[�g
	const float FLAME_RATE = 60.0f;

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
	const float SHAKE_TREMOR = 2.5f;		// �U���͈�

	// ���S�G�t�F�N�g���o��ꏊ
	const float DEATH_EFFECT_LINE = DURATION_FLOOR_LINE / 1.5f;

	// �X�^�[�g�J�E���g
	const int START_COUNT = 4;

	// �J�E���g�_�E���̑��x
	const float COUNT_SPEED = 0.5f;

	// �ŏ��̃J�������o�I�_
	const float MOVE_CAMERA_SPEED = 0.05f;
	const DirectX::SimpleMath::Vector3 START_CAMERA_POS = { 0.0f,50.0f,40.0f };

	// �O�l�̃J�����̋���
	const DirectX::SimpleMath::Vector3 THIRD_DISTANCE = { 0.0f, 2.5f, 6.0f };

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	/// <param name="coins">�R�C���̐�</param>
	/// <returns>�Ȃ�</returns>
	PlayScene(const int& stageNum,const int& coins);
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
	/// �v���C���[�̍쐬
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CreatePlayer();

	/// <summary>
	/// �u���b�N�̍쐬
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CreateBlock();

	/// <summary>
	/// �X�^�[�g�̃J�E���g�_�E��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�I����Ă�����True��Ԃ�</returns>
	bool Countdown();

	/// <summary>
	/// �X�^�[�g���o
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void FirstMovement();

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

public:

	// �^�C�����~�b�g�̎擾
	const float& GetClearTime() { return m_clearTime; }
	/// �l�������R�C�����擾
	const int& GetCoinNum();
	/// �X�e�[�W�̍ő�R�C�����擾
	const int& GetMaxCoinCount();
	// �}�b�v�ԍ��̎擾
	const int& GetStageNum() { return m_stageNum; }

};

#endif // PLAYSCENE