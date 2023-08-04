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
class SelectScene : public IScene
{
private:

	// �^�C�}�[
	float m_timer;

	// �_�ŃJ�E���^
	float m_flashCount;
	const float MAX_FLASH = 180.0f;

	// �J�����A���O��
	const float CAMERA_ANGLE = 45.0f;

	// �X�J�C�h�[��
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// �X�e�[�W�ԍ�
	int m_stageNum;

	// �ő�X�e�[�W��
	const int MAX_STAGE_NUM = 10;
	std::unique_ptr<Blocks> m_blocks[10];
	std::unique_ptr<DirectX::Model> m_stageModels[10];

	// �J�����̉�]���a
	const float CAMERA_RADIUS = 4.0f;

	// �X�J�C�h�[���̉�]���x
	const float SKY_ROT_SPEED = 2.0f;

	// ���グ�l
	const float UP_VALUE = 60.0f;
	// ���������x
	const float DOWN_SPEED = 0.8f;

	// �؂�ւ����ǂݍ��݉��o
	float m_changeMapMove;

	std::future<void> m_loadTask;

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

	// �X�e�[�W�̃��[�f�B���O
	void LoadStage(ID3D11Device1* device);

	// �X�e�[�W�ԍ��̃A�N�Z�T
	const int& GetStageNum() { return m_stageNum; }
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }
};

#endif // SELECTSCENE