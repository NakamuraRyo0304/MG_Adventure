/*
 *	@File	SelectScene.h
 *	@Brief  �Z���N�g�V�[���B
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTSCENE
#define	SELECTSCENE

#include "../IScene.h"

class Blocks;
class FontObject;
class SelectCamera;
class SelectSky;
class SelectUI;
class SelectScene final : public IScene
{
private:

	// �X�e�[�W�ԍ�/���J���X�e�[�W��
	int m_stageNum, m_safeNum;

	// ���v�R�C����
	int m_allCoins;

	// �u���b�N�ƃX�e�[�W
	std::unique_ptr<Blocks> m_blocks[6];

	// �؂�ւ����ǂݍ��݉��o
	float m_targetY;

	// �u���b�N�̃��[�f�B���O
	std::future<void> m_loadTask;

	// UI
	std::unique_ptr<SelectUI> m_selectUI;

	// �X�e�[�W�ԍ�
	std::unique_ptr<FontObject> m_fontObject;

	// �X�J�C�h�[��
	std::unique_ptr<SelectSky> m_skyDome;

	// �J����
	std::unique_ptr<SelectCamera> m_selectCamera;

	// �R�C���g�p���o
	float m_useCoins;
	int m_initCoins;
	bool is_selectEdit;

private:
	// �萔-----------------------------------------------------------------------------------

	const float MAX_FLASH = 180.0f;

	// �ő�X�e�[�W��
	const int MAX_STAGE_NUM = 6;

	// �X�e�[�W���쉿�i
	const int STAGE_CREATE_PRICE = 10;

	// �J�E���g�X�s�[�h
	const float COUNT_SPEED = (10.0f / 60.0f);

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	/// <param name="safeNum">���J���X�e�[�W��</param>
	/// <param name="coins">�R�C���̐�</param>
	/// <returns>�Ȃ�</returns>
	SelectScene(const int& stageNum,const int& safeNum,const int& coins);
	~SelectScene();

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
	/// �X�e�[�W���f���̃��[�h
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CreateStages();
	void CreateFirstStage();

	/// <summary>
	/// �X�e�[�W�̑I��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void ChangeStageNumber();

public:
	// �X�e�[�W�ԍ��̎擾
	const int& GetStageNum() { return m_stageNum; }
	// �X�e�[�W�ԍ��̐ݒ�
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }
	// ���J���X�e�[�W�̐ݒ�
	void SetSafeStages(const int& stageNum) { m_safeNum = stageNum; }
	// ���v�R�C�����̎擾
	const int& GetAllCoins() { return m_allCoins; }
	// ���v�R�C�����̐ݒ�
	void SetAllCoins(const int& num) { m_allCoins = num; }
};

#endif // SELECTSCENE