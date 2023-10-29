/*
 *	@File	Fade.h
 *	@Brief	��ʑJ�ڎ��̃t�F�[�h�����B
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef FADE
#define FADE

class Transition;
class Fade
{
private:
	// �g�����W�V����

	std::unique_ptr<Transition> m_transition;

private:

	// �t�F�[�h�C�����J�n����t���O
	bool is_fadeInFlag;

	// �t�F�[�h�A�E�g���J�n����t���O
	bool is_fadeOutFlag;

	// �t�F�[�h���x
	float m_fadeSpeed;

	// �t�F�[�h�I������t���O
	bool is_endFlag;

	// �t�F�[�h�̐��l0-1
	float m_fadeNum;

private:

	// �ő�l/�ŏ��l
	const float MAX_NUM = 1.f;
	const float MIN_NUM = 0.f;
	const float CLAMP_NUM = 255.0f;

public:
	Fade(float speed);
	~Fade();

	// ����������
	void Initialize(DX::DeviceResources* pDR);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �t�F�[�h�C�����J�n����֐�
	void SetFadeIn();

	// �t�F�[�h�A�E�g���J�n����֐�
	void SetFadeOut();

	// �t�F�[�h�A�E�g�̒l��Ԃ�
	const float& GetMaxValue() { return MAX_NUM; }

	// �l�̃��Z�b�g��������֐�
	void Reset();

	// �t�F�[�h�I���𔻒肷��֐�
	const bool& GetEndFlag() { return is_endFlag; }

	// �t�F�[�h�̐��l�̎擾�֐�
	const float& GetFadeNum() { return m_fadeNum; }

	// �t�F�[�h���x�Z�b�^�[
	void SetFadeSpeed(float speed) { m_fadeSpeed = speed; }

};

#endif // FADE