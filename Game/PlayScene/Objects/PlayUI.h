/*
 *	@File	PlayUI.h
 *	@Brief	�v���C�V�[����UI�\���B
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYUI
#define PLAYUI

class SystemManager;
class ShowKey;
class PlayUI
{
public:

	// �J�ڔԍ�
	enum GO{ NONE, RETRY, SELECT, TITLE, EXIT, LENGTH};
private:

	// �V�X�e���|�C���^
	std::shared_ptr<SystemManager> m_system;

	// �L�[���͕\��
	std::unique_ptr<ShowKey> m_showKey;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �^�C�}�[�̍��W(1�̈�)
	DirectX::SimpleMath::Vector2 m_oneSecPos;

	// �^�C�}�[�̍��W(10�̈�)
	DirectX::SimpleMath::Vector2 m_tenSecPos;

	// �^�C�}�[�̍��W(�ŏ��̃J�E���g�_�E��)
	DirectX::SimpleMath::Vector2 m_countDownPos;

	// �X�^�[�g��������^�C�}�[
	float m_countDownEnds;

	// ���z�̈ʒu
	DirectX::SimpleMath::Vector2 m_sunPos;

	// ��������
	int m_gameTimer;

	// �����G�t�F�N�g�t���O
	bool is_effectFlag;

private:

	// ��������\���t���O
	bool is_helpFlag;

	// �J�ډ�ʃt���O
	bool is_transFlag;

	// �J�ڔԍ�
	int m_transNum;

	// �摜�̈ړ����W
	DirectX::SimpleMath::Vector2 m_moveTexPos;

	// �w���v�\���̍��W
	struct HelpPosition { DirectX::SimpleMath::Vector2 initPos; DirectX::SimpleMath::Vector2 nowPos; };
	std::map<const wchar_t*, HelpPosition> m_helpPoses;
	int m_pageNum;

	// �����W
	HelpPosition m_arrowPos;
	float m_targetArrow[GO::LENGTH];

private:

	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// �^�C�}�[�̃X�v���C�g�̃T�C�Y
	const int NUM_SIZE = 100;

	// Help�\���̉��T�C�Y
	const float HELP_WIDTH = 400.0f;

	// ���z�̉摜�̃T�C�Y
	const DirectX::SimpleMath::Vector2 SUN_SIZE = { 100.0f,100.0f };

	// �ő�b��
	const int MAX_LIMITS = 3600;

	// ��]���x
	const float ROT_SPEED = 0.005f;

	// ���z�̈ړ����x
	const float SUN_MOVE_SPEED = (FULL_SCREEN_SIZE.x - SUN_SIZE.x) / MAX_LIMITS;

	// �w���v�y�[�W��
	const int MAX_PAGE = 3;

	// �y�[�W���߂��鑬�x
	const float PAGE_SPEED = 0.1f;

	// ���̃T�C�Y
	const DirectX::SimpleMath::Vector2 ARROW_SIZE = { 256.0f,256.0f };

	// �J�E���g�_�E���\���I�����x
	const float COUNT_END_SPEED = 0.025f;

	// �X�^�[�g�̕����̏�ړ����x
	const float COUNT_MOVE_SPEED = 25.0f;

public:
	PlayUI();
	~PlayUI();

	/// <summary>
	/// �쐬����
	/// </summary>
	/// <param name="system">�V�X�e���}�l�[�W��</param>
	/// <param name="windowSize">�E�B���h�E�T�C�Y</param>
	/// <returns>�Ȃ�</returns>
	void Create(const std::shared_ptr<SystemManager>& system, const DirectX::SimpleMath::Vector2& windowSize);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="timelimit">�c�莞��</param>
	/// <returns>�Ȃ�</returns>
	void Update(const float& timelimit);

	/// <summary>
	/// �y�[�W���߂���
	/// </summary>
	/// <param name="leftArrow">��orA�L�[</param>
	/// <param name="rightArrow">�EorD�L�[</param>
	/// <returns>�Ȃ�</returns>
	void UpdatePage(const bool& leftArrow, const bool& rightArrow);

	/// <summary>
	/// �J�ڂ̍X�V
	/// </summary>
	/// <param name="upArrow">��orW�L�[</param>
	/// <param name="downArrow">��orS�L�[</param>
	/// <returns>�Ȃ�</returns>
	void UpdateTransition(const bool& upArrow, const bool& downArrow);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Render();

	/// <summary>
	/// �J�n�̃J�E���g�_�E��
	/// </summary>
	/// <param name="countDown">�J�E���g�_�E��</param>
	/// <returns>�Ȃ�</returns>
	void RenderCountDown(const float& countDown);

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize();

private:


	/// <summary>
	/// �^�C�}�[�̕`��
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	/// <returns>�Ȃ�</returns>
	void RenderTimer(DirectX::SimpleMath::Vector2 scale);

	/// <summary>
	/// ���z�̕`��
	/// </summary>
	/// <param name="scale">�摜�̊g�嗦</param>
	/// <returns>�Ȃ�</returns>
	void DrawSun(DirectX::SimpleMath::Vector2 scale);

	/// <summary>
	/// �y�[�W�̕`��
	/// </summary>
	/// <param name="scale">��ʂ̊g�嗦</param>
	/// <returns>�Ȃ�</returns>
	void DrawPage(DirectX::SimpleMath::Vector2 scale);

	/// <summary>
	/// ���W���ړ�������
	/// </summary>
	/// <param name="*pos">���W</param>
	/// <param name="end">�G���h���W</param>
	/// <returns>�Ȃ�</returns>
	void MovePositions(DirectX::SimpleMath::Vector2* pos,const DirectX::SimpleMath::Vector2& end);

	/// <summary>
	/// �o�^�������W���Ăяo��
	/// </summary>
	/// <param name="key">�o�^�L�[</param>
	/// <returns>�o�^���W</returns>
	const DirectX::SimpleMath::Vector2& GetHelpPosition(const wchar_t* key);

public:

	// �J�E���g�_�E���I������
	bool IsCountDownEnd() { return (m_countDownEnds < -0.5f); };
	// �y�[�W�ԍ��Q�b�^�[
	const int& GetPage() { return m_pageNum; }
	// �J�ڃy�[�W�Q�b�^�[
	const bool& GetTransitionPage() { return is_transFlag; }
	// �J�ڐ�Q�b�^�[
	const int& GetTransNum() { return m_transNum; }
	// �G�t�F�N�g�t���O�̃Z�b�g
	void SetEffectFlag(const bool& flag) { is_effectFlag = flag; }
	// �w���v�t���O�̃Z�b�g
	void SetHelpFlag(const bool& flag) { is_helpFlag = flag; }
};

#endif //PLAYUI
