/*
 *	@File	UserInterface.h
 *	@Brief	UI�̕\���B
 *	@Date	2023-06-17
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef USERINTERFACE
#define USERINTERFACE

#include "Libraries/SystemDatas/Collider.h"

class SystemManager;
class UserInterface
{
private:

	// �V�F�A�|�C���^
	std::shared_ptr<SystemManager> m_systemManager;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �t���X�N���[���T�C�Y
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// �摜�̍��W
	DirectX::SimpleMath::Vector2 m_saveTexPos;
	DirectX::SimpleMath::Vector2 m_openTexPos;
	DirectX::SimpleMath::Vector2 m_cameraTexPos;
	DirectX::SimpleMath::Vector2 m_penTexPos;

	// �t���O
	bool is_saveFlag;
	bool is_openFlag;
	bool is_cameraFlag;
	bool is_drawFlag;

	// �摜�̒��S�ʒu
	const float	IMAGE_CENTER = 128.0f;

	// �摜�T�C�Y
	const float IMAGE_RATE = 0.55f;

	// �����蔻��I�u�W�F�N�g
	Collider::AABBCollider m_aabbCol;

public:

	UserInterface(const DirectX::SimpleMath::Vector2& windowSize);
	~UserInterface();

	void Initialize(std::shared_ptr<SystemManager> shareSystem,ID3D11DeviceContext1* context,ID3D11Device1* device);
	void Update(DirectX::Mouse::State& mouseState);
	void Render();
	void Finalize();

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

	// �`��t���O
	const bool& GetDrawFlag() { return is_drawFlag; }
	void SetDrawFlag(const bool flag) { is_drawFlag = flag; }
};

#endif // USERINTERFACE