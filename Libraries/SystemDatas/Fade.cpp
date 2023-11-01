/*
 *	@File	Fade.cpp
 *	@Brief	��ʑJ�ڎ��̃t�F�[�h�����B
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Transition.h"

#include "Fade.h"

// �R���X�g���N�^
Fade::Fade(float speed)
	: m_fadeNum{}			// �t�F�[�h�J�E���^
	, m_fadeSpeed{speed}	// �t�F�[�h���x
	, is_fadeInFlag{}		// �t�F�[�h�C���t���O
	, is_fadeOutFlag{}		// �t�F�[�h�A�E�g�t���O
	, is_endFlag{}			// �I������t���O
{
	m_fadeNum = MAX_NUM;
}

// �f�X�g���N�^
Fade::~Fade()
{
}

// ����������
void Fade::Initialize()
{
	// �g�����W�V�������쐬
	m_transition = std::make_unique<Transition>();
	m_transition->Create();
}

// �X�V����
void Fade::Update()
{
	// �t�F�[�h���I����Ă����珈�����Ȃ�
	if (is_endFlag) return;

	// �t�F�[�h�C��
	if (is_fadeInFlag)
	{
 		m_fadeNum -= m_fadeSpeed / CLAMP_NUM;
		if (m_fadeNum < MIN_NUM)
		{
			m_fadeNum = MIN_NUM;
		}
	}
	// �t�F�[�h�A�E�g
	if (is_fadeOutFlag)
	{
		m_fadeNum += m_fadeSpeed / CLAMP_NUM;
		if (m_fadeNum > MAX_NUM)
		{
			m_fadeNum = MAX_NUM;
		}
	}

	// �I������
	if (m_fadeNum == MAX_NUM || m_fadeNum == MIN_NUM)
	{
		is_endFlag = true;
	}
}

// �`�揈��
void Fade::Draw()
{
	// �t�F�[�h���I����Ă�����`�悵�Ȃ�
	if (is_endFlag) return;

	// �`��֐�
	m_transition->Render(m_fadeNum);
}

// �t�F�[�h�C���X�^�[�g
void Fade::SetFadeIn()
{
	Reset();
	is_endFlag = false;
	is_fadeInFlag = true;
	m_fadeNum = MAX_NUM;
}

// �t�F�[�h�A�E�g�X�^�[�g
void Fade::SetFadeOut()
{
	Reset();
	is_endFlag = false;
	is_fadeOutFlag = true;
	m_fadeNum = MIN_NUM;
}

// ���Z�b�g�֐�
void Fade::Reset()
{
	m_fadeNum = MIN_NUM;
	is_fadeInFlag = false;
	is_fadeOutFlag = false;
	is_endFlag = false;
	m_transition->CycleTextures();
}
