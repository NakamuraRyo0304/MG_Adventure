/*
 *	@File	SelectScene.h
 *	@Brief  �Z���N�g�V�[���B
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTSCENE
#define	SELECTSCENE

#include "../IScene.h"

class SelectScene : public IScene
{
private:
	static const float CAMERA_ANGLE;
	std::unique_ptr<DirectX::GeometricPrimitive> m_tea;

	// ��]�p�ϐ�
	float m_angle;

	// �X�e�[�W�ԍ�
	int m_stageNum;

public:

	// �R���X�g���N�^
	SelectScene();

	// �f�X�g���N�^
	~SelectScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,DirectX::Mouse::State& mouseState) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// ��ʈˑ��̏�����
	void CreateWindowDependentResources() override;

	// �X�e�[�W�ԍ��̃A�N�Z�T
	const int& GetStageNum() { return m_stageNum; }
	void SetStageNum(const int& stageNum) { m_stageNum = stageNum; }
};

#endif // SELECTSCENE