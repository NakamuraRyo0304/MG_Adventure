/*
 *	@File	GameMain.h
 *	@Brief	�Q�[�����[�v�������N���X
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef GAMEMAIN
#define GAMEMAIN

// TODO: �V�[���P�F�V�[��ID�̒ǉ�
enum class SCENE : int
{
    NONE,

    TITLE,
    SELECT,
    PLAY,
    RESULT,
    EDIT,
    ENDGAME,
};

class IScene;
class Fade;
class GameMain
{
private: //-------------------------------------------------------------------------------------//
    // �V�X�e���n�ϐ�
    // �X�N���[���T�C�Y
    int m_screenWidth, m_screenHeight;

    // ���̃V�[���ԍ�
    SCENE m_nextScene;
    // �O�̃V�[���ԍ�
    SCENE m_prevScene;

    // ���ݏ�������V�[��
    std::unique_ptr<IScene> m_nowScene;

    // �t�F�[�h�I�u�W�F�N�g
    std::unique_ptr<Fade> m_fade;

private: //-------------------------------------------------------------------------------------//
    // �f�[�^�ۑ��n�ϐ�
    // �X�e�[�W�ԍ���ۑ�����
    int m_num;

    // ���J���X�e�[�W�ԍ���ۑ�����
    int m_safeNum;

    // �N���A���Ԃ�ۑ�����
    float m_clearTime;

    // �R�C���̖�����ۑ�����
    int m_coinNum;

    // ���܂ł̗݌v�R�C������
    int m_allCoins;
    bool is_saveOnce;

private: //-------------------------------------------------------------------------------------//
    // �t�F�[�h�֘A�ϐ�
    // �f�t�H���g�̃t�F�[�h���x
    const float DEFAULT_FADE_SPEED = 4.0f;
    // �v���C�V�[���̃t�F�[�h���x
    const float PLAY_FADE_SPEED = 2.5f;

public:
    GameMain();
    ~GameMain();

    /// <summary>
    /// ����������
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void Initialize();

    /// <summary>
    /// �X�V����
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void Update();

    /// <summary>
    /// �`�揈��
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void Draw();

    /// <summary>
    /// �I������
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void Finalize();


    /// <summary>
    /// �V�[���̐V�K�쐬
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void CreateScene();

    /// <summary>
    /// �V�[���̍폜
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void DeleteScene();

    /// <summary>
    /// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
    /// </summary>
    /// <param name="screenWidth">��ʉ���</param>
    /// <param name="screenHeight">��ʏc��</param>
    /// <returns>�Ȃ�</returns>
    void CreateWindowDependentResources(const int& screenWidth, const int& screenHeight);

    // �V�[���L���X�^    // �����FT = �ϊ��������^�� / scene ���̃V�[��
    template<typename T>
    T* CastSceneType(std::unique_ptr<IScene>& scene)
    {
        return dynamic_cast<T*>(scene.get());
    }

    /// <summary>
    /// �Z�[�u�f�[�^��ǂݍ���
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void LoadSaveData();

    /// <summary>
    /// �Z�[�u�f�[�^�������o��
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void WriteSaveData();

    /// <summary>
    /// �X�e�[�W�������
    /// </summary>
    /// <param name="��������"></param>
    /// <returns>�Ȃ�</returns>
    void OpenNewStage();
};

#endif // GAMEMAIN