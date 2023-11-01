/*
 *	@File	Input.h
 *	@Brief	�C���v�b�g�V�X�e���B
 *	@Date	2023-10-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef INPUT
#define INPUT

class Input
{
private:
	// �L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	// �}�E�X
	std::unique_ptr<DirectX::Mouse> m_mouse;
	// �����ꂽ�u�Ԃ����m����
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>	m_keyboardStateTracker;
	// �����ꂽ�u�Ԃ����m����
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker>			m_mouseStateTracker;

public:

	// �C���X�^���X���쐬���擾
	static Input& GetInstance()
	{
		static Input instance;
		return instance;
	}

private:

	Input();
public:

	~Input() = default;

	// �A�b�v�f�[�g
	void Update();

	// �L�[�{�[�h�X�e�[�g�g���b�J�[�̎擾
	const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& GetKeyTrack();

	// �}�E�X�g���b�J�[�̎擾
	const std::unique_ptr<DirectX::Mouse::ButtonStateTracker>& GetMouseTrack();
};

#endif // INPUT