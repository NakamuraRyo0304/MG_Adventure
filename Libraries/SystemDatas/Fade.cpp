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
//����������                                              //
//--------------------------------------------------------//
void Fade::Initialize(ID3D11DeviceContext1* context, ID3D11Device1* device)
{
	// �X�v���C�g�o�b�`�̏�����
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// �t�F�[�h�t���O������
	is_endFlag = false;

	// �摜�̓o�^
	CreateDDSTextureFromFile(
		device,
		L"Resources/Textures/FadeBlind.dds",		// �e�N�X�`���̃p�X
		nullptr,
		m_SRV.ReleaseAndGetAddressOf()
	);
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
		if (m_fadeNum < MIN_NUM)
		{
			m_fadeNum = MIN_NUM;
		}
	}
	// �t�F�[�h�A�E�g
	if (is_fadeOutFlag)
	{
		m_fadeNum += speed;
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

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void Fade::Draw()
{
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(
		m_SRV.Get(),												// �X�v���C�g
		SimpleMath::Vector2{0.0f},									// ���W
		nullptr,													// �؂蔲���ʒu
		SimpleMath::Vector4{1.0f,1.0f,1.0f,m_fadeNum / MAX_NUM},	// �F
		0.f,														// ��]
		SimpleMath::Vector2::Zero,									// ���S�ʒu
		1.f,														// �X�P�[��
		SpriteEffects_None,	0.f
	);

	m_spriteBatch->End();
}

//--------------------------------------------------------//
//�t�F�[�h�C��                                            //
//--------------------------------------------------------//
void Fade::SetFadeIn()
{
	is_endFlag = false;
	is_fadeInFlag = true;
	m_fadeNum = MAX_NUM;
}

//--------------------------------------------------------//
//�t�F�[�h�A�E�g                                          //
//--------------------------------------------------------//
void Fade::SetFadeOut()
{
	is_endFlag = false;
	is_fadeOutFlag = true;
	m_fadeNum = MIN_NUM;
}

//--------------------------------------------------------//
//���Z�b�g�֐�                                            //
//--------------------------------------------------------//
void Fade::Reset()
{
	m_fadeNum = 0.0f;
	is_fadeInFlag = false;
	is_fadeOutFlag = false;
	is_endFlag = false;
}
