/*
 *	@File	SelectScene.cpp
 *	@Brief  セレクトシーン。
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SelectScene.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
SelectScene::SelectScene():
	IScene()
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
SelectScene::~SelectScene()
{
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
void SelectScene::Initialize()
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
void SelectScene::Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,
	DirectX::Mouse::State& mouseState)
{
	elapsedTime;

	// キー入力情報を取得する
	GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetMouseTrack()->Update(mouseState);

	// カメラの更新
	m_camera->Update();

	// ステージ番号変更
	if (GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Right)) m_stageNum++;
	if (GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Left))  m_stageNum--;
	auto clamp = [](int value,int min,int max)
	{
		if (min > value) return min;
		if (max < value) return max;
		return value;
	};
	m_stageNum = clamp(m_stageNum, 1, 3);

	// ESCキーで終了
	if (keyState.Escape) ExitApp();
	// Spaceキーでシーン切り替え
	if (GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::PLAY);
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void SelectScene::Draw()
{
	// デバッグフォント
	GetString()->ChangeFontColor(DirectX::Colors::Black);
	GetString()->DrawFormatString(GetCommonStates().get(), { 0,0 }, L"SelectScene");

	// ステージ番号確認
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetString()->DrawFormatString(GetCommonStates().get(), { 0,20 }, num);
}

//--------------------------------------------------------//
//終了処理                                                //
//--------------------------------------------------------//
void SelectScene::Finalize()
{
}

//--------------------------------------------------------//
//画面依存の初期化                                        //
//--------------------------------------------------------//
void SelectScene::CreateWindowDependentResources()
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