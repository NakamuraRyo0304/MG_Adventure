/*
 *	@File	Button.cpp
 *	@Brief	ボタンクラス。
 *	@Date	2023-11-02
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Button.h"

// コンストラクタ
Button::Button()
    : m_buttons{}           // ボタン
    , m_batch{}             // スプライトバッチ
{
	// スプライトバッチを作成
	m_batch = std::make_unique<SpriteBatch>(DX::DeviceResources::GetInstance()->GetD3DDeviceContext());
}

// デストラクタ
Button::~Button()
{
	m_batch.reset();
}

// ボタンの作成
void Button::CreateButton(const wchar_t* key, const wchar_t* imagePath)
{
    ButtonData buttonData;
    LoadTexture(imagePath, buttonData.texture);  // 画像を読み込んでテクスチャを設定

    // 初期パラメーターを設定（デフォルト値）
    buttonData.parameters.position = SimpleMath::Vector2(0.0f, 0.0f);
    buttonData.parameters.scale = SimpleMath::Vector2(1.0f, 1.0f);
    buttonData.parameters.rotation = 0.0f;
    buttonData.parameters.origin = SimpleMath::Vector2(0.0f, 0.0f);
    buttonData.parameters.alpha = 1.0f;

    m_buttons[key] = buttonData;  // マップにボタンデータを登録
}

// 座標をセット
void Button::SetPosition(const wchar_t* key, const SimpleMath::Vector2& pos)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        m_buttons[key].parameters.position = pos;
    }
}

// 拡大率をセット
void Button::SetScale(const wchar_t* key, const SimpleMath::Vector2& scale)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        m_buttons[key].parameters.scale = scale;
    }
}

// 回転率をセット
void Button::SetRotation(const wchar_t* key, const float& angle)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        m_buttons[key].parameters.rotation = angle;
    }
}

// 中心座標をセット
void Button::SetOrigin(const wchar_t* key, const SimpleMath::Vector2& origin)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        m_buttons[key].parameters.origin = origin;
    }
}

// 透明度をセット
void Button::SetAlpha(const wchar_t* key, const float& alpha)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        m_buttons[key].parameters.alpha = alpha;
    }
}

// 描画処理
void Button::Draw(const wchar_t* key)
{
    if (m_buttons.find(key) != m_buttons.end())
    {
        // パラメーターを適用してボタンを描画
        m_batch->Begin();
        m_batch->Draw(m_buttons[key].texture.Get(),
            m_buttons[key].parameters.position,
            nullptr,
            {1.f,1.f,1.f,m_buttons[key].parameters.alpha},
            m_buttons[key].parameters.rotation,
            m_buttons[key].parameters.origin,
            m_buttons[key].parameters.scale,
            SpriteEffects_None, 0.0f);
        m_batch->End();
    }
}

// 画像読み込み
void Button::LoadTexture(const wchar_t* imagePath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
{
    DX::DeviceResources* deviceResources = DX::DeviceResources::GetInstance();
    ID3D11Device* device = deviceResources->GetD3DDevice();
    ID3D11DeviceContext* context = deviceResources->GetD3DDeviceContext();

    // 画像ファイルを読み込んでテクスチャを生成
    CreateWICTextureFromFile(device, context, imagePath, nullptr, texture.ReleaseAndGetAddressOf());
}