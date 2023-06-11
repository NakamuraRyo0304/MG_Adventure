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

struct EditObject
{
	DirectX::SimpleMath::Vector3 position;		// ���W
	bool hitFlag;								// �����蔻��t���O
	UINT state;									// �}�b�v�̃X�e�[�^�X
};

class EditScene : public IScene
{
private:
	// �{�b�N�X�ƃX�t�B�A
	std::unique_ptr<DirectX::GeometricPrimitive> m_sphere,m_box;
	DirectX::SimpleMath::Vector3 m_spherePos;
	EditObject m_obj[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];

	// �}�b�v
	MapLoad m_map;

	// �����蔻��
	Collider::BoxCollider m_boxCol;
	Collider::AABBCollider m_aabbCol;

	// ���f��
	std::unique_ptr<DirectX::Model>m_grassBlockModel,m_grassBlockModel_D,m_coinModel;
	
	// �摜�̍��W
	DirectX::SimpleMath::Vector2 m_saveTexPos;
	DirectX::SimpleMath::Vector2 m_cameraTexPos;
	DirectX::SimpleMath::Vector2 m_penTexPos;

	// �t���O
	bool is_saveFlag;
	bool is_cameraFlag;
	bool is_upFlag;

	// ���݂̃u���b�N�X�e�[�^�X
	int m_nowState;

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
	void ChangeState(UINT State);

	// �}�b�v��ҏW
	void EditMap(DirectX::Keyboard::State& keyState);

	// �}�b�v�ǂݍ���
	void LoadMap(int num);

	// �t�@�C�����Z�[�u����
	void SaveFile();

	// �摜�̕`��
	void DrawImages();

	// UI�̃N���b�N
	void ClickUserInterface(DirectX::Mouse::State& mouseState);

};

#endif // EDITSCENE