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
	// �J�����A���O��
	const float CAMERA_ANGLE = 45.0f;

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
	const float END_MOVE_POS = 1.0f;
	const float LOGO_CHANGE_SCALE = 1.05f;

private:

	// UI
	std::unique_ptr<TitleUI> m_titleUI;

	// ���蔻��
	bool is_startFlag;

	// �I���t���O
	bool is_menuFlag;

public:

	TitleScene();
	~TitleScene();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update() override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Draw() override;

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize() override;

	/// <summary>
	/// ��ʈˑ��A�f�o�C�X�ˑ��̏�����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CreateWindowDependentResources() override;

private:

	/// <summary>
	/// �V�[�����̕ϐ��������֐�
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void SetSceneValues() override;

private:

	/// <summary>
	/// �X�^�[�g���������Ƃ��̉��o
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>���o������True��Ԃ�</returns>
	bool FlyStartObjects();
};

#endif // TITLESCENE