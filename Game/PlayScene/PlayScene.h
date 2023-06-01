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
	// �{�b�N�X�̃��f��
	std::unique_ptr<DirectX::GeometricPrimitive> m_box , m_test;
	DirectX::SimpleMath::Vector3 m_boxPos[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];

	// �}�b�v���[�_�[
	MapLoad m_map;
	// �}�b�v�p�z��
	int m_mapData[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN] = { 0 };

	DirectX::SimpleMath::Vector3 m_wantPos,m_testPos;

	Collider::SphereCollider m_sphereColl;

	float m_size;

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