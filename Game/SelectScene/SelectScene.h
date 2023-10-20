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
	int m_safeStages;

	// ���v�R�C����
	int m_allCoins;

	// �u���b�N�ƃX�e�[�W
	std::unique_ptr<Blocks> m_blocks[6];
	std::unique_ptr<DirectX::Model> m_stageModels[6];

	// �؂�ւ����ǂݍ��݉��o
	float m_targetY;

	// �u���b�N�̃��[�f�B���O
	std::future<void> m_loadTask;
	std::mutex m_mutex;

	// UI
	std::unique_ptr<SelectUI> m_userInterface;

	// �R�C���g�p���o
	float m_useCoins;
	int m_initCoins;
	bool is_selectEdit;

private:
	// �萔-----------------------------------------------------------------------------------

	const float MAX_FLASH = 180.0f;

	// �J�����A���O��
	const float CAMERA_ANGLE = 45.0f;
	const float CAMERA_POS_Y = 30.0f;

	// �ő�X�e�[�W��
	const int MAX_STAGE_NUM = 6;

	// �J�����̉�]���a
	const float CAMERA_RADIUS = 4.0f;

	// �X�J�C�h�[���̉�]���x
	const float SKY_ROT_SPEED = 2.0f;

	// ���グ�l
	const float UP_VALUE = 60.0f;
	// ���������x
	const float DOWN_SPEED = 0.7f;

	// �X�e�[�W���쉿�i
	const int STAGE_CREATE_PRICE = 10;

	// �J�E���g�X�s�[�h
	const float COUNT_SPEED = (10.0f / 60.0f);

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

	// �V�[�����̕ϐ��������֐�
	void SetSceneValues() override;

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
	void SetSafeStages(const int& stageNum) { m_safeStages = stageNum; }

	// ���v�R�C���A�N�Z�T(�S�̃R�C���[�g�p�ς݃R�C��)
	const int& GetAllCoins() { return m_allCoins; }
	void SetAllCoins(const int& num);
};

#endif // SELECTSCENE