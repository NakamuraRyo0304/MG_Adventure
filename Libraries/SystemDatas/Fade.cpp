/*
 *	@File	Fade.cpp
 *	@Brief	��ʑJ�ڎ��̃t�F�[�h�����B
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Fade.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
Fade::Fade():
	m_fadeNum{},
	is_fadeInFlag{},
	is_fadeOutFlag{},
	is_endFlag{}
{
	m_fadeNum = MAX_NUM;
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
Fade::~Fade()
{
}

//--------------------------------------------------------//
//�t�F�[�h�A�b�v�f�[�g                                    //
//--------------------------------------------------------//
void Fade::Update(float speed)
{
	// �t�F�[�h���I����Ă����珈�����Ȃ�
	if (is_endFlag) return;

	// �t�F�[�h�C��
	if (is_fadeInFlag)
	{
		m_fadeNum -= speed;
		if (m_fadeNum <= MIN_NUM)
		{
			m_fadeNum = MIN_NUM;
			is_endFlag = true;
		}
	}
	// �t�F�[�h�A�E�g
	if (is_fadeOutFlag)
	{
		m_fadeNum += speed;
		if (m_fadeNum >= MAX_NUM)
		{
			m_fadeNum = MAX_NUM;
			is_endFlag = true;
		}
	}
}

//--------------------------------------------------------//
//�t�F�[�h�C��                                            //
//--------------------------------------------------------//
void Fade::SetFadeIn()
{
	m_fadeNum = MAX_NUM;
	is_fadeInFlag = true;
	is_endFlag = false;
}

//--------------------------------------------------------//
//�t�F�[�h�A�E�g                                          //
//--------------------------------------------------------//
void Fade::SetFadeOut()
{
	m_fadeNum = MIN_NUM;
	is_fadeOutFlag = true;
	is_endFlag = false;
}
