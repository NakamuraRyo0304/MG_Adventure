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

class Player;
class PlayScene : public IScene
{
private:
	// �^�C�}�[
	float m_timer;

	// �u���b�N
	std::vector<Object> m_mapObj;

	// �}�b�v
	MapLoad m_map;

	// �v���C��
	std::unique_ptr<Player> m_player;

	// �����蔻��
	Collider::BoxCollider is_boxCol;
	std::vector<Object> m_colObjList;

	// ���f��
	std::unique_ptr<DirectX::Model> m_grassModel;
	std::unique_ptr<DirectX::Model> m_coinModel;
	std::unique_ptr<DirectX::Model> m_clowdModel;
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// �R�C���J�E���^
	int m_coinCount;
	int m_maxCoins;

	// �X�J�C�h�[���̉�]���x
	const float SKY_ROT_SPEED = 0.02f;

private:
	// �}�b�v�T�C�Y(Stage)
	const float COMMON_SIZE = 0.9f;

	// �Œፂ�x
	const float	COMMON_LOW = COMMON_SIZE / 2;

	// �J�����A���O��
	const float	CAMERA_ANGLE = 44.5f;

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

	// �����߂�����
	void ApplyPushBack(Object& obj);

	// �f�o�b�O�\��
	void DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// �}�b�v�ǂݍ���
	void LoadMap(int num);

};

#endif // PLAYSCENE