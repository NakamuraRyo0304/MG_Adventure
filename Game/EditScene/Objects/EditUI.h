/*
 *	@File	EditUI.h
 *	@Brief	UI�̕\���B
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef EDITUI
#define EDITUI

#include "Libraries/SystemDatas/MapLoad.h"
#include "Libraries/SystemDatas/Collider.h"

// �G�C���A�X��錾
using MAPSTATE = MapLoad::BOXSTATE;

class SystemManager;
class EditUI
{
private:

	// �V�F�A�|�C���^
	std::shared_ptr<SystemManager> m_system;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// �摜�̍��W
	DirectX::SimpleMath::Vector2 m_saveTexPos;
	DirectX::SimpleMath::Vector2 m_openTexPos;
	DirectX::SimpleMath::Vector2 m_cameraTexPos;
	DirectX::SimpleMath::Vector2 m_toolButtonTexPos;
	DirectX::SimpleMath::Vector2 m_backTexPos;

	// ���݂̃X�e�[�^�X
	int m_nowState;

	// �t���O
	bool is_anyHitFlag;
	bool is_saveFlag;
	bool is_openFlag;
	bool is_cameraFlag;
	bool is_toolFlag;
	bool is_backFlag;

private:
	// �{�b�N�X�X�e�[�^�X�̔z��
	const wchar_t* m_texName[MAPSTATE::LENGTH];					// �e�N�X�`����
	bool is_boxState[MAPSTATE::LENGTH];							// ���݂̃X�e�[�g
	float m_boxHover[MAPSTATE::LENGTH];							// �z�o�[�`�F�b�N
	DirectX::SimpleMath::Vector2 m_imagePos[MAPSTATE::LENGTH];	// �e�N�X�`���̍��W

	// �摜�̒��S�ʒu
	const float	IMAGE_CENTER = 128.0f;

	// �摜�T�C�Y
	const float IMAGE_RATE = 0.55f;
	const float HALF = 0.5f;

	// �����蔻��I�u�W�F�N�g
	Collider::AABBCollider m_imageHitter;

public:

	EditUI(const DirectX::SimpleMath::Vector2& windowSize);
	~EditUI();

	void Initialize(const std::shared_ptr<SystemManager>& shareSystem,ID3D11Device1* device);
	void Update(DirectX::Mouse::State& mouseState);
	void Render();
	void Finalize();

	// �A�C�R���̕`��
	void DrawIcon(const DirectX::SimpleMath::Vector2& imageScale);

	// �X�e�[�^�X�̕ύX
	void ChangeState(DirectX::Mouse::State& mouseState);

	// �A�N�Z�T
	// �J�����t���O
	const bool& GetCameraFlag() { return is_cameraFlag; }
	void SetCameraFlag(const bool flag) { is_cameraFlag = flag; }

	// �I�[�v���t���O
	const bool& GetOpenFlag() { return is_openFlag; }
	void SetOpenFlag(const bool flag) { is_openFlag = flag; }

	// �Z�[�u�t���O
	const bool& GetSaveFlag() { return is_saveFlag; }
	void SetSaveFlag(const bool flag) { is_saveFlag = flag; }

	// �o�b�N�{�^���t���O
	const bool& GetBackSelectFlag() { return is_backFlag; }

	// ���̃X�e�[�^�X���擾
	const int& GetNowState() { return m_nowState; }
};

#endif // EDITUI