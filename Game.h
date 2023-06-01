/*
 *	@file	Game.h
 *	@brief	�Q�[���̊�ՁB
 *	@date	2023-03-31
 *  @human  NakamuraRyo
 */

#pragma once
#ifndef GAME
#define GAME

#include "DeviceResources.h"
#include "StepTimer.h"
#include "Game/GameMain.h"

// �O���錾
class Scene;

class Game final : public DX::IDeviceNotify
{
public:

    static const wchar_t* TITLE;    // �Q�[���^�C�g��
    static const int SCREEN_W;      // ��ʃT�C�Y
    static const int SCREEN_H;

public:
    Game() noexcept(false);

    // ������
    void Initialize(HWND window, int width, int height);

    // �Q�[�����[�v
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // ���b�Z�[�W
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // �v���p�e�B
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Draw();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    DX::StepTimer m_timer;

    // �V�[��
    std::unique_ptr<GameMain> mGameMain;

};

#endif // GAME