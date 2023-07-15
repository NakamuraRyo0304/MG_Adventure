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
class Fade;

// TODO: �V�[���P�F�V�[��ID�̒ǉ�
enum class SCENE : int
{
    NONE,

    TITLE,
    SELECT,
    PLAY,
    RESULT,
    EDIT
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
    std::unique_ptr<IScene> m_nowScene;

    // �t�F�[�h�I�u�W�F�N�g
    std::unique_ptr<Fade> m_fade;

    // �X�e�[�W�ԍ���ۑ�����
    int m_num;

    // �N���A���Ԃ�ۑ�����
    float m_clearTime;

    // �f�t�H���g�̃t�F�[�h���x
    const float DEFAULT_FADE_SPEED = 4.0f;
    // �v���C�V�[���̃t�F�[�h���x
    const float PLAY_FADE_SPEED = 2.5f;

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

    // �V�[���L���X�^    // �����FT = �ϊ��������^�� / scene ���̃V�[��
    template<typename T>
    T* CastSceneType(std::unique_ptr<IScene>& scene)
    {
        return dynamic_cast<T*>(scene.get());
    }
};

#endif // GAMEMAIN