/*
 *	@File	Fade.h
 *	@Brief	��ʑJ�ڎ��̃t�F�[�h�����B
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef FADE
#define FADE

class Fade
{
private:
	// �t�F�[�h�C�����J�n����t���O
	bool is_fadeInFlag;

	// �t�F�[�h�A�E�g���J�n����t���O
	bool is_fadeOutFlag;

	// �t�F�[�h�I������t���O
	bool is_endFlag;

	// �t�F�[�h�̐��l0-255
	float m_fadeNum;

	// �ő�l/�ŏ��l
	const float MAX_NUM = 255.0f;
	const float MIN_NUM = 0.0f;

public:
	Fade();
	~Fade();

	// �X�V����
	void Update(float speed);

	// �t�F�[�h�C�����J�n����֐�
	void SetFadeIn();

	// �t�F�[�h�A�E�g���J�n����֐�
	void SetFadeOut();

	// �t�F�[�h�I���𔻒肷��֐�
	const bool& GetEndFlag() { return is_endFlag; }
};

#endif // FADE