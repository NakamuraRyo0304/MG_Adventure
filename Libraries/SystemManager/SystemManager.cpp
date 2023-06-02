/*
 *	@File	SystemManager.cpp
 *	@Brief	�Q�[�����쐬���邽�߂̃V�X�e���̊Ǘ��N���X�B
 *	@Date	2023-06-02
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SystemManager.h"

// DeviceResources
DX::DeviceResources*
SystemManager::GetDeviceResources()
{
	if (!m_pDR)
	{
		m_pDR = DX::DeviceResources::GetInstance();
	}
	return m_pDR;
}

// CommonStates
const std::unique_ptr<DirectX::CommonStates>&
SystemManager::GetCommonStates()
{
	if (!m_commonState)
	{
		return nullptr;
	}
	return m_commonState;
}

// KeyboardStateTracker
const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>&
SystemManager::GetStateTrack()
{
	if (!m_keyboardStateTracker)
	{
		return nullptr;
	}
	return m_keyboardStateTracker;
}

// DrawString
const std::unique_ptr<DrawString>&
SystemManager::GetString()
{
	if (!m_drawString)
	{
		return nullptr;
	}
	return m_drawString;
}

// MouseTracker
const std::unique_ptr<DirectX::Mouse::ButtonStateTracker>&
SystemManager::GetMouseTrack()
{
	if (!m_mouseStateTracker)
	{
		return nullptr;
	}
	return m_mouseStateTracker;
}

// GridFloor
const std::unique_ptr<GridFloor>&
SystemManager::GetGridFloor()
{
	if (!m_gridFloor)
	{
		return nullptr;
	}
	return m_gridFloor;
}

// RayCast
const std::unique_ptr<RayCast>&
SystemManager::GetRayCast()
{
	if (!m_rayCast)
	{
		return nullptr;
	}
	return m_rayCast;
}

//--------------------------------------------------------//
//�J�����Q�b�^�[                                          //
//--------------------------------------------------------//
Camera*& SystemManager::GetCamera()
{
	return m_camera;
}

//--------------------------------------------------------//
//���f�����쐬����t�@�N�g���[                            //
//--------------------------------------------------------//
// ��P�����F�f�o�C�X ��Q�����F�t�@�C���̖��O
std::unique_ptr<DirectX::DX11::Model> SystemManager::ModelFactory(
	ID3D11Device1* device,
	const wchar_t* filename)
{
	// ���f���̍쐬(�쐬���݂̂̎g���̂�)
	std::unique_ptr<DirectX::EffectFactory> fx =
		std::make_unique<DirectX::EffectFactory>(device);

	// �f�B���N�g���̐ݒ�
	fx->SetDirectory(L"Resources/Models");

	// ���f���f�[�^�̓ǂݍ���
	std::unique_ptr<DirectX::DX11::Model> model = DirectX::Model::CreateFromCMO(
		device,
		filename,
		*fx
	);

	// �쐬�������f����ԋp
	return model;
}

//--------------------------------------------------------//
//���C�N���j�[�N                                          //
//--------------------------------------------------------//
void SystemManager::CreateUnique(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// �f�o�C�X���\�[�X
	m_pDR = DX::DeviceResources::GetInstance();

	// �R�c�����_�����O
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// �L�[�{�[�h
	m_keyboardStateTracker
		= std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	// �����̕`��
	m_drawString = std::make_unique<DrawString>();

	// �J�����̏�����
	m_camera = new Camera();

	// �}�E�X
	m_mouseStateTracker
		= std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	// �O���b�h�t���A 30 x 30
	m_gridFloor = std::make_unique<GridFloor>(device, context, 30, 30);

	// ���C���΂�
	m_rayCast = std::make_unique<RayCast>();

}
