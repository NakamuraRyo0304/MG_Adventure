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
class ResultScene : public IScene
{
private:
	// ���[�h
	enum { RETRY, SELECT, TITLE };

	// �^�C�}�[
	float m_timer;

	// �Z���N�g
	int m_selectNum;

	// �N���A�^�C��
	float m_clearTime;

	// �u���b�N
	std::unique_ptr<Blocks> m_blocks;

	// �}�b�v�ԍ�
	int m_stageNum;
public:

	// �R���X�g���N�^
	ResultScene();

	// �f�X�g���N�^
	~ResultScene();

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

	// �N���A�^�C���Z�b�^�[
	void SetClearTime(const float& clearTime) { m_clearTime = clearTime; }

	// �}�b�v�ԍ��Z�b�^�[
	void SetStageNum(const int& num) { m_stageNum = num; }

};

#endif // RESULTSCENE