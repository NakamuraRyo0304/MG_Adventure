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

class Logo;
class Stage;
class TitleSky;
class TitleUI;
class TitleScene final : public IScene
{
private:
	// �J�����A���O��
	const float CAMERA_ANGLE = 45.0f;

private:

	// UI
	std::unique_ptr<TitleUI> m_titleUI;

	// �^�C�g�����S
	std::unique_ptr<Logo> m_logo;

	// �X�e�[�W
	std::unique_ptr<Stage> m_stage;

	// �X�J�C�h�[��
	std::unique_ptr<TitleSky> m_titleSky;

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

};

#endif // TITLESCENE