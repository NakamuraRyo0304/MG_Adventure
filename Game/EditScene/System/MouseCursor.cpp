/*
 *	@File	MouseCursor.cpp
 *	@Brief	マウスカーソルの変更。
 *	@Date	2023-08-25
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "MouseCursor.h"


// コンストラクタ
MouseCursor::MouseCursor()
{
	m_spriteBatch = std::make_unique<SpriteBatch>(DX::DeviceResources::GetInstance()->GetD3DDeviceContext());
}

// デストラクタ
MouseCursor::~MouseCursor()
{
	Finalize();
}

// 初期化処理
void MouseCursor::Initialize(const wchar_t* filename)
{
	// 画像の登録
	CreateDDSTextureFromFile(
		DX::DeviceResources::GetInstance()->GetD3DDevice(),
		filename,
		nullptr,
		m_SRV.ReleaseAndGetAddressOf()
	);
}

// マウス位置の更新
void MouseCursor::Update()
{
	auto _mouse = Mouse::Get().GetState();
	m_cursorPos = SimpleMath::Vector2{ static_cast<float>(_mouse.x),static_cast<float>(_mouse.y) };
}

// 描画処理
void MouseCursor::Render()
{
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(
		m_SRV.Get(),
		m_cursorPos,				// 表示する位置
		nullptr,					// 切り取り位置
		Colors::White,				// 描画色
		0.0f,						// 回転
		SimpleMath::Vector2{128.0f},// 画像の原点
		0.5f,						// 拡大率
		SpriteEffects_None, 0.0f	// 描画レイヤー
	);

	m_spriteBatch->End();
}

// 終了処理
void MouseCursor::Finalize()
{
	m_SRV.Reset();
}
