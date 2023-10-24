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

// �����蔻��p
#include <deque>

class Player;
class PlayerBill;
class PlayUI;
class PlayCamera;
class Blocks;
class ThirdPersonCamera;
class PlayScene final : public IScene
{
private:
	// �^�C�}�[
	float m_timer;

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
	std::unique_ptr<PlayUI> m_userInterFace;

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

	// �R���X�g���N�^
	PlayScene();

	// �f�X�g���N�^
	~PlayScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState, DirectX::Mouse::State& mouseState) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// ��ʈˑ��̏�����
	void CreateWindowDependentResources() override;

private:

	// �V�[�����̕ϐ��������֐�
	void SetSceneValues() override;

private:

	// �����蔻�菈��
	void Judgement();

	// �����߂�����
	void ApplyPushBack(Object& obj);

	// ���C�e�B���O�̐ݒ�
	void InitializeLighting();

	// ��̍X�V
	void UpdateSky();

	// �X�^�[�g�̃J�E���g�_�E��
	bool StartTimer();

	// �X�^�[�g���o
	void MoveStart();

	// �v���C���[�̍쐬
	void MakePlayer(ID3D11Device1* device);

public:

	// �^�C�����~�b�g�̃Q�b�^�[
	const float& GetClearTime() { return m_clearTime; }

	// �R�C���̖����Q�b�^�[
	const int& GetCoinNum();
	const int& GetMaxCoinCount();

	// �}�b�v�ԍ��̃Z�b�^�[
	const int& GetStageNum() { return m_stageNum; }
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }

	// ���v�R�C�����Z�b�^�[
	void SetAllCoinNum(const int& allCoins) { m_allCoins = allCoins; }

};

#endif // PLAYSCENE