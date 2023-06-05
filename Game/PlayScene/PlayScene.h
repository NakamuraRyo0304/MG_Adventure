/*
 *	@File	PlayScene.h
 *	@Brief  �v���C�V�[���B
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYSCENE
#define PLAYSCENE

#include "../IScene.h"

// CSV�ǂݍ���
#include "../../Libraries/SystemDatas/MapLoad.h"

#include "../../Libraries/SystemDatas/Collider.h"

class PlayScene : public IScene
{
private:
	// �{�b�N�X�ƃX�t�B�A
	std::unique_ptr<DirectX::GeometricPrimitive> m_sphere,m_box;
	DirectX::SimpleMath::Vector3 m_spherePos;
	DirectX::SimpleMath::Vector3 m_boxesPos[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];
	bool is_boxesHitFlag[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];

	// �}�b�v
	MapLoad m_map;
	int m_mapData[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN] = { 0 };

	// �����蔻��
	Collider::BoxCollider m_boxCol;

	// ���f��
	std::unique_ptr<DirectX::Model>m_grassBox,m_grassBoxDark;

public:

	// �R���X�g���N�^
	PlayScene();

	// �f�X�g���N�^
	~PlayScene();

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

	// �}�b�v�ǂݍ���
	void LoadMap(int num);

};

#endif // PLAYSCENE