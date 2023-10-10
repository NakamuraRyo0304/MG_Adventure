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

#include "../../Libraries/SystemDatas/Collider.h"

// �G�C���A�X
using MAPSTATE = MapLoad::BOXSTATE;

class ClearChecker;
class UserInterface;
class MouseCursor;
class EditScene final : public IScene
{
private:
	// �^�C�}�[
	float m_timer;

	// �t�@�C���ǂݍ��݃��[�h
	enum { WRITE, READ };

	// �J�[�\���ʒu
	std::unique_ptr<MouseCursor> m_mouseCursor;
	DirectX::SimpleMath::Vector3 m_cursorPos;
	const float CURSOR_MIN = -2.0f;
	const float CURSOR_MAX = static_cast<float>(MapLoad::MAP_HEIGHT);

	// �u���b�N�̔z��
	std::vector<Object> m_mapObj;

	// �}�b�v
	MapLoad m_map;

	// �����蔻��
	Collider::BoxCollider is_boxCol;

	// UI
	std::unique_ptr<UserInterface> m_userInterface;
	std::shared_ptr<SystemManager> m_sharedSystem;

	// ���݂̃u���b�N�X�e�[�^�X
	int m_nowState;

	// �p�X�̊i�[
	std::wstring m_filePath;

	// �N���A�`�F�b�J�[
	std::unique_ptr<ClearChecker> m_checker;

private:
	// ���f��
	std::unique_ptr<DirectX::Model>m_grassModel;		// �����f��
	std::unique_ptr<DirectX::Model>m_noneModel;			// �����S�����f��
	std::unique_ptr<DirectX::Model>m_coinModel;			// �R�C�����f��
	std::unique_ptr<DirectX::Model>m_cloudModel;		// �_���f��
	std::unique_ptr<DirectX::Model>m_resetPtModel;		// �_���Z�b�g���f��
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
public:

	// �R���X�g���N�^
	EditScene();

	// �f�X�g���N�^
	~EditScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const float& elapsedTime,DirectX::Keyboard::State& keyState,DirectX::Mouse::State& mouseState) override;

	// �`��
	void Draw() override;

	// Switch�ȗ���
	void SwitchDraw(const int& objNum, ID3D11DeviceContext* context, DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// �I������
	void Finalize() override;

	// ��ʈˑ��̏�����
	void CreateWindowDependentResources() override;

private:

	// �V�[�����̕ϐ��������֐�
	void SetSceneValues() override;

public:

	// �}�b�v��ҏW
	void EditMap();

	// ���W�␳�֐�
	void OffsetPosition(std::vector<Object>* obj , const int& mode);

	// �}�b�v�ǂݍ���
	void LoadMap(std::wstring filename);

	// �t�@�C�����Z�[�u����
	void SaveFile();

};

#endif // EDITSCENE