/*
 *	@File	SystemManager.cpp
 *	@Brief	�Q�[�����쐬���邽�߂̃V�X�e���̊Ǘ��N���X�B
 *	@Date	2023-06-02
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SystemManager.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
SystemManager::SystemManager()
	: m_commonState{nullptr}
	, m_drawSprite{nullptr}
	, m_effect{nullptr}
	, m_camera{nullptr}
	, m_pDR{nullptr}
	, m_rayCast{nullptr}
#ifdef _DEBUG
	, m_gridFloor{nullptr}
	, m_drawString{nullptr}
#endif
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
SystemManager::~SystemManager()
{

}


/// <summary>
/// �f�o�C�X���\�[�X�̎擾
/// </summary>
/// <param name="��������"></param>
/// <returns>�f�o�C�X���\�[�X�̐��|�C���^</returns>
DX::DeviceResources*
SystemManager::GetDeviceResources()
{
	if (!m_pDR)
	{
		m_pDR = DX::DeviceResources::GetInstance();
	}
	return m_pDR;
}

/// <summary>
/// �R�����X�e�[�g�̎擾
/// </summary>
/// <param name="��������"></param>
/// <returns>�R�����X�e�[�g�̃��j�[�N�|�C���^</returns>
const std::unique_ptr<CommonStates>&
SystemManager::GetCommonStates()
{
	if (!m_commonState)
	{
		m_commonState = std::make_unique<CommonStates>(GetDeviceResources()->GetD3DDevice());
	}
	return m_commonState;
}

/// <summary>
/// ���C�L���X�g�̎擾
/// </summary>
/// <param name="��������"></param>
/// <returns>���C�L���X�g�̃��j�[�N�|�C���^</returns>
const std::unique_ptr<RayCast>&
SystemManager::GetRayCast()
{
	if (!m_rayCast)
	{
		throw;
	}
	return m_rayCast;
}

/// <summary>
/// �h���[�X�v���C�g�̎擾
/// </summary>
/// <param name="��������"></param>
/// <returns>�h���[�X�v���C�g�̃��j�[�N�|�C���^</returns>
const std::unique_ptr<DrawSprite>&
SystemManager::GetDrawSprite()
{
	if (!m_drawSprite)
	{
		throw;
	}
	return m_drawSprite;
}

/// <summary>
/// �J�����̎擾
/// </summary>
/// <param name="��������"></param>
/// <returns>�J�����̃��j�[�N�|�C���^</returns>
const std::unique_ptr<Camera>&
SystemManager::GetCamera()
{
	if (!m_camera)
	{
		m_camera = std::make_unique<Camera>();
	}
	return m_camera;
}

/// <summary>
/// �x�[�V�b�N�G�t�F�N�g�̎擾
/// </summary>
/// <param name="��������"></param>
/// <returns>�x�[�V�b�N�G�t�F�N�g�̃��j�[�N�|�C���^</returns>
const std::unique_ptr<BasicEffect>&
SystemManager::GetBasicEffect()
{
	if (!m_effect)
	{
		m_effect = std::make_unique<BasicEffect>(GetDeviceResources()->GetD3DDevice());
	}
	return m_effect;
}

/// <summary>
/// �T�E���h�}�l�[�W���̎擾
/// </summary>
/// <param name="��������"></param>
/// <returns>�T�E���h�}�l�[�W���̃��j�[�N�|�C���^</returns>
const std::unique_ptr<SoundManager>& SystemManager::GetSoundManager()
{
	if (!m_soundManager)
	{
		m_soundManager = std::make_unique<SoundManager>();
	}
	return m_soundManager;
}

#ifdef _DEBUG

/// <summary>
/// �����`��N���X�̎擾
/// </summary>
/// <param name="��������"></param>
/// <returns>�����`��N���X�̃��j�[�N�|�C���^</returns>
const std::unique_ptr<Debug::DrawString>&
SystemManager::GetString()
{
	if (!m_drawString)
	{
		m_pDR = DX::DeviceResources::GetInstance();
		m_drawString = std::make_unique<Debug::DrawString>(m_pDR->GetD3DDevice(), m_pDR->GetD3DDeviceContext());
	}
	return m_drawString;
}
/// <summary>
/// �h���[�t���A�̎擾
/// </summary>
/// <param name="��������"></param>
/// <returns>�h���[�t���A�̃��j�[�N�|�C���^</returns>
const std::unique_ptr<Debug::GridFloor>&
SystemManager::GetGridFloor()
{
	if (!m_gridFloor)
	{
		m_gridFloor = std::make_unique<Debug::GridFloor>(50, 50);
	}
	return m_gridFloor;
}

#endif

/// <summary>
/// �ꊇ�ŃV�X�e���̃��\�[�X���쐬
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void SystemManager::CreateUnique()
{
	// �f�o�C�X���\�[�X
	m_pDR = DX::DeviceResources::GetInstance();
	auto _device = m_pDR->GetD3DDevice();

	// �R�c�����_�����O
	m_commonState = std::make_unique<CommonStates>(_device);

	// �J�����̏�����
	m_camera = std::make_unique<Camera>();

	// ���C���΂�
	m_rayCast = std::make_unique<RayCast>();

	// �摜�̕`��
	m_drawSprite = std::make_unique<DrawSprite>();

	// �x�[�V�b�N�G�t�F�N�g
	m_effect = std::make_unique<BasicEffect>(_device);

	// �T�E���h�}�l�[�W��
	m_soundManager = std::make_unique<SoundManager>();

#ifdef _DEBUG

	auto _context = m_pDR->GetD3DDeviceContext();

	// �����̕`��
	m_drawString = std::make_unique<Debug::DrawString>(_device, _context);

	// �O���b�h�t���A 30 x 30
	m_gridFloor = std::make_unique<Debug::GridFloor>(50, 50);
#endif
}