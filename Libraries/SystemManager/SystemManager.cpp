/*
 *	@File	SystemManager.cpp
 *	@Brief	�Q�[�����쐬���邽�߂̃V�X�e���̊Ǘ��N���X�B
 *	@Date	2023-06-02
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SystemManager.h"

// �R���X�g���N�^
SystemManager::SystemManager()
	: m_commonState{nullptr}
	, m_drawSprite{nullptr}
	, m_camera{nullptr}
	, m_pDR{nullptr}
	, m_rayCast{nullptr}
	, m_gridFloor{nullptr}
	, m_drawString{nullptr}
{
}

// �f�X�g���N�^
SystemManager::~SystemManager()
{

}


// �f�o�C�X���\�[�X�̎擾
DX::DeviceResources* SystemManager::GetDeviceResources()
{
	if (not m_pDR)
	{
		m_pDR = DX::DeviceResources::GetInstance();
	}
	return m_pDR;
}

// �R�����X�e�[�g�̎擾
const std::unique_ptr<CommonStates>& SystemManager::GetCommonStates()
{
	if (not m_commonState)
	{
		m_commonState = std::make_unique<CommonStates>(GetDeviceResources()->GetD3DDevice());
	}
	return m_commonState;
}

// ���C�L���X�g�̎擾
const std::unique_ptr<RayCast>& SystemManager::GetRayCast()
{
	if (not m_rayCast)
	{
		m_rayCast = std::make_unique<RayCast>();
	}
	return m_rayCast;
}

// �h���[�X�v���C�g�̎擾
const std::unique_ptr<DrawSprite>& SystemManager::GetDrawSprite()
{
	if (not m_drawSprite)
	{
		m_drawSprite = std::make_unique<DrawSprite>();
		m_drawSprite->MakeSpriteBatch();
	}
	return m_drawSprite;
}

// �J�����̎擾
const std::unique_ptr<Camera>& SystemManager::GetCamera()
{
	if (not m_camera)
	{
		m_camera = std::make_unique<Camera>();
	}
	return m_camera;
}

// �T�E���h�}�l�[�W���̎擾
const std::unique_ptr<SoundManager>& SystemManager::GetSoundManager()
{
	if (not m_soundManager)
	{
		m_soundManager = std::make_unique<SoundManager>();
	}
	return m_soundManager;
}

// �h���[�X�g�����O�̎擾
const std::unique_ptr<Debug::DrawString>& SystemManager::GetString()
{
	if (not m_drawString)
	{
		m_pDR = DX::DeviceResources::GetInstance();
		m_drawString = std::make_unique<Debug::DrawString>(m_pDR->GetD3DDevice(), m_pDR->GetD3DDeviceContext());
	}
	return m_drawString;
}

// �O���b�h�t���A�̎擾
const std::unique_ptr<Debug::GridFloor>& SystemManager::GetGridFloor()
{
	if (not m_gridFloor)
	{
		m_gridFloor = std::make_unique<Debug::GridFloor>(50, 50);
	}
	return m_gridFloor;
}

// �S�Ẵ��\�[�X���쐬
void SystemManager::CreateSystem()
{
	// �f�o�C�X���\�[�X
	m_pDR = DX::DeviceResources::GetInstance();
	auto _device = m_pDR->GetD3DDevice();
	auto _context = m_pDR->GetD3DDeviceContext();

	// �R�c�����_�����O
	m_commonState = std::make_unique<CommonStates>(_device);

	// �J�����̏�����
	m_camera = std::make_unique<Camera>();

	// ���C���΂�
	m_rayCast = std::make_unique<RayCast>();

	// �摜�̕`��
	m_drawSprite = std::make_unique<DrawSprite>();

	// �T�E���h�}�l�[�W��
	m_soundManager = std::make_unique<SoundManager>();

	// �����̕`��
	m_drawString = std::make_unique<Debug::DrawString>(_device, _context);

	// �O���b�h�t���A
	m_gridFloor = std::make_unique<Debug::GridFloor>(50, 50);
}