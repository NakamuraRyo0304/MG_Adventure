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
	std::shared_ptr<SystemManager> m_system;

	// ���̃V�[�����w�肷��
	SCENE m_nextScene;

	// �X�N���[���T�C�Y
	DirectX::SimpleMath::Vector2 m_fullScreenSize;

	// �V�[���؂�ւ�����t���O
	bool is_changeFlag;

	// �V�[���؂�ւ��t�F�[�h���x
	float m_fadeSpeed;

	// �t�F�[�h�̒l
	float m_fadeValue;

private:

	// �V�[�����̕ϐ��̏������֐�
	virtual void SetSceneValues() = 0;

public:
	IScene();

	virtual ~IScene() = default;

	// ������
	virtual void Initialize() = 0;

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// �I������
	virtual void Finalize() = 0;

	// ��ʈˑ��̏�����
	virtual void CreateWindowDependentResources() = 0;

public:
	// �t�F�[�h���x�A�N�Z�T
	const float& GetFadeSpeed(){ return m_fadeSpeed; }
	void SetFadeSpeed(const float& speed) { m_fadeSpeed = speed; }

	// �t�F�[�h�l�A�N�Z�T
	const float& GetFadeValue() { return m_fadeValue; }
	void SetFadeValue(const float& value) { m_fadeValue = value; }

public:

	//-------------------------------------------------------------------------------------//
	// �Q�[���V�[�����Ŏg�p����֐�
	//-------------------------------------------------------------------------------------//

	// �V�X�e���}�l�[�W�����擾����
	inline const std::shared_ptr<SystemManager>& GetSystemManager() { return m_system; }

	// �J�ڐ�̃V�[���ݒ�
	inline void ChangeScene(const SCENE& nextScene)
	{
		m_nextScene = nextScene;
		is_changeFlag = true;
	}

	// �X�N���[���T�C�Y���擾����
	const DirectX::SimpleMath::Vector2& GetScreenSize() { return m_fullScreenSize; }

	//-------------------------------------------------------------------------------------//
	// GameMain�Ŏg�p����֐� (�ʏ�͎g�p���Ȃ�)
	//-------------------------------------------------------------------------------------//

	// �t�F�[�h�p�J�ڑj�~�֐�
	inline void StopNextScene()
	{
		m_nextScene = SCENE::NONE;
		is_changeFlag = false;
	}

	// ��ʃT�C�Y��ݒ肷��
	inline void SetScreenSize(const DirectX::SimpleMath::Vector2& size) { m_fullScreenSize = size; }

	// �\�t�g�I��
	inline void ExitApp(){	PostQuitMessage(0);	}

	// ���̃V�[�����Q�b�g
	inline const SCENE& GetNextScene() { return m_nextScene; }

	// �V�[���t���O���Q�b�g
	inline const bool& GetChangeSceneFlag() { return is_changeFlag; }
};

#endif // ISCENE