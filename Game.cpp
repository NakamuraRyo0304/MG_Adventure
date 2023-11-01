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
    // �C���X�^���X�̐���
    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
    DX::StepTimer::GetInstance();
    Input::GetInstance();

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

    // �V���O���g����StepTimer�C���X�^���X���擾
    DX::StepTimer& _stepTimer = DX::StepTimer::GetInstance();
    _stepTimer.ResetElapsedTime();
    _stepTimer.SetFixedTimeStep(true);
    _stepTimer.SetTargetElapsedSeconds(1.0 / 60);

    // �V�[���̍쐬
    m_gameMain = std::make_unique<GameMain>();
    m_gameMain->CreateWindowDependentResources(width, height);
    m_gameMain->Initialize();
}

#pragma region Frame Update
//--------------------------------------------------------//
//�Q�[�����[�v                                            //
//--------------------------------------------------------//
void Game::Tick()
{
    // �V���O���g����StepTimer�C���X�^���X���擾
    DX::StepTimer& _stepTimer = DX::StepTimer::GetInstance();

    _stepTimer.Tick([&]()
        {
            Update();
        });

    Draw();
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
void Game::Update()
{
    auto& _input = Input::GetInstance();
    _input.Update();
    m_gameMain->Update();
}
#pragma endregion

#pragma region Frame Draw

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void Game::Draw()
{
    // �V���O���g����StepTimer�C���X�^���X���擾
    DX::StepTimer& _stepTimer = DX::StepTimer::GetInstance();

    // �ŏ���Update�̑O�ɉ����������_�����O���悤�Ƃ��Ȃ�
    if (_stepTimer.GetFrameCount() == 0)
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
    m_gameMain->Draw();

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
    DX::StepTimer& _timer = DX::StepTimer::GetInstance();
    _timer.ResetElapsedTime();

    // �ŏ������畜�A��
}

void Game::OnWindowMoved()
{
    DX::DeviceResources* _pDR = DX::DeviceResources::GetInstance();

    auto r = _pDR->GetOutputSize();
    _pDR->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    DX::DeviceResources* _pDR = DX::DeviceResources::GetInstance();

    if (!_pDR->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    width = Game::SCREEN_W;
    height = Game::SCREEN_H;
}
#pragma endregion

#pragma region Direct3D Resources
// �f�o�C�X�ˑ��̃��\�[�X
void Game::CreateDeviceDependentResources()
{

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
