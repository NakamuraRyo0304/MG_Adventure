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
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="speed">�t�F�[�h�̑��x</param>
	/// <returns>�Ȃ�</returns>
	Fade(float speed);
	~Fade();

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
	/// �t�F�[�h�C�����J�n����֐�
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void SetFadeIn();

	/// <summary>
	/// �t�F�[�h�A�E�g���J�n����֐�
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void SetFadeOut();

	/// <summary>
	/// �t�F�[�h�A�E�g�̒l��Ԃ�
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�t�F�[�h�A�E�g�̒l</returns>
	const float& GetMaxValue() { return MAX_NUM; }

	/// <summary>
	/// �t�F�[�h�I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�t�F�[�h�I����True</returns>
	const bool& GetEndFlag() { return is_endFlag; }

	/// <summary>
	/// �t�F�[�h�̐��l�̎擾�֐�
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�t�F�[�h�̒l</returns>
	const float& GetFadeNum() { return m_fadeNum; }

	/// <summary>
	/// �t�F�[�h���x�Z�b�^�[
	/// </summary>
	/// <param name="speed">�t�F�[�h���x</param>
	/// <returns>�Ȃ�</returns>
	void SetFadeSpeed(float speed) { m_fadeSpeed = speed; }

private:

	/// <summary>
	/// �t�F�[�h�������s���O�ɌĂяo���֐�
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Reset();
};

#endif // FADE