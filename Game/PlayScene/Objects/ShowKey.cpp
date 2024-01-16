/*
 *	@File	ShowKey.cpp
 *	@Brief	キーボタンを表示。
 *	@Date	2023-01-16
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/DrawSprite.h"
#include "Libraries/SystemDatas/Input.h"
#include "ShowKey.h"

// コンストラクタ
ShowKey::ShowKey()
{
	m_show = std::make_unique<DrawSprite>();
	m_show->MakeSpriteBatch();

	// 画像の追加
	m_show->AddTextureData(L"Akey", L"Resources/Textures/PLAY_BUTTON/Akeydef.dds");
	m_show->AddTextureData(L"Wkey", L"Resources/Textures/PLAY_BUTTON/Wkeydef.dds");
	m_show->AddTextureData(L"Skey", L"Resources/Textures/PLAY_BUTTON/Skeydef.dds");
	m_show->AddTextureData(L"Dkey", L"Resources/Textures/PLAY_BUTTON/Dkeydef.dds");

	// フラグの初期化
	is_show.emplace(L"Akey", false);
	is_show.emplace(L"Wkey", false);
	is_show.emplace(L"Skey", false);
	is_show.emplace(L"Dkey", false);
}

// デストラクタ
ShowKey::~ShowKey()
{
	m_show.reset();
	is_show.clear();
}

// 更新処理
void ShowKey::Update()
{
	auto _key = Keyboard::Get().GetState();

	is_show[L"Akey"] = _key.A;
	is_show[L"Wkey"] = _key.W;
	is_show[L"Skey"] = _key.S;
	is_show[L"Dkey"] = _key.D;
}

// 描画処理
void ShowKey::Draw(const SimpleMath::Vector2 scale)
{
	if (is_show[L"Akey"])
	{
		m_show->DrawTexture(L"Akey", SimpleMath::Vector2(100.0f, 900.0f) * scale,
			{ 1,1,1,1 }, { 1,1 }, { 32,32 }, { 0,0,64,64 });
	}
	if (is_show[L"Dkey"])
	{
		m_show->DrawTexture(L"Dkey", SimpleMath::Vector2(300.0f, 900.0f) * scale,
			{ 1,1,1,1 }, { 1,1 }, { 32,32 }, { 0,0,64,64 });
	}
	if (is_show[L"Wkey"])
	{
		m_show->DrawTexture(L"Wkey", SimpleMath::Vector2(200.0f, 800.0f) * scale,
			{ 1,1,1,1 }, { 1,1 }, { 32,32 }, { 0,0,64,64 });
	}
	if (is_show[L"Skey"])
	{
		m_show->DrawTexture(L"Skey", SimpleMath::Vector2(200.0f, 900.0f) * scale,
			{ 1,1,1,1 }, { 1,1 }, { 32,32 }, { 0,0,64,64 });
	}
}