/*
 *	@File	DepthStencil.h
 *	@Brief	深度ステンシル作成クラス。
 *	@Date	2023-11-24
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef DEPTHSTENCIL
#define DEPTHSTENCIL

class DepthStencil
{
private:
    // デバイスポインタ
    Microsoft::WRL::ComPtr<ID3D11Device>                m_device;
    // テクスチャポインタ
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_texture2D;
    // デプスステンシルビューポインタ
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_depthStencilView;
    // フォーマット
    DXGI_FORMAT                                         m_format;
    // 画面サイズ
    size_t                                              m_width;
    size_t                                              m_height;

public:
    // DXGI_FORMATを受け取るコンストラクタ
    explicit DepthStencil(DXGI_FORMAT format) noexcept;

    // ムーブコンストラクタおよびムーブ代入演算子
    DepthStencil(DepthStencil&&) = default;
    DepthStencil& operator= (DepthStencil&&) = default;

    // コピーを禁止するための削除されたコピーコンストラクタおよびコピー代入演算子
    DepthStencil(DepthStencil const&) = delete;
    DepthStencil& operator= (DepthStencil const&) = delete;

    // デバイスを設定する
    void SetDevice(_In_ ID3D11Device* device);

    // リソース（幅と高さ）のサイズを設定する
    void SizeResources(size_t width, size_t height);

    // デバイスおよび関連するリソースを解放する
    void ReleaseDevice() noexcept;

    // 指定されたRECT構造に基づいてウィンドウのサイズを設定する
    void SetWindow(const RECT& rect);

    // テクスチャを取得する
    ID3D11Texture2D* GetTexture2D() const noexcept { return m_texture2D.Get(); }
    // デプスステンシルビューを取得する
    ID3D11DepthStencilView* GetDepthStencilView() const noexcept { return m_depthStencilView.Get(); }
    // フォーマットを取得する
    DXGI_FORMAT GetFormat() const noexcept { return m_format; }
};


#endif // DEPTHSTENCIL