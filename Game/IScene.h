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

// ����֐�
#include "../Libraries/SystemDatas/DrawString.h"
#include "../Libraries/SystemDatas/Camera.h"
#include "../Libraries/SystemDatas/GridFloor.h"
#include "../Libraries/SystemDatas/RayCast.h"
#include "../Libraries/UserUtility.h"

class IScene
{
private:
	// �f�o�C�X���\�[�V�[�Y
	DX::DeviceResources*										m_pDR = nullptr;
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>						m_commonState;
	// �����ꂽ�u�Ԃ����m����
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>	m_keyboardStateTracker;
	// �}�E�X
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker>			m_mouseStateTracker;
	// �����`��
	std::unique_ptr<DrawString>                                 m_drawString;
	// �O���b�h��
	std::unique_ptr<GridFloor>                                  m_gridFloor;
	// ���C�L���X�g
	std::unique_ptr<RayCast>									m_rayCast;
private:
	// ���̃V�[�����w�肷��
	SCENE m_nextScene = SCENE::NONE;
public:
	// �J����
	Camera*														 m_camera = nullptr;

public:

	virtual ~IScene()
	{
		DeletePointer(m_camera);
	};

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
	int m_stageNum = 0;
	
	// �ԍ��Q�b�^�[
	int GetStageNum() { return m_stageNum; }
	
	// �ԍ��Z�b�^�[
	void SetStageNum(int stageNum) { m_stageNum = stageNum; }

public: // �Q�b�^�[�A�Z�b�^�[�A�t�@�N�g���[
	// ���ʂ̃��C�N���j�[�N
	void CreateUnique(ID3D11Device1* device, ID3D11DeviceContext1* context);

	// unique_ptr�̃A�N�Z�T

	// �f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* GetDeviceResources();

	// �R�����X�e�[�g�̎擾
	const std::unique_ptr<DirectX::CommonStates>& GetCommonStates();

	// �L�[�{�[�h�X�e�[�g�g���b�J�[�̎擾
	const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& GetStateTrack();

	// �h���[�X�g�����O�̎擾
	const std::unique_ptr<DrawString>& GetString();

	// �}�E�X�g���b�J�[�̎擾
	const std::unique_ptr<DirectX::Mouse::ButtonStateTracker>& GetMouseTrack();
	
	// �O���b�h�t���A�̎擾
	const std::unique_ptr<GridFloor>& GetGridFloor();
	
	// ���C�L���X�g�̎擾
	const std::unique_ptr<RayCast>& GetRayCast();
	
	// ���f���̍쐬������t�@�N�g���[
	std::unique_ptr<DirectX::DX11::Model> ModelFactory(ID3D11Device1* device, const wchar_t* filename);

public:
	// ���ʋ@�\�i�g�p�j
	
	// �J�ڐ�̃V�[���ݒ�
	void GoNextScene(SCENE nextScene) 
	{
		m_nextScene = nextScene; 
	};

	// �\�t�g�I��
	void ExitApp()
	{
		PostQuitMessage(0); 
	};

public:
	// ���ʋ@�\�i�����g�p�j
	
	// ���̃V�[�����Q�b�g
	SCENE GetNextScene()
	{
		return m_nextScene;
	};
	// �|�C���^�̍폜
	template <typename T>
	inline void DeletePointer(T*& p)
	{
		delete p; 
		p = nullptr;
	}
};

#endif // ISCENE