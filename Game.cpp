/*
 *	@file	Game.cpp
 *	@brief	ゲームの基盤。
 *	@date	2023-03-31
 *  @human  ImaseHideyasu
 */

#include "pch.h"
#include "Game.h"

 // TODO: ウィンドウタイトルの設定
const wchar_t* Game::TITLE = L"SkyFall";
const int Game::SCREEN_W = 1280;
const int Game::SCREEN_H = 720;
const XMVECTORF32 SCREEN_COLOR = XMVECTORF32{ 0.133333f,0.266666f,0.411765f,1.0f };

extern void ExitGame();

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    // インスタンスの生成
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

    // シングルトンのStepTimerインスタンスを取得
    DX::StepTimer& _stepTimer = DX::StepTimer::GetInstance();
    _stepTimer.ResetElapsedTime();
    _stepTimer.SetFixedTimeStep(true);
    _stepTimer.SetTargetElapsedSeconds(1.0 / 60);

    // シーンの作成
    m_gameMain = std::make_unique<GameMain>();
    m_gameMain->CreateWindowDependentResources(width, height);
    m_gameMain->Initialize();
}

#pragma region Frame Update
//--------------------------------------------------------//
//ゲームループ                                            //
//--------------------------------------------------------//
void Game::Tick()
{
    // シングルトンのStepTimerインスタンスを取得
    DX::StepTimer& _stepTimer = DX::StepTimer::GetInstance();

    _stepTimer.Tick([&]()
        {
            Update();
        });

    Draw();
}

//--------------------------------------------------------//
//更新処理                                                //
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
//描画処理                                                //
//--------------------------------------------------------//
void Game::Draw()
{
    // シングルトンのStepTimerインスタンスを取得
    DX::StepTimer& _stepTimer = DX::StepTimer::GetInstance();

    // 最初のUpdateの前に何かをレンダリングしようとしない
    if (_stepTimer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

    pDR->PIXBeginEvent(L"Render");
    auto context = pDR->GetD3DDeviceContext();

    // 描画コードの追加はここから
    context;

    pDR->PIXEndEvent();

    // ゲームの描画
    m_gameMain->Draw();

    // フレーム確認
    pDR->Present();
}

// バックバッファをクリアするヘルパーメソッド
void Game::Clear()
{
    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

    pDR->PIXBeginEvent(L"Clear");

    // ビューの初期化
    auto context = pDR->GetD3DDeviceContext();
    auto renderTarget = pDR->GetRenderTargetView();
    auto depthStencil = pDR->GetDepthStencilView();

    // 色の変更
    context->ClearRenderTargetView(renderTarget, SCREEN_COLOR);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // ビューポートの設定
    auto viewport = pDR->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    pDR->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
void Game::OnActivated()
{
    // ウィンドウがアクティブである
}

void Game::OnDeactivated()
{
    // バックグラウンド中
}

void Game::OnSuspending()
{
    // 最小化中
}

void Game::OnResuming()
{
    DX::StepTimer& _timer = DX::StepTimer::GetInstance();
    _timer.ResetElapsedTime();

    // 最小化から復帰中
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
// デバイス依存のリソース
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
