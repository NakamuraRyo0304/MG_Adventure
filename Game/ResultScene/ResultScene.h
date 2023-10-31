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
	std::unique_ptr<ResultUI> m_resultUI;

	// �}�b�v�ԍ�
	int m_stageNum;

	// �R�C���̖���
	int m_coinNum;

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
	void Update() override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// ��ʈˑ��̏�����
	void CreateWindowDependentResources() override;

private:

	// �V�[�����̕ϐ��������֐�
	void SetSceneValues() override;

	// ���l���o
	bool AnimationValue();

public:

	// �N���A�^�C���Z�b�^�[
	void SetClearTime(const float& clearTime) { m_clearTime = clearTime; }

	// �}�b�v�ԍ��Z�b�^�[
	void SetStageNum(const int& num) { m_stageNum = num; }

	// �R�C���Z�b�^�[
	void SetCoinNum(const int& coinNum);
};

#endif // RESULTSCENE