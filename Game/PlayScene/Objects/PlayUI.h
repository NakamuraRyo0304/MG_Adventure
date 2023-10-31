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
class PlayUI
{
public:

	// �J�ڔԍ�
	enum GO{ NONE, RETRY, SELECT, TITLE, EXIT, LENGTH};
private:

	// �V�X�e���|�C���^
	std::shared_ptr<SystemManager> m_system;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �^�C�}�[�̍��W(1�̈�)
	DirectX::SimpleMath::Vector2 m_oneSecPos;

	// �^�C�}�[�̍��W(10�̈�)
	DirectX::SimpleMath::Vector2 m_tenSecPos;

	// �^�C�}�[�̍��W(�ŏ��̃J�E���g�_�E��)
	DirectX::SimpleMath::Vector2 m_countDownPos;

	// ��ʉ��̐������̍��W
	DirectX::SimpleMath::Vector2 m_underFontPos;

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

	// ���̕��̗���鑬�x
	const float UNDER_SPEED = 2.5f;

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


public:
	PlayUI();
	~PlayUI();

	// �쐬����
	void Create(const std::shared_ptr<SystemManager>& system, ID3D11Device1* device, const DirectX::SimpleMath::Vector2& windowSize);

	// �X�V����
	void Update(const float& timelimit);

	// �y�[�W�X�V
	void UpdatePage(const bool& leftArrow, const bool& rightArrow);

	// �J�ڍX�V
	void UpdateTransition(const bool& upArrow, const bool& downArrow);

	// �`�揈��
	void Render();

	// �J�E���g�_�E��
	void RenderCountDown(const float& countDown);

	// �I������
	void Finalize();

private:

	// �����̍X�V
	void UpdateUnderLine(DirectX::SimpleMath::Vector2 scale);

	// �^�C�}�[�̕`��
	void RenderTimer(DirectX::SimpleMath::Vector2 scale);

	// ���z�̕`��֐�
	void RenderSunny(DirectX::SimpleMath::Vector2 scale);

	// �w���v�y�[�W�̕`��
	void RenderHelpPage(DirectX::SimpleMath::Vector2 scale);

	// �w���v�摜�̍��E�ړ�
	void MovePositions(DirectX::SimpleMath::Vector2* pos,const DirectX::SimpleMath::Vector2& end);

	// �w���v�摜�̍��W���Ăяo��
	const DirectX::SimpleMath::Vector2& GetHelpPosition(const wchar_t* key);

public:

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
