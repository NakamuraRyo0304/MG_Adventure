/*
 *	@File	DepthStencil.cpp
 *	@Brief	深度ステンシル作成クラス。
 *	@Date	2023-11-24
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "DepthStencil.h"

//==============================================================================
// コンストラクタ
//==============================================================================
DepthStencil::DepthStencil(DXGI_FORMAT format) noexcept
    : m_format(format)      // フォーマット
    , m_width(0)            // 横幅
    , m_height(0)           // 縦幅
{
}

//==============================================================================
// デバイスを設定する
//==============================================================================
void DepthStencil::SetDevice(_In_ ID3D11Device* device)
{
    // 設定されていたら処理しない
    if (device == m_device.Get()) return;

    // 一度クリアする
    if (m_device) ReleaseDevice();

    // 指定されたフォーマットがデバイスでサポートされているか確認
    UINT formatSupport = 0;
    if (FAILED(device->CheckFormatSupport(m_format, &formatSupport)))
    {
        throw std::runtime_error("CheckFormatSupport");
    }

    // デバイスが必要なフォーマット機能をサポートしているか確認
    constexpr UINT32 required = D3D11_FORMAT_SUPPORT_TEXTURE2D | D3D11_FORMAT_SUPPORT_DEPTH_STENCIL;
    if ((formatSupport & required) != required)
    {
        throw std::runtime_error("DepthStencil");
    }
    m_device = device;
}

//==============================================================================
// リソース（幅と高さ）のサイズを設定する
//==============================================================================
void DepthStencil::SizeResources(size_t width, size_t height)
{
    if (width == m_width && height == m_height) return;

    if (m_width > UINT32_MAX || m_height > UINT32_MAX)
    {
        throw std::out_of_range("無効な幅/高さ");
    }

    if (not m_device) return;

    m_width = m_height = 0;

    // デプスステンシルテクスチャとビューを作成する
    CD3D11_TEXTURE2D_DESC depthStencilDesc(m_format, static_cast<UINT>(width), static_cast<UINT>(height),
        1, 1, D3D11_BIND_DEPTH_STENCIL);

    DX::ThrowIfFailed(m_device->CreateTexture2D(
        &depthStencilDesc,
        nullptr,
        m_texture2D.ReleaseAndGetAddressOf()
    ));

    DX::ThrowIfFailed(m_device->CreateDepthStencilView(
        m_texture2D.Get(),
        nullptr,
        m_depthStencilView.ReleaseAndGetAddressOf()
    ));

    m_width = width;
    m_height = height;
}

//==============================================================================
// Direct3Dデバイスおよび関連するリソースを解放する
//==============================================================================
void DepthStencil::ReleaseDevice() noexcept
{
    m_depthStencilView.Reset();
    m_texture2D.Reset();
    m_device.Reset();
    m_width = m_height = 0;
}

//==============================================================================
// 指定されたRECT構造に基づいてウィンドウのサイズを設定する
//==============================================================================
void DepthStencil::SetWindow(const RECT& output)
{
    // ピクセル単位でレンダリングターゲットサイズを決定する
    auto const width = size_t(std::max<LONG>(output.right - output.left, 1));
    auto const height = size_t(std::max<LONG>(output.bottom - output.top, 1));

    // ウィンドウサイズに基づいてリソースのサイズを設定する
    SizeResources(width, height);
}