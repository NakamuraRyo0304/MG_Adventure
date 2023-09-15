/*
 *	@File	SelectScene.h
 *	@Brief  �Z���N�g�V�[���B
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTSCENE
#define	SELECTSCENE

#include <future>
#include "../IScene.h"

class Blocks;
class SelectUI;
class SelectScene final : public IScene
{
private:

	// �^�C�}�[
	float m_timer;

	// �_�ŃJ�E���^
	float m_flashCount;

	// �X�J�C�h�[��
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// �X�e�[�W�ԍ�
	int m_stageNum;
	// ���J���X�e�[�W��
	int m_noStageNum;

	// ���v�R�C����
	int m_totalCoins;

	// �u���b�N�ƃX�e�[�W
	std::unique_ptr<Blocks> m_blocks[10];
	std::unique_ptr<DirectX::Model> m_stageModels[10];

	// �؂�ւ����ǂݍ��݉��o
	float m_targetY;

	// �u���b�N�̃��[�f�B���O
	std::future<void> m_loadTask;
	std::mutex m_mutex;

	// UI
	std::unique_ptr<SelectUI> m_userInterface;

private:
	// �萔-----------------------------------------------------------------------------------

	const float MAX_FLASH = 180.0f;

	// �J�����A���O��
	const float CAMERA_ANGLE = 45.0f;
	const float CAMERA_POS_Y = 30.0f;

	// �ő�X�e�[�W��
	const int MAX_STAGE_NUM = 10;
	// �J�����̉�]���a
	const float CAMERA_RADIUS = 4.0f;

	// �X�J�C�h�[���̉�]���x
	const float SKY_ROT_SPEED = 2.0f;

	// ���グ�l
	const float UP_VALUE = 60.0f;
	// ���������x
	const float DOWN_SPEED = 0.7f;

public:

	// �R���X�g���N�^
	SelectScene();

	// �f�X�g���N�^
	~SelectScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,DirectX::Mouse::State& mouseState) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// ��ʈˑ��̏�����
	void CreateWindowDependentResources() override;

private:
	// �X�e�[�W�̃��[�f�B���O
	void CreateStages(ID3D11Device1* device);
	void CreateFirstStage(ID3D11Device1* device);

	// �X�e�[�W�ԍ��̕ύX
	void ChangeStageNumber(DirectX::Keyboard::State keyState);

	// �Z���N�g���o
	void DirectionSelectChange();

	// �e�L�X�g�̃}�g���b�N�X
	DirectX::SimpleMath::Matrix CreateTextMatrix(const float& rotValue);

public:
	// �X�e�[�W�ԍ��̃A�N�Z�T
	const int& GetStageNum() { return m_stageNum; }
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }

	// ���J���X�e�[�W�Z�b�^�[
	void SetNoStageNum(const int& stageNum) { m_noStageNum = stageNum; }

	// ���v�R�C���A�N�Z�T
	const int& GetTotalCoins() { return m_totalCoins; }
	void SetTotalCoins(const int& num);
};

#endif // SELECTSCENE