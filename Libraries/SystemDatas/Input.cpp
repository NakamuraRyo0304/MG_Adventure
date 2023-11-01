/*
 *	@File	Input.cpp
 *	@Brief	�C���v�b�g�V�X�e���B
 *	@Date	2023-10-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Input.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
Input::Input()
	: m_keyboard{}
	, m_mouse{}
	, m_keyboardStateTracker{}
	, m_mouseStateTracker{}
{
	// �f�o�C�X���\�[�X�̃C���X�^���X���擾
	auto _pDR = DX::DeviceResources::GetInstance();

	// �L�[�{�[�h�̍쐬
	m_keyboard = std::make_unique<Keyboard>();
	m_keyboardStateTracker = std::make_unique<Keyboard::KeyboardStateTracker>();

	// �}�E�X�̍쐬
	m_mouse = std::make_unique<Mouse>();
	m_mouseStateTracker = std::make_unique<Mouse::ButtonStateTracker>();
	m_mouse->SetWindow(_pDR->GetHwnd());
}

/// <summary>
/// �C���v�b�g�V�X�e���̍X�V
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Input::Update()
{
	auto _key = Keyboard::Get().GetState();
	auto _mouse = Mouse::Get().GetState();
	m_keyboardStateTracker->Update(_key);
	m_mouseStateTracker->Update(_mouse);
}

// �L�[�{�[�h�g���b�J�[
const std::unique_ptr<Keyboard::KeyboardStateTracker>& Input::GetKeyTrack()
{
	if (!m_keyboardStateTracker)
	{
		m_keyboardStateTracker = std::make_unique<Keyboard::KeyboardStateTracker>();
	}
	return m_keyboardStateTracker;
}

// �}�E�X�g���b�J�[
const std::unique_ptr<Mouse::ButtonStateTracker>& Input::GetMouseTrack()
{
	if (!m_mouseStateTracker)
	{
		m_mouseStateTracker = std::make_unique<Mouse::ButtonStateTracker>();
	}
	return m_mouseStateTracker;
}
