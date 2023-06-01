/*
 *	@File	IScene.cpps
 *	@Brief	���N���X
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "IScene.h"

// DeviceResources
DX::DeviceResources* 
IScene::GetDeviceResources()
{
	if (!m_pDR)
	{
		m_pDR = DX::DeviceResources::GetInstance();
	}
	return m_pDR;
}

// CommonStates
const std::unique_ptr<DirectX::CommonStates>& 
IScene::GetCommonStates()
{
	if (!m_commonState)
	{
		return nullptr;
	}
	return m_commonState;
}

// KeyboardStateTracker
const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& 
IScene::GetStateTrack()
{
	if (!m_keyboardStateTracker)
	{
		m_keyboardStateTracker
			= std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	}
	return m_keyboardStateTracker;
}

// DrawString
const std::unique_ptr<DrawString>& 
IScene::GetString()
{
	if (!m_drawString)
	{
		return nullptr;
	}
	return m_drawString;
}

// MouseTracker
const std::unique_ptr<DirectX::Mouse::ButtonStateTracker>& 
IScene::GetMouseTrack()
{
	if (!m_mouseStateTracker)
	{
		m_mouseStateTracker
			= std::make_unique<DirectX::Mouse::ButtonStateTracker>();
	}
	return m_mouseStateTracker;
}

// GridFloor
const std::unique_ptr<GridFloor>&
IScene::GetGridFloor()
{
	if (!m_gridFloor)
	{
		return nullptr;
	}
	return m_gridFloor;
}

// RayCast
const std::unique_ptr<RayCast>& 
IScene::GetRayCast()
{
	if (!m_rayCast)
	{
		m_rayCast = std::make_unique<RayCast>();
	}
	return m_rayCast;
}

//--------------------------------------------------------//
//���C�N���j�[�N                                          //
//--------------------------------------------------------//
void IScene::CreateUnique(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	// �f�o�C�X���\�[�X
	m_pDR		  = DX::DeviceResources::GetInstance();

	// �R�c�����_�����O
	m_commonState = std::make_unique<DirectX::CommonStates>(device);
	
	// �L�[�{�[�h
	m_keyboardStateTracker
		          = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	// �����̕`��
	m_drawString  = std::make_unique<DrawString>();

	// �J�����̏�����
	m_camera      = new Camera();

	// �}�E�X
	m_mouseStateTracker
				  = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	// �O���b�h�t���A 30 x 30
	m_gridFloor   = std::make_unique<GridFloor>(device, context, 30, 30);

	// ���C���΂�
	m_rayCast     = std::make_unique<RayCast>();

}
