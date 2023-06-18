/*
 *	@File	EditScene.h
 *	@Brief  �G�f�B�b�g�V�[���B
 *	@Date	2023-03-31
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
using MapState = MapLoad::BoxState;

class UserInterface;
class EditScene : public IScene
{
private:
	// �{�b�N�X�ƃX�t�B�A
	std::unique_ptr<DirectX::GeometricPrimitive> m_sphere;
	DirectX::SimpleMath::Vector3 m_cursorPos;

	// �u���b�N�̔z��
	std::vector<Object> m_mapObj;

	// �}�b�v
	MapLoad m_map;

	// �����蔻��
	Collider::BoxCollider is_boxCol;

	// UI
	std::unique_ptr<UserInterface> m_userInterface;
	std::shared_ptr<SystemManager> m_sharedSystem;

	// ���f��
	std::unique_ptr<DirectX::Model>m_grassModel;							// �����f��
	std::unique_ptr<DirectX::Model>m_noneModel;								// ���胂�f��
	std::unique_ptr<DirectX::Model>m_coinModel;								// �R�C�����f��
	std::unique_ptr<DirectX::Model>m_clowdModel;							// �_���f��

	// ���݂̃u���b�N�X�e�[�^�X
	int m_nowState;
	float m_timer;

	// �p�X�̊i�[
	std::wstring m_filePath;

private:
	// �}�b�v�T�C�Y(Stage)
	const float	COMMON_SIZE = 1.0f;

	// �Œፂ�x
	const float COMMON_LOW = COMMON_SIZE / 2; 

	// �J�����A���O��
	const float	CAMERA_ANGLE = 44.5f;

	// �z�C�[���̃X�p��
	const int WHEEL_SPAWN = 640;
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

	// �I������
	void Finalize() override;

	// ��ʈˑ��̏�����
	void CreateWindowDependentResources() override;

	// �f�o�b�O�\��
	void DebugLog(DirectX::SimpleMath::Matrix view,DirectX::SimpleMath::Matrix proj);

	// �X�e�[�^�X��ύX����
	void ChangeState(const int& State);

	// �}�b�v��ҏW
	void EditMap();

	// ���W�␳�֐�
	void OffsetPosition_Read(std::vector<Object>* obj);
	void OffsetPosition_Write(std::vector<Object>* obj);
	
	// �}�b�v�ǂݍ���
	void LoadMap(std::wstring filename);

	// �t�@�C�����Z�[�u����
	void SaveFile();

};

#endif // EDITSCENE