/*
 *	@file	Game.cpp
 *	@brief	�Q�[���̊�ՁB
 *	@date	2023-03-31
 *  @human  ImaseHideyasu
 */

#include "pch.h"
#include "Game.h"

// TODO: �E�B���h�E�^�C�g���̐ݒ�
const wchar_t* Game::TITLE = L"SkyFall";
const int Game::SCREEN_W = 1280;
const int Game::SCREEN_H = 720;
const XMVECTORF32 SCREEN_COLOR = XMVECTORF32{ 0.133333f,0.266666f,0.411765f,1.0f };

extern void ExitGame();

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

    pDR->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

    pDR->SetWindow(window, width, height);

    pDR->CreateDeviceResources();
    CreateDeviceDependentResources();

    pDR->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // �σ^�C�}�[���g��Ȃ��ꍇ�̓^�C�}�[�̃R�����g�A�E�g���O��

    m_timer.ResetElapsedTime();
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);

    // �V�[���̍쐬
    mGameMain = std::make_unique<GameMain>();
    mGameMain->CreateWindowDependentResources(width, height);
    mGameMain->Initialize();
}

#pragma region Frame Update
//--------------------------------------------------------//
//�Q�[�����[�v                                            //
//--------------------------------------------------------//
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Draw();
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
void Game::Update(DX::StepTimer const& timer)
{
    mGameMain->Update(timer);
}
#pragma endregion

#pragma region Frame Draw

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void Game::Draw()
{
    // �ŏ���Update�̑O�ɉ����������_�����O���悤�Ƃ��Ȃ�
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

    pDR->PIXBeginEvent(L"Render");
    auto context = pDR->GetD3DDeviceContext();

    // �`��R�[�h�̒ǉ��͂�������
    context;

    pDR->PIXEndEvent();

    // �Q�[���̕`��
    mGameMain->Draw();

    // �t���[���m�F
    pDR->Present();
}

// �o�b�N�o�b�t�@���N���A����w���p�[���\�b�h
void Game::Clear()
{
    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

    pDR->PIXBeginEvent(L"Clear");

    // �r���[�̏�����
    auto context = pDR->GetD3DDeviceContext();
    auto renderTarget = pDR->GetRenderTargetView();
    auto depthStencil = pDR->GetDepthStencilView();

    // �F�̕ύX
    context->ClearRenderTargetView(renderTarget, SCREEN_COLOR);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // �r���[�|�[�g�̐ݒ�
    auto viewport = pDR->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    pDR->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
void Game::OnActivated()
{
    // �E�B���h�E���A�N�e�B�u�ł���
}

void Game::OnDeactivated()
{
    // �o�b�N�O���E���h��
}

void Game::OnSuspending()
{
    // �ŏ�����
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // �ŏ������畜�A��
}

void Game::OnWindowMoved()
{
    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

    auto r = pDR->GetOutputSize();
    pDR->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

    if (!pDR->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    width  = Game::SCREEN_W;
    height = Game::SCREEN_H;
}
#pragma endregion

#pragma region Direct3D Resources
// �f�o�C�X�ˑ��̃��\�[�X
void Game::CreateDeviceDependentResources()
{
    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

    auto device = pDR->GetD3DDevice();

    // �f�o�C�X�ˑ��̃I�u�W�F�N�g�������ŏ���������
    device;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
}

void Game::OnDeviceLost()
{
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
