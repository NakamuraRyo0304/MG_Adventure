/*
 *	@File	TitleScene.cpp
 *	@Brief	タイトルシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "TitleScene.h"

 //--------------------------------------------------------//
 //コンストラクタ                                          //
 //--------------------------------------------------------//
TitleScene::TitleScene():
	IScene()
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
TitleScene::~TitleScene()
{
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
void TitleScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	GetSystemManager()->GetCamera()->SetMoveMode(false);		// カメラ座標移動
	GetSystemManager()->GetCamera()->SetEagleMode(false);		// カメラ視点移動
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
// 第１引数：時間(60FPS = 1sec) / 第２引数：キーボードのポインタ / 第３引数：マウスのポインタ
void TitleScene::Update(const float& elapsedTime,DirectX::Keyboard::State& keyState,
	DirectX::Mouse::State& mouseState)
{
	elapsedTime;

	// キー入力情報を取得する
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// ESCキーで終了
	if (keyState.Escape) ExitApp();

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::SELECT);
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void TitleScene::Draw()
{
	// デバッグフォント
	GetSystemManager()->GetString()->ChangeFontColor(DirectX::Colors::Black);
	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(), 
		{ 0,0 }, 
		L"TitleScene"
	);
}

//--------------------------------------------------------//
//終了処理                                                //
//--------------------------------------------------------//
void TitleScene::Finalize()
{
}

//--------------------------------------------------------//
//画面依存の初期化                                        //
//--------------------------------------------------------//
void TitleScene::CreateWindowDependentResources()
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