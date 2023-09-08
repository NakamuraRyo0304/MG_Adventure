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

class TitleUI;
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
	float m_cameraMoveY;
	const float MAX_HEIGHT = 10.0f;
	float m_logoMoveScale;

	// �I��ύX���̉���
	bool is_accelerateFlag;
	const float MAX_ACCELERATE_TIME = 6.0f;
	float m_accelerate;

	// ���S�̊J�n���̉��o
	float m_logoMoveY;
private:

	// UI
	std::unique_ptr<TitleUI> m_titleUI;

	// ���蔻��
	bool is_startFlag;

	// �I���t���O
	bool is_menuFlag;

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

	// ��ʉ��o
	bool FlyStartObjects();
};

#endif // TITLESCENE