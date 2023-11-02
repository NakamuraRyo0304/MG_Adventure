/*
 *	@File	SelectUI.h
 *	@Brief	�Z���N�g��UI�\���B
 *	@Date	2023-09-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTUI
#define SELECTUI

class SystemManager;
class SelectUI
{
private:
	// �V�X�e��
	std::shared_ptr<SystemManager> m_system;

	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

private:

	// �L�[����
	float m_rightAlpha;
	float m_leftAlpha;

	// ���v�R�C����
	int m_oneCoins;
	int m_tenCoins;
	int m_hanCoins;

	// �v�b�V���X�y�[�X�_�ŗp�t���O
	bool is_pushToFlag;

	// ���o�pY���㉺�ϐ�
	float m_moveY;

private:

	// �ړ����x
	const float MOVE_SPEED = 5.0f;

	// �ړ���
	const float MOVE_WIDTH = 6.0f;

	// �摜��
	const int NUM_WIDTH = 100;

	// �v�b�V���摜�̃T�C�Y
	const DirectX::SimpleMath::Vector2 PUSH_SIZE = SimpleMath::Vector2{ 1280.0f,128.0f };

public:
	SelectUI();
	~SelectUI();

	/// <summary>
	/// �쐬�֐�
	/// </summary>
	 /// <param name="system">�V�X�e���}�l�[�W��</param>
	/// <param name="windowSize">��ʃT�C�Y</param>
	/// <returns>�Ȃ�</returns>
	void Create(std::shared_ptr<SystemManager> system, const DirectX::SimpleMath::Vector2& windowSize);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="rightFlag">�E�L�[������������</param>
	/// <param name="leftFlag">���L�[������������</param>
	/// <returns>�Ȃ�</returns>
	void Update(const bool& rightFlag, const bool& leftFlag);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="fadeValue">�t�F�[�h�̒l</param>
	/// <param name="selectNum">�I�𒆂̃X�e�[�W�ԍ�</param>
	/// <param name="maxNum">�ő�̃X�e�[�W�ԍ�</param>
	/// <returns>�Ȃ�</returns>
	void Render(const float& fadeValue, const int& selectNum , const int& maxNum);

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize();

private:

	/// <summary>
	/// ����`�悷��
	/// </summary>
	/// <param name="windowRate">�E�B���h�E�䗦</param>
	/// <param name="selectNum">�I�𒆂̃X�e�[�W�ԍ�</param>
	/// <param name="maxNum">�X�e�[�W�ő吔</param>
	/// <returns>�Ȃ�</returns>
	void DrawArrow(DirectX::SimpleMath::Vector2 windowRate, const int& selectNum, const int& maxNum);

	/// <summary>
	/// �����̕`��
	/// </summary>
	/// <param name="windowRate">�E�B���h�E�䗦</param>
	/// <param name="texScale">�e�N�X�`���̃X�P�[��</param>
	/// <returns>�Ȃ�</returns>
	void DrawNumber(DirectX::SimpleMath::Vector2 windowRate, DirectX::SimpleMath::Vector2 texScale);

	/// <summary>
	/// �O�g�t���[��
	/// </summary>
	/// <param name="windowRate">�E�B���h�E�䗦</param>
	/// <returns>�Ȃ�</returns>
	void DrawFrame(DirectX::SimpleMath::Vector2 windowRate);

public:

	/// <summary>
	/// ���v�R�C�������Z�b�g���Čv�Z
	/// </summary>
	/// <param name="totalCoinNum">�R�C����</param>
	/// <returns>�Ȃ�</returns>
	void SetAllCoins(const int& totalCoinNum);

	// �R�C���g�p���̉��o
	void MoveCoins(const float& moveY) { m_moveY = moveY; };
};

#endif // SELECTUI