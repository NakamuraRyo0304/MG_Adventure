/*
 *	@File	SelectScene.cpp
 *	@Brief  セレクトシーン。
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "SelectScene.h"

// 角度
const float SelectScene::CAMERA_ANGLE = 45.0f;

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
SelectScene::SelectScene():
	IScene(),
	m_tea{},
	m_angle{}
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

	GetSystemManager()->GetCamera()->SetEagleMode(false);	// カメラ視点移動

}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
// 第１引数：時間(60FPS = 1sec) / 第２引数：キーボードのポインタ / 第３引数：マウスのポインタ
void SelectScene::Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,
	DirectX::Mouse::State& mouseState)
{
	m_angle = elapsedTime;

	// キー入力情報を取得する
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// ESCキーで終了
	if (keyState.Escape) ExitApp();

	// ステージ番号変更
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Right))
	{
		m_stageNum++;
	}
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Left))
	{
		m_stageNum--;
	}
	
	auto clamp = [](int value, int min, int max)
	{
		if (min > value) return min;
		if (max < value) return max;
		return value;
	};
	m_stageNum = clamp(m_stageNum, 0, 3);

	// ESCキーで終了
	if (keyState.Escape) ExitApp();

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		if (m_stageNum == 0)
		{
			// 0番目を選択したらステージエディタのシーンへ飛ぶ
			GoNextScene(SCENE::EDIT);
		}
		else
		{
			GoNextScene(SCENE::PLAY);
		}
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void SelectScene::Draw()
{
	// デバッグフォント
	GetSystemManager()->GetString()->ChangeFontColor(DirectX::Colors::Black);
	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(),
		{ 0,0 },
		L"SelectScene"
	);

	// ステージ番号確認
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(),
		{ 0,20 },
		num
	);

	// カメラ用行列
	DirectX::SimpleMath::Matrix world, view, projection;

	// ワールド行列
	world = DirectX::SimpleMath::Matrix::Identity;

	// ビュー行列
	view = GetSystemManager()->GetCamera()->GetView();

	// プロジェクション行列
	projection = GetSystemManager()->GetCamera()->GetProjection();


	// 箱の描画
	world *= DirectX::SimpleMath::Matrix::CreateRotationY(m_angle);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(0,0,0);
	switch (m_stageNum)
	{
	case 0:
		m_tea->Draw(world, view, projection, DirectX::Colors::Black);
		break;
	case 1:
		m_tea->Draw(world, view, projection, DirectX::Colors::Red);
		break;
	case 2:
		m_tea->Draw(world, view, projection, DirectX::Colors::Blue);
		break;
	case 3:
		m_tea->Draw(world, view, projection, DirectX::Colors::Green);
		break;
	default:
		break;
	}
	
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
	auto device = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique(device, context);
	GetSystemManager()->GetString()->CreateString(device, context);

	// 画面サイズの格納
	float width =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	m_tea = DirectX::GeometricPrimitive::CreateTeapot(context,2.0f);

}