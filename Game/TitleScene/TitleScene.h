/*
 *	@File	TitleScene.h
 *	@Brief	�^�C�g���V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TITLESCENE
#define	TITLESCENE

#include "../IScene.h"

class TitleScene final : public IScene
{
private:
	// �^�C�}�[
	float m_timer;

	// �^�C�g�����S���f��
	std::unique_ptr<DirectX::Model> m_titleLogoModel;
	// �^�C�g���X�e�[�W���f��
	std::unique_ptr<DirectX::Model> m_miniatureModel;
	// �X�J�C�h�[��
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// �J�������o
	float m_moveY;
	const float MAX_HEIGHT = 10.0f;
	float m_logoMoveScale;

	// ���蔻��
	bool is_startFlag;

public:

	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,DirectX::Mouse::State&) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// ��ʈˑ��̏�����
	void CreateWindowDependentResources() override;
};

#endif // TITLESCENE