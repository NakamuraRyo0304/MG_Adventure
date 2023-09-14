/*
 *	@File	ResultScene.h
 *	@Brief  ���U���g�V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef RESULTSCENE
#define	RESULTSCENE

#include "../IScene.h"

class Blocks;
class ResultScene final : public IScene
{
private:
	// ���[�h
	enum { RETRY, SELECT, TITLE };

	// �^�C�}�[
	float m_timer;

	// �Z���N�g
	int m_selectingScene;

	// �N���A�^�C��
	float m_clearTime;

	// �N���A�^�C����ۑ�����ϐ�
	float m_saveTime;

	// ���o����
	float m_directionTime;

	// �u���b�N
	std::unique_ptr<Blocks> m_blocks;

	// �}�b�v�ԍ�
	int m_stageNum;

	// �R�C���̖���
	int m_coinNum;

	// �摜���W
	DirectX::SimpleMath::Vector2 m_retryPos;
	DirectX::SimpleMath::Vector2 m_selectPos;
	DirectX::SimpleMath::Vector2 m_titlePos;
	DirectX::SimpleMath::Vector2 m_clearPos;
	DirectX::SimpleMath::Vector2 m_coinsPos;

	// �^�C�}�[�̍��W(1�̈�)
	DirectX::SimpleMath::Vector2 m_oneSecPos;
	// �^�C�}�[�̍��W(10�̈�)
	DirectX::SimpleMath::Vector2 m_tenSecPos;
	// �R�C���̍��W(1�̈�)
	DirectX::SimpleMath::Vector2 m_oneCoiPos;
	// �R�C���̍��W(10�̈�)
	DirectX::SimpleMath::Vector2 m_tenCoiPos;

	// �����x
	float m_retryAlpha;
	float m_selectAlpha;
	float m_titleAlpha;

	// �g�嗦
	float m_retryScale;
	float m_selectScale;
	float m_titleScale;

	// ��ʃT�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

private:
	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = {1920.0f,1080.0f};

	// �摜�g�嗦
	const float IMAGE_RATE = 0.55f;
	const float DEFAULT_RATE = 1.0f;
	// �摜�̒��S�ʒu
	const float	IMAGE_CENTER = 128.0f;
	const float FONT_WIDTH = 512.0f;

	// �摜�̂P�X�v���C�g�T�C�Y
	const float SPRITE_SIZE = 100.0f;

	// �t�H���g�T�C�Y
	const float DEFAULT_FONT_SCALE = 1.0f;
	const float SELECT_FONT_SCALE = 1.1f;

	// �t�H���g�����x
	const float DEFAULT_FONT_ALPHA = 0.5f;
	const float SELECT_FONT_ALPHA = 1.0f;

public:

	// �R���X�g���N�^
	ResultScene();

	// �f�X�g���N�^
	~ResultScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,DirectX::Mouse::State& mouseState) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// ��ʈˑ��̏�����
	void CreateWindowDependentResources() override;

	// �N���A�^�C���Z�b�^�[
	void SetClearTime(const float& clearTime) { m_clearTime = clearTime; }

	// �}�b�v�ԍ��Z�b�^�[
	void SetStageNum(const int& num) { m_stageNum = num; }

	// �f�B�W�b�g�̌v�Z
	void RenderDigit(int digit, const DirectX::SimpleMath::Vector2& position, DirectX::SimpleMath::Vector2 scale, int digitWidth, int digitHeight);

	// �R�C���Z�b�^�[
	void SetCoinNum(const int& coinNum) { m_coinNum = coinNum; }

private:

	// �����̕`��
	void DrawTextFonts(DirectX::SimpleMath::Vector2 imageScale);
};

#endif // RESULTSCENE