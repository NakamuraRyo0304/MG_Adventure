/*
 *	@File	SystemManager.h
 *	@Brief	�Q�[�����쐬���邽�߂̃V�X�e���̊Ǘ��N���X�B
 *	@Date	2023-06-02
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SYSTEMMANAGER
#define SYSTEMMANAGER

#include "Game/GameMain.h"

 //-------------------------------------------------------------------------------------//
 // �V�X�e���̃C���N���[�h
#include "../SystemDatas/Camera.h"
#include "../SystemDatas/RayCast.h"
#include "../SystemDatas/DrawSprite.h"
#include "../SystemDatas/SoundManager.h"
#include "../SystemDatas/GridFloor.h"
#include "../SystemDatas/DrawString.h"

//-------------------------------------------------------------------------------------//

class SystemManager
{
private:
	// �f�o�C�X���\�[�V�[�Y
	DX::DeviceResources*										m_pDR = nullptr;
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>						m_commonState;
	// ���C�L���X�g
	std::unique_ptr<RayCast>									m_rayCast;
	// �h���[�X�v���C�g
	std::unique_ptr<DrawSprite>									m_drawSprite;
	// �J����
	std::unique_ptr<Camera>										m_camera;
	// �T�E���h
	std::unique_ptr<SoundManager>								m_soundManager;
	// �����`��
	std::unique_ptr<Debug::DrawString>                          m_drawString;
	// �O���b�h��
	std::unique_ptr<Debug::GridFloor>                           m_gridFloor;


public: // �Q�b�^�[�A�Z�b�^�[�A�t�@�N�g���[
	// ���ʂ̃��C�N���j�[�N
	void CreateUnique();

	// unique_ptr�̃A�N�Z�T

	// �f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* GetDeviceResources();

	// �R�����X�e�[�g�̎擾
	const std::unique_ptr<DirectX::CommonStates>& GetCommonStates();

	// ���C�L���X�g�̎擾
	const std::unique_ptr<RayCast>& GetRayCast();

	// �h���[�X�v���C�g�̎擾
	const std::unique_ptr<DrawSprite>& GetDrawSprite();

	// �J�����̎擾
	const std::unique_ptr<Camera>& GetCamera();

	// �T�E���h�}�l�[�W���̎擾
	const std::unique_ptr<SoundManager>& GetSoundManager();

	// �h���[�X�g�����O�̎擾
	const std::unique_ptr<Debug::DrawString>& GetString();

	// �O���b�h�t���A�̎擾
	const std::unique_ptr<Debug::GridFloor>& GetGridFloor();

public:
	SystemManager();
	~SystemManager();
};

#endif // SYSTEMMANAGER