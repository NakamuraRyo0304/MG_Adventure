/*
 *	@File	ResultScene.cpp
 *	@Brief  リザルトシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ユーザーユーティリティ
#include "Libraries/UserUtility.h"

#include "ResultScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
ResultScene::ResultScene():
	IScene(),
	m_selectNum{RETRY}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
ResultScene::~ResultScene()
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// カメラ視点移動
	GetSystemManager()->GetCamera()->SetEagleMode(false);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">時間/fps</param>
/// <param name="keyState">キーボードポインタ</param>
/// <param name="mouseState">マウスポインタ</param>
/// <returns>なし</returns>
void ResultScene::Update(const float& elapsedTime, Keyboard::State& keyState, 
	Mouse::State& mouseState)
{
	elapsedTime;

	// キー入力情報を取得する
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// メニューセレクト
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Right))
	{
		m_selectNum++;
	}
	else if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Left))
	{
		m_selectNum--;
	}

	// クランプ
	m_selectNum = UserUtility::Clamp(m_selectNum, static_cast<int>(RETRY), static_cast<int>(TITLE));

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		switch (m_selectNum)
		{
		case RETRY:
			GoNextScene(SCENE::PLAY);
			break;
		case SELECT:
			GoNextScene(SCENE::SELECT);
			break;
		case TITLE:
			GoNextScene(SCENE::TITLE);
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::Draw()
{
	// デバッグフォント
	GetSystemManager()->GetString()->ChangeFontColor(Colors::Black);
	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(), 
		{ 0,0 }, 
		L"ResultScene"
	);

	wchar_t sel[10];

	if (m_selectNum == RETRY)
	{
		swprintf_s(sel, 10, L"Retry");
	}
	else if (m_selectNum == SELECT)
	{
		swprintf_s(sel, 10, L"Select");
	}
	else if (m_selectNum == TITLE)
	{
		swprintf_s(sel, 10, L"Title");
	}

	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(), 
		{ 0,20 }, 
		sel
	);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::Finalize()
{
}

/// <summary>
/// 画面依存、デバイス依存の初期化
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::CreateWindowDependentResources()
{
	// デバイスとデバイスコンテキストの取得
	auto device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique(device, context);
	GetSystemManager()->GetString()->CreateString(device, context);

	// 画面サイズの格納
	float width  = 
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = 
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, 45.0f);

}