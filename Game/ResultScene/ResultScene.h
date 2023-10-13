/*
 *	@File	ResultScene.h
 *	@Brief  ���U���g�V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef RESULTSCENE
#define	RESULTSCENE

#include "../IScene.h"

class Blocks;
class ResultUI;
class ResultScene final : public IScene
{
private:

	// �^�C�}�[
	float m_timer;

	// �N���A�^�C��
	float m_clearTime;

	// �N���A�^�C����ۑ�����ϐ�
	float m_saveTime;

	// �Z���N�g
	int m_selectingScene;

	// ���o����
	float m_directionTime;

	// �u���b�N
	std::unique_ptr<Blocks> m_blocks;

	// UI
	std::unique_ptr<ResultUI> m_userInterface;

	// �}�b�v�ԍ�
	int m_stageNum;

	// �R�C���̖���
	int m_coinNum;

	// ��ʃT�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

private:
	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// �ő�b��
	const int MAX_TIME = 60;

public:

	// �R���X�g���N�^
	ResultScene();

	// �f�X�g���N�^
	~ResultScene();

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

public:

	// �N���A�^�C���Z�b�^�[
	void SetClearTime(const float& clearTime) { m_clearTime = clearTime; }

	// �}�b�v�ԍ��Z�b�^�[
	void SetStageNum(const int& num) { m_stageNum = num; }

	// �R�C���Z�b�^�[
	void SetCoinNum(const int& coinNum);
};

#endif // RESULTSCENE