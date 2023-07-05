/*
 *	@File	ResultScene.cpp
 *	@Brief  リザルトシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ResultScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
ResultScene::ResultScene():
	IScene()
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

	GetSystemManager()->GetCamera()->SetEagleMode(true);		// カメラ視点移動
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

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		GoNextScene(SCENE::TITLE);
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