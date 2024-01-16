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
	m_show->AddTextureData(L"Move", L"Resources/Textures/PLAY_BUTTON/BackArrow.dds");
	m_show->AddTextureData(L"Camera", L"Resources/Textures/PLAY_BUTTON/BackCamera.dds");

	m_show->AddTextureData(L"Wkey", L"Resources/Textures/PLAY_BUTTON/Wkeydef.dds");
	m_show->AddTextureData(L"Akey", L"Resources/Textures/PLAY_BUTTON/Akeydef.dds");
	m_show->AddTextureData(L"Skey", L"Resources/Textures/PLAY_BUTTON/Skeydef.dds");
	m_show->AddTextureData(L"Dkey", L"Resources/Textures/PLAY_BUTTON/Dkeydef.dds");
	m_show->AddTextureData(L"Hkey", L"Resources/Textures/PLAY_BUTTON/Hkeydef.dds");
	m_show->AddTextureData(L"WkeyR", L"Resources/Textures/PLAY_BUTTON/Wkeyred.dds");
	m_show->AddTextureData(L"AkeyR", L"Resources/Textures/PLAY_BUTTON/Akeyred.dds");
	m_show->AddTextureData(L"SkeyR", L"Resources/Textures/PLAY_BUTTON/Skeyred.dds");
	m_show->AddTextureData(L"DkeyR", L"Resources/Textures/PLAY_BUTTON/Dkeyred.dds");
	m_show->AddTextureData(L"ArD", L"Resources/Textures/PLAY_BUTTON/ArDown.dds");
	m_show->AddTextureData(L"ArU", L"Resources/Textures/PLAY_BUTTON/ArUp.dds");
	m_show->AddTextureData(L"ArR", L"Resources/Textures/PLAY_BUTTON/ArRight.dds");
	m_show->AddTextureData(L"ArL", L"Resources/Textures/PLAY_BUTTON/ArLeft.dds");
	m_show->AddTextureData(L"ArDR", L"Resources/Textures/PLAY_BUTTON/ArDownR.dds");
	m_show->AddTextureData(L"ArUR", L"Resources/Textures/PLAY_BUTTON/ArUpR.dds");
	m_show->AddTextureData(L"ArRR", L"Resources/Textures/PLAY_BUTTON/ArRightR.dds");
	m_show->AddTextureData(L"ArLR", L"Resources/Textures/PLAY_BUTTON/ArLeftR.dds");

	// フラグの初期化
	is_show.emplace(L"Wkey", false);
	is_show.emplace(L"Akey", false);
	is_show.emplace(L"Skey", false);
	is_show.emplace(L"Dkey", false);
	is_show.emplace(L"Upkey", false);
	is_show.emplace(L"Downkey", false);
	is_show.emplace(L"Rightkey", false);
	is_show.emplace(L"Leftkey", false);
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

	is_show[L"Wkey"] = _key.W;
	is_show[L"Akey"] = _key.A;
	is_show[L"Skey"] = _key.S;
	is_show[L"Dkey"] = _key.D;
	is_show[L"Upkey"] = _key.Up;
	is_show[L"Downkey"] = _key.Down;
	is_show[L"Rightkey"] = _key.Right;
	is_show[L"Leftkey"] = _key.Left;
}

// 描画処理
void ShowKey::Draw(const SimpleMath::Vector2 scale)
{
	// 移動の背景
	m_show->DrawTexture(L"Move", SimpleMath::Vector2(200.0f, 900.0f) * scale,
		{ 1,1,1,1 }, SimpleMath::Vector2::One * 2 * scale, { 96,64 }, { 0,0,192,128 });

	// カメラの背景
	m_show->DrawTexture(L"Camera", SimpleMath::Vector2(1720.0f, 900.0f) * scale,
		{ 1,1,1,1 }, SimpleMath::Vector2::One * 2 * scale, { 96,64 }, { 0,0,192,128 });

	// Hキー　ヘルプボタン
	m_show->DrawTexture(L"Hkey", SimpleMath::Vector2(1650.0f, 50.0f) * scale,
		{ 1,1,1,1 }, SimpleMath::Vector2::One * scale, { 32,32 }, { 0,0,64,64 });

	// WASDキーの表示　押していたら赤色に表示する
	m_show->DrawTexture((is_show[L"Wkey"] ? L"WkeyR" : L"Wkey"),
		SimpleMath::Vector2(200.0f, 850.0f) * scale, { 1,1,1,1 }, SimpleMath::Vector2::One * scale,
		{ 32,32 }, { 0,0,64,64 });

	m_show->DrawTexture((is_show[L"Akey"] ? L"AkeyR" : L"Akey"),
		SimpleMath::Vector2(100.0f, 950.0f) * scale, { 1,1,1,1 }, SimpleMath::Vector2::One * scale,
		{ 32,32 }, { 0,0,64,64 });

	m_show->DrawTexture((is_show[L"Skey"] ? L"SkeyR" : L"Skey"),
		SimpleMath::Vector2(200.0f, 950.0f) * scale, { 1,1,1,1 }, SimpleMath::Vector2::One * scale,
		{ 32,32 }, { 0,0,64,64 });

	m_show->DrawTexture((is_show[L"Dkey"] ? L"DkeyR" : L"Dkey"),
		SimpleMath::Vector2(300.0f, 950.0f) * scale, { 1,1,1,1 }, SimpleMath::Vector2::One * scale,
		{ 32,32 }, { 0,0,64,64 });

	// 十字キーの表示　押していたら赤色に表示する
	m_show->DrawTexture((is_show[L"Upkey"] ? L"ArUR" : L"ArU"),
		SimpleMath::Vector2(1720.0f, 850.0f) * scale, { 1,1,1,1 }, SimpleMath::Vector2::One * scale,
		{ 32,32 }, { 0,0,64,64 });

	m_show->DrawTexture((is_show[L"Downkey"] ? L"ArDR" : L"ArD"),
		SimpleMath::Vector2(1720.0f, 950.0f) * scale, { 1,1,1,1 }, SimpleMath::Vector2::One * scale,
		{ 32,32 }, { 0,0,64,64 });

	m_show->DrawTexture((is_show[L"Leftkey"] ? L"ArLR" : L"ArL"),
		SimpleMath::Vector2(1620.0f, 950.0f) * scale, { 1,1,1,1 }, SimpleMath::Vector2::One * scale,
		{ 32,32 }, { 0,0,64,64 });

	m_show->DrawTexture((is_show[L"Rightkey"] ? L"ArRR" : L"ArR"),
		SimpleMath::Vector2(1820.0f, 950.0f) * scale, { 1,1,1,1 }, SimpleMath::Vector2::One * scale,
		{ 32,32 }, { 0,0,64,64 });
}