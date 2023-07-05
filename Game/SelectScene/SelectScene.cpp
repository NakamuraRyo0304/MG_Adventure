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

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
SelectScene::SelectScene():
	IScene(),
	m_tea{},
	m_angle{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
SelectScene::~SelectScene()
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	GetSystemManager()->GetCamera()->SetEagleMode(false);	// カメラ視点移動

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">時間/fps</param>
/// <param name="keyState">キーボードポインタ</param>
/// <param name="mouseState">マウスポインタ</param>
/// <returns>なし</returns>
void SelectScene::Update(const float& elapsedTime, Keyboard::State& keyState,
	Mouse::State& mouseState)
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
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Right))
	{
		m_stageNum++;
	}
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Left))
	{
		m_stageNum--;
	}
	
	m_stageNum = UserUtillity::Clamp(m_stageNum, 0, 3);

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
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

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectScene::Draw()
{
	// デバッグフォント
	GetSystemManager()->GetString()->ChangeFontColor(Colors::Black);
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
	SimpleMath::Matrix world, view, projection;

	// ワールド行列
	world = SimpleMath::Matrix::Identity;

	// ビュー行列
	view = GetSystemManager()->GetCamera()->GetView();

	// プロジェクション行列
	projection = GetSystemManager()->GetCamera()->GetProjection();


	// 箱の描画
	world *= SimpleMath::Matrix::CreateRotationY(m_angle);
	world *= SimpleMath::Matrix::CreateTranslation(0,0,0);
	switch (m_stageNum)
	{
	case 0:
		m_tea->Draw(world, view, projection, Colors::Black);
		break;
	case 1:
		m_tea->Draw(world, view, projection, Colors::Red);
		break;
	case 2:
		m_tea->Draw(world, view, projection, Colors::Blue);
		break;
	case 3:
		m_tea->Draw(world, view, projection, Colors::Green);
		break;
	default:
		break;
	}
	
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectScene::Finalize()
{
}

/// <summary>
/// 画面依存、デバイス依存の初期化
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
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

	m_tea = GeometricPrimitive::CreateTeapot(context,2.0f);

}