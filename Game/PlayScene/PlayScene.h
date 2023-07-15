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
class Blocks;
class PlayScene : public IScene
{
private:
	// �^�C�}�[
	float m_timer;

	// ��������
	float m_timeLimit;
	float m_returnTimeVal;

	// �}�b�v
	MapLoad m_mapLoad;
	// �}�b�v�ԍ�
	int m_mapNum;

	// �v���C��
	std::unique_ptr<Player> m_player;

	// �u���b�N
	std::unique_ptr<Blocks> m_blocks;

	// �������o�ϐ�
	float m_fallValue;

	// �����蔻��
	Collider::BoxCollider is_boxCol;
	std::vector<Object> m_colObjList;
	std::deque<int> m_prevIndex;

	// ���f��
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// �v���C���[�̖ڈ�i�r���{�[�h�j
	std::unique_ptr<PlayerBill> m_playerBill;

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
	const float JUDGE_AREA = 1.8f;

	// �J�����A���O��
	const float	CAMERA_ANGLE = 44.5f;

	// �u���b�N���󃉃C��
	const float DURATION_FLOOR_LINE = -7.5f;

	// ���S�㗎�����x
	const float FALL_SPEED = 0.002f;

	// �U���ݒ�
	const float SHAKE_DURATION = 4.0f;		// ��������
	const float SHAKE_TREMOR = 1.0f;		// �U���͈�


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

	// �����蔻�菈��
	void Judgement();
	
	// �����߂�����
	void ApplyPushBack(Object& obj);

	// �f�o�b�O�\��
	void DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// �^�C�����~�b�g�̃Q�b�^�[
	const float& GetTimeLimit() { return m_returnTimeVal; }

	// �}�b�v�ԍ��̃Z�b�^�[
	void SetStageNum(const int& stageNum) { m_mapNum = stageNum; }

};

#endif // PLAYSCENE