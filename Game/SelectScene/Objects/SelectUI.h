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

	// �^�C�}�[
	float _timer;

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
	SelectUI(std::shared_ptr<SystemManager> system, ID3D11DeviceContext1* context, ID3D11Device1* device);
	~SelectUI();

	// ����������
	void Initialize();

	// �X�V����
	void Update(const float& timer, const bool& rightFlag, const bool& leftFlag);

	// �`�揈��
	void Render(const float& fadeValue, const int& selectNum , const int& maxNum);

	// �I������
	void Finalize();

private:

	// ���̕`��
	void DrawArrow(DirectX::SimpleMath::Vector2 windowRate, const int& selectNum, const int& maxNum);

	// �����̕`��
	void DrawNumber(DirectX::SimpleMath::Vector2 windowRate, DirectX::SimpleMath::Vector2 texScale);

	// �I����ʂɘg������
	void DrawFrame(DirectX::SimpleMath::Vector2 windowRate);

public:
	// �R�C�����Z�b�^�[
	void SetAllCoins(const int& totalCoinNum);

	// �R�C���g�p���̉��o
	void MoveCoins(const float& moveY) { m_moveY = moveY; };
};

#endif // SELECTUI