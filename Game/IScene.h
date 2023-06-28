/*
 *	@File	IScene.h
 *	@Brief	���N���X
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef ISCENE
#define	ISCENE

#include "GameMain.h"

// ���C�u����
#include "../Libraries/SystemManager/SystemManager.h"
#include "../Libraries/UserUtility.h"
#include "../Libraries/Factories/ModelFactory.h"

class IScene
{
private:
	// �V�X�e���}�l�[�W��
	std::shared_ptr<SystemManager> m_systemManager;

	// ���̃V�[�����w�肷��
	SCENE m_nextScene;

	// �V�[���؂�ւ�����t���O
	bool is_changeFlag;

public:
	IScene();

	virtual ~IScene() = default;

	// ������
	virtual void Initialize() = 0;

	// �X�V
	virtual void Update(const float& elapsedTime,DirectX::Keyboard::State& keyState,DirectX::Mouse::State& mouseState) = 0;

	// �`��
	virtual void Draw() = 0;

	// �I������
	virtual void Finalize() = 0;

	// ��ʈˑ��̏�����
	virtual void CreateWindowDependentResources() = 0;

public:
	// �X�e�[�W�ԍ�
	int m_stageNum = 1;
	
	// �ԍ��Q�b�^�[
	int GetStageNum() { return m_stageNum; }
	
	// �ԍ��Z�b�^�[
	void SetStageNum(int stageNum) { m_stageNum = stageNum; }

public:
	// �J�ڐ�̃V�[���ݒ�
	void GoNextScene(SCENE nextScene)
	{	
		m_nextScene = nextScene;
		is_changeFlag = true;
	}

	// �\�t�g�I��
	void ExitApp(){	PostQuitMessage(0);	}

	// ���̃V�[�����Q�b�g
	SCENE GetNextScene() { return m_nextScene; }

	// �V�X�e���}�l�[�W�����Q�b�g
	std::shared_ptr<SystemManager>& GetSystemManager() { return m_systemManager; }

	// �V�[���t���O���Q�b�g
	const bool& GetChangeSceneFlag() { return is_changeFlag; }
};

#endif // ISCENE