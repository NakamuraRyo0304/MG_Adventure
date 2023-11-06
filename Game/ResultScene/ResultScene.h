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
class ResultUI;
class ResultScene final : public IScene
{
private:

	// �N���A�^�C��
	float m_clearTime;

	// �N���A�^�C����ۑ�����ϐ�
	float m_saveTime;

	// �Z���N�g
	int m_selectingScene;

	// ���o����
	float m_directionTime;

	// �u���b�N
	std::unique_ptr<Blocks> m_blocks;

	// UI
	std::unique_ptr<ResultUI> m_resultUI;

	// �}�b�v�ԍ�
	int m_stageNum;

	// �R�C���̖���
	int m_coinNum;

	// �ő�b��
	const int MAX_TIME = 60;

public:

	ResultScene();
	~ResultScene();

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

	/// <summary>
	/// ���f���쐬���s���֐�
	/// </summary>
	/// <param name="fm">�t�@�N�g���}�l�[�W���̃|�C���^</param>
	/// <returns>�Ȃ�</returns>
	void CreateModels(std::shared_ptr<FactoryManager> fm);

	/// <summary>
	/// �����̃A�j���[�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�I�������True</returns>
	bool AnimationValue();

	/// <summary>
	/// �V�[���؂�ւ�����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void NextScene();

public:

	// �N���A�^�C���̐ݒ�
	void SetClearTime(const float& clearTime) { m_clearTime = clearTime; }
	// �}�b�v�ԍ��̎擾
	const int& GetStageNum() { return m_stageNum; }
	// �}�b�v�ԍ��̐ݒ�
	void SetStageNum(const int& num) { m_stageNum = num; }
	// �R�C���̐ݒ�
	void SetCoinNum(const int& coinNum) { m_coinNum = coinNum; }
};

#endif // RESULTSCENE