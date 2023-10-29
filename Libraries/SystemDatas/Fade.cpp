/*
 *	@File	Fade.cpp
 *	@Brief	��ʑJ�ڎ��̃t�F�[�h�����B
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Transition.h"

#include "Fade.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="speed">�t�F�[�h�̑��x</param>
 /// <returns>�Ȃ�</returns>
Fade::Fade(float speed)
	: m_fadeNum{}			// �t�F�[�h�J�E���^
	, m_fadeSpeed{speed}	// �t�F�[�h���x
	, is_fadeInFlag{}		// �t�F�[�h�C���t���O
	, is_fadeOutFlag{}		// �t�F�[�h�A�E�g�t���O
	, is_endFlag{}			// �I������t���O
{
	m_fadeNum = MAX_NUM;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Fade::~Fade()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="pDR">�f�o�C�X���\�[�X</param>
/// <returns>�Ȃ�</returns>
void Fade::Initialize(DX::DeviceResources* pDR)
{
	// �g�����W�V�������쐬
	m_transition = std::make_unique<Transition>();
	m_transition->Create(pDR);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
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

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Fade::Draw()
{
	// �t�F�[�h���I����Ă�����`�悵�Ȃ�
	if (is_endFlag) return;

	// �`��֐�
	m_transition->Render(m_fadeNum);
}

/// <summary>
/// �t�F�[�h�C�����J�n����֐�
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Fade::SetFadeIn()
{
	Reset();
	is_endFlag = false;
	is_fadeInFlag = true;
	m_fadeNum = MAX_NUM;
}

/// <summary>
/// �t�F�[�h�A�E�g���J�n����֐�
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Fade::SetFadeOut()
{
	Reset();
	is_endFlag = false;
	is_fadeOutFlag = true;
	m_fadeNum = MIN_NUM;
}

/// <summary>
/// �t�F�[�h�������s���O�ɌĂяo���֐�
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Fade::Reset()
{
	m_fadeNum = MIN_NUM;
	is_fadeInFlag = false;
	is_fadeOutFlag = false;
	is_endFlag = false;
}
