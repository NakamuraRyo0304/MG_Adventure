/*
 *	@File	GameMain.h
 *	@Brief	�Q�[�����[�v�������N���X
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef GAMEMAIN
#define GAMEMAIN

 // �O���錾
class IScene;

// TODO: �V�[���P�F�V�[��ID�̒ǉ�
enum class SCENE : int
{
    NONE,

    TITLE,
    SELECT,
    PLAY,
    RESULT
};

class GameMain
{
private:
    // �X�N���[���T�C�Y
    int m_screenWidth, m_screenHeight;

    // �L�[�{�[�h
    std::unique_ptr<DirectX::Keyboard> m_keyboard;

    // �����ꂽ�u�Ԃ����m����
    std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;
    
    // �}�E�X
    std::unique_ptr<DirectX::Mouse> m_mouse;

    // �����ꂽ�u�Ԃ����m����
    std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_mouseStateTracker;

	// ���̃V�[���ԍ�
    SCENE m_nextScene;

	// ���ݏ�������V�[��
    IScene* m_pNowScene;

    // �X�e�[�W�ԍ���ۑ�����
    int m_num;

public:
    GameMain();
    ~GameMain();

    // ������
    void Initialize();

    // �X�V
    void Update(const DX::StepTimer& timer);

    // �`��
    void Draw();

    // �I��
    void Finalize();

	// �V�[���̍쐬
	void CreateScene();

	// �V�[���̍폜
	void DeleteScene();

    // �E�B���h�E�T�C�Y�ˑ��̏�����
    void CreateWindowDependentResources(const int& screenWidth, const int& screenHeight);

};

#endif // GAMEMAIN