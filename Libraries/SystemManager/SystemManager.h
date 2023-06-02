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

#include "../SystemDatas/DrawString.h"
#include "../SystemDatas/Camera.h"
#include "../SystemDatas/GridFloor.h"
#include "../SystemDatas/RayCast.h"

class SystemManager
{
private:
	// �f�o�C�X���\�[�V�[�Y
	DX::DeviceResources* m_pDR = nullptr;
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
public:
	// �J����
	Camera* m_camera = nullptr;

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

	// �J�����̎擾
	Camera*& GetCamera();

	// ���f���̍쐬������t�@�N�g���[
	std::unique_ptr<DirectX::DX11::Model> ModelFactory(ID3D11Device1* device, const wchar_t* filename);

public:
	// �|�C���^�̍폜
	template <typename T>
	inline void DeletePointer(T*& p)
	{
		delete p;
		p = nullptr;
	}
};

#endif // SYSTEMMANAGER