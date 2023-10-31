/*
 *	@File	EditUI.h
 *	@Brief	UI�̕\���B
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef EDITUI
#define EDITUI

// �\���̂��g�p���邽�߂̃C���N���[�h
#include "Libraries/SystemDatas/MapLoad.h"
#include "Libraries/SystemDatas/Collider.h"
#include "Libraries/SystemDatas/DrawSprite.h"

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

	// ���݂̃X�e�[�^�X
	int m_nowState;

private:

	// �t���O
	bool is_anyHitFlag;
	bool is_saveFlag;
	bool is_openFlag;
	bool is_cameraFlag;
	bool is_toolFlag;
	bool is_backFlag;
	bool is_boxState[MAPSTATE::LENGTH];
private:

	// �摜�̍��W
	DirectX::SimpleMath::Vector2 m_toolTexPos[3];
	DirectX::SimpleMath::Vector2 m_toolButtonTexPos;
	DirectX::SimpleMath::Vector2 m_backTexPos;
	DirectX::SimpleMath::Vector2 m_imagePos[MAPSTATE::LENGTH];

	// �e�N�X�`���̐؂���p�X
	enum Cut256 { _0 = 0, _1 = 256, _2 = 512, _3 = 768, _4 = 1024 };

	// �摜�؂���ʒu
	RECT_U m_texRect[MAPSTATE::LENGTH];
	RECT_U m_modeRect[3];
	RECT_U m_saveRect[4];

	// �g��
	float m_boxHover[MAPSTATE::LENGTH];

private:

	// �摜�̒��S�ʒu
	const float	IMAGE_CENTER = 128.0f;

	// �摜�T�C�Y
	const float IMAGE_RATE = 0.55f;
	const float HALF = 0.5f;
	const float BAR_HEIGHT = 170.0f;
	const RECT_U DO_BAR_RECT = { 0L,0L,1420L,150L };

	// �����蔻��I�u�W�F�N�g
	Collider::AABBCollider m_imageHitter;

public:

	EditUI();
	~EditUI();

	void Create(const std::shared_ptr<SystemManager>& system, ID3D11Device1* device, const SimpleMath::Vector2& windowSize);
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