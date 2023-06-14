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

class EditScene : public IScene
{
private:
	// �{�b�N�X�ƃX�t�B�A
	std::unique_ptr<DirectX::GeometricPrimitive> m_sphere,m_box;
	DirectX::SimpleMath::Vector3 m_spherePos;

	// �u���b�N�̔z��
	std::vector<Object> m_mapObj;

	// �}�b�v
	MapLoad m_map;

	// �����蔻��
	Collider::BoxCollider is_boxCol;
	Collider::AABBCollider m_aabbCol;

	// ���f��
	std::unique_ptr<DirectX::Model>m_grassBlockModel,m_grassBlockModel_D;	// �����f��
	std::unique_ptr<DirectX::Model>m_coinModel;								// �R�C�����f��
	std::unique_ptr<DirectX::Model>m_clowdModel;							// �_���f��
	
	// �摜�̍��W
	DirectX::SimpleMath::Vector2 m_saveTexPos;
	DirectX::SimpleMath::Vector2 m_cameraTexPos;
	DirectX::SimpleMath::Vector2 m_penTexPos;

	// �t���O
	bool is_saveFlag;
	bool is_cameraFlag;
	bool is_drawFlag;

	// ���݂̃u���b�N�X�e�[�^�X
	int m_nowState;

	// �p�X�̊i�[
	std::wstring m_filePath;

private:
	// �}�b�v�T�C�Y(Stage)
	const float	COMMON_SIZE = 1.0f;

	// �Œፂ�x
	const float COMMON_LOW = COMMON_SIZE / 2;

	// �J�����A���O��
	const float	CAMERA_ANGLE = 45.0f;

	// �摜�̒��S�ʒu
	const float	IMAGE_CENTER = 128;

	// �{�b�N�X�̍ő�l�ƍŏ��l
	const int	MIN_BOX = 1;
	const int	MAX_BOX = 15;

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

	// �摜�̕`��
	void DrawImages();

	// UI�̃N���b�N
	void ClickUserInterface(DirectX::Mouse::State& mouseState);

};

#endif // EDITSCENE