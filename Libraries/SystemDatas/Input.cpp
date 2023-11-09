/*
 *	@File	Input.cpp
 *	@Brief	インプットシステム。
 *	@Date	2023-10-31
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Input.h"

// コンストラクタ
Input::Input()
	: m_keyboard{}
	, m_mouse{}
	, m_keyboardStateTracker{}
	, m_mouseStateTracker{}
{
	// デバイスリソースのインスタンスを取得
	auto _pDR = DX::DeviceResources::GetInstance();

	// キーボードの作成
	m_keyboard = std::make_unique<Keyboard>();
	m_keyboardStateTracker = std::make_unique<Keyboard::KeyboardStateTracker>();

	// マウスの作成
	m_mouse = std::make_unique<Mouse>();
	m_mouseStateTracker = std::make_unique<Mouse::ButtonStateTracker>();
	m_mouse->SetWindow(_pDR->GetHwnd());
}

// 更新処理
void Input::Update()
{
	auto _key = Keyboard::Get().GetState();
	auto _mouse = Mouse::Get().GetState();
	m_keyboardStateTracker->Update(_key);
	m_mouseStateTracker->Update(_mouse);
}

// キーボードトラッカー
const std::unique_ptr<Keyboard::KeyboardStateTracker>& Input::GetKeyTrack()
{
	if (!m_keyboardStateTracker)
	{
		m_keyboardStateTracker = std::make_unique<Keyboard::KeyboardStateTracker>();
	}
	return m_keyboardStateTracker;
}

// マウストラッカー
const std::unique_ptr<Mouse::ButtonStateTracker>& Input::GetMouseTrack()
{
	if (!m_mouseStateTracker)
	{
		m_mouseStateTracker = std::make_unique<Mouse::ButtonStateTracker>();
	}
	return m_mouseStateTracker;
}
