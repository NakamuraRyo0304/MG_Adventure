/*
 *	@File	EditScene.h
 *	@Brief  �G�f�B�b�g�V�[���B
 *	@Date	2023-09-24
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef EDITSCENE
#define EDITSCENE

#include "../IScene.h"

 // CSV�ǂݍ���
#include "../../Libraries/SystemDatas/MapLoad.h"

// �����蔻��N���X
#include "../../Libraries/SystemDatas/Collider.h"

// ���O�Ǘ��N���X
#include "System/EditorLog.h"

// �������g
#include "System/MementoMap.h"

// �G�C���A�X
using MAPSTATE = MapLoad::BOXSTATE;

class ClearChecker;
class EditUI;
class MouseCursor;
class EditScene final : public IScene
{
private:

	// �J�[�\���ʒu
	DirectX::SimpleMath::Vector3 m_cursorPos;

	// �u���b�N�̔z��
	std::vector<Object> m_mapObj;

	// ���݂̃u���b�N�X�e�[�^�X
	int m_nowState;

	// �p�X�̊i�[
	std::wstring m_filePath;


private:

	// �}�b�v
	MapLoad m_mapLoader;

	// �����蔻��
	Collider::BoxCollider is_hitCol;

	// UI
	std::unique_ptr<EditUI> m_editUI;

	// �N���A�`�F�b�J�[
	std::unique_ptr<ClearChecker> m_checker;

	// �}�E�X�J�[�\��
	std::unique_ptr<MouseCursor> m_mouseCursor;

private:

	// UndoRedo
	EditorLog m_history;

private:
	// ���f��
	std::unique_ptr<DirectX::Model>m_grassModel;		// �����f��
	std::unique_ptr<DirectX::Model>m_noneModel;			// �����S�����f��
	std::unique_ptr<DirectX::Model>m_coinModel;			// �R�C�����f��
	std::unique_ptr<DirectX::Model>m_cloudModel;		// �_���f��
	std::unique_ptr<DirectX::Model>m_gravityModel;		// �_���Z�b�g���f��
	std::unique_ptr<DirectX::Model>m_skyDomeModel;		// �X�J�C�h�[�����f��
	std::unique_ptr<DirectX::Model>m_playerModel;		// �v���C�����f��

private:
	// �}�b�v�T�C�Y(Stage)
	const float	COMMON_SIZE = 1.0f;

	// �Œፂ�x
	const float COMMON_LOW = COMMON_SIZE / 2;

	// �J�����A���O��
	const float	CAMERA_ANGLE = 44.5f;

	// �z�C�[���̃X�p��
	const int WHEEL_SPAWN = 320;

	// �J�[�\���̈ړ����x
	const float CURSOR_MOVE_SPEED = 0.1f;

	// �J�[�\���̍Œፂ�x�ƍō����x
	const float CURSOR_MIN = -2.0f;
	const float CURSOR_MAX = static_cast<float>(MapLoad::MAP_HEIGHT + 0.5f);

	// �X�J�C�h�[���̉�]��
	const float SKY_ROTATE_RATE = 0.01f;
public:

	EditScene();
	~EditScene();

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
	/// �V�[���ϐ��������֐�
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void SetSceneValues() override;

private:

	/// <summary>
	/// �`��I�u�W�F�N�g�؂�ւ�
	/// </summary>
	/// <param name="objNum">�I�u�W�F�N�g�ԍ�</param>
	/// <param name="context">�f�o�C�X�R���e�L�X�g</param>
	/// <param name="states">�R�����X�e�[�g</param>
	/// <param name="world">���[���h�s��</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�ˉe�s��</param>
	/// <returns>�Ȃ�</returns>
	void SwitchDraw(const int& objNum, ID3D11DeviceContext* context, DirectX::CommonStates& states,
		DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	/// <summary>
	/// �}�b�v�̕ҏW
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void EditMap();

	/// <summary>
	/// �}�b�v�ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C���p�X</param>
	/// <returns>�Ȃ�</returns>
	void LoadMap(std::wstring filename);

	/// <summary>
	/// �}�b�v�����o��
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void SaveFile();
	/// <summary>
	/// �Z�[�u�\�����m�F����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	bool IsCanSave();

private:

	/// <summary>
	/// UndoRedo�����s����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void DoUndoRedo();

	/// <summary>
	/// UndoRedo�p�ɕۑ�����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void SaveModification();

	/// <summary>
	/// UndoRedo��K�p����
	/// </summary>
	/// <param name="mement"></param>
	/// <returns>�Ȃ�</returns>
	void RestoreHistory(MementoMap mement);
};

#endif // EDITSCENE