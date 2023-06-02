/*
 *	@File	ResultScene.cpp
 *	@Brief  リザルトシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ResultScene.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
ResultScene::ResultScene():
	IScene()
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
ResultScene::~ResultScene()
{
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
void ResultScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	m_camera->SetMoveMode(true);				// カメラ座標移動
	m_camera->SetEagleMode(false);				// カメラ視点移動
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
// 第１引数：時間(60FPS = 1sec) / 第２引数：キーボードのポインタ / 第３引数：マウスのポインタ
void ResultScene::Update(const float& elapsedTime, DirectX::Keyboard::State& keyState, 
	DirectX::Mouse::State& mouseState)
{
	elapsedTime;

	// キー入力情報を取得する
	GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetMouseTrack()->Update(mouseState);

	// カメラの更新
	m_camera->Update();

	// ESCキーで終了
	if (keyState.Escape) ExitApp();

	// Spaceキーでシーン切り替え
	if (GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::TITLE);
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void ResultScene::Draw()
{
	// デバッグフォント
	GetString()->ChangeFontColor(DirectX::Colors::Black);
	GetString()->DrawFormatString(GetCommonStates().get(), { 0,0 }, L"ResultScene");
}

//--------------------------------------------------------//
//終了処理                                                //
//--------------------------------------------------------//
void ResultScene::Finalize()
{
}

//--------------------------------------------------------//
//画面依存の初期化                                        //
//--------------------------------------------------------//
void ResultScene::CreateWindowDependentResources()
{
	// デバイスとデバイスコンテキストの取得
	auto device  = GetDeviceResources()->GetD3DDevice();
	auto context = GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	CreateUnique(device, context);
	GetString()->CreateString(device, context);

	// 画面サイズの格納
	float width  = static_cast<float>(GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetDeviceResources()->GetOutputSize().bottom);

	// カメラの設定
	m_camera->GetProjection(width, height, 45.0f);

}