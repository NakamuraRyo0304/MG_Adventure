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

    // 可変タイマーを使わない場合はタイマーのコメントアウトを外す

    m_timer.ResetElapsedTime();
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);

    // シーンの作成
    mGameMain = std::make_unique<GameMain>();
    mGameMain->CreateWindowDependentResources(width, height);
    mGameMain->Initialize();
}

#pragma region Frame Update
//--------------------------------------------------------//
//ゲームループ                                            //
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
//更新処理                                                //
//--------------------------------------------------------//
void Game::Update(DX::StepTimer const& timer)
{
    mGameMain->Update(timer);
}
#pragma endregion

#pragma region Frame Draw

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void Game::Draw()
{
    // 最初のUpdateの前に何かをレンダリングしようとしない
    if (m_timer.GetFrameCount() == 0)
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
    mGameMain->Draw();

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
    m_timer.ResetElapsedTime();

    // 最小化から復帰中
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
// デバイス依存のリソース
void Game::CreateDeviceDependentResources()
{
    DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

    auto device = pDR->GetD3DDevice();

    // デバイス依存のオブジェクトをここで初期化する
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
