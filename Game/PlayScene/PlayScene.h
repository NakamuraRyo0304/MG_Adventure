/*
 *	@File	PlayScene.h
 *	@Brief  �v���C�V�[���B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYSCENE
#define PLAYSCENE

#include "../IScene.h"

 // CSV�ǂݍ���
#include "../../Libraries/SystemDatas/MapLoad.h"

#include "../../Libraries/SystemDatas/Collider.h"

struct Object
{
	DirectX::SimpleMath::Vector3 position;		// ���W
	bool hitFlag;								// �����蔻��t���O
	UINT state;									// �}�b�v�̃X�e�[�^�X
};

class PlayScene : public IScene
{
private:
	// �v���C��
	DirectX::SimpleMath::Vector3 m_playerPos;
	// �{�b�N�X
	Object m_obj[MapLoad::MAP_RAW][MapLoad::MAP_COLUMN];

	// �}�b�v
	MapLoad m_map;

	// �����蔻��
	Collider::BoxCollider m_boxCol;

	// ���f��
	std::unique_ptr<DirectX::Model> m_player, m_grassBox;

	// �d��
	float m_gravity;

	// �}�b�v�T�C�Y(Stage)
	const float COMMON_SIZE = 1.0f;

	// �Œፂ�x
	const float	COMMON_LOW = COMMON_SIZE / 2;

	// �J�����A���O��
	const float	CAMERA_ANGLE = 45.0f;


public:

	// �R���X�g���N�^
	PlayScene();

	// �f�X�g���N�^
	~PlayScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState, DirectX::Mouse::State& mouseState) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// ��ʈˑ��̏�����
	void CreateWindowDependentResources() override;

	// �����蔻��
	void DoBoxCollision();

	// �f�o�b�O�\��
	void DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// �}�b�v�ǂݍ���
	void LoadMap(int num);

};

#endif // PLAYSCENE