/*
 *	@File	PlayScene.cpp
 *	@Brief	プレイシーン
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "PlayScene.h"

// マップサイズ(Stage)
#define			COMMON_SIZE			1.0f


 //--------------------------------------------------------//
 //コンストラクタ                                          //
 //--------------------------------------------------------//
PlayScene::PlayScene() :
	IScene(),
	m_sphere{},
	m_boxesPos{},
	m_map{},
	m_mapData{},
	m_size{},
	m_spherePos{ DirectX::SimpleMath::Vector3::Zero }		// デバッグ用
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
PlayScene::~PlayScene()
{
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
void PlayScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	m_camera->SetMoveMode(false);				// カメラ座標移動
	m_camera->SetEagleMode(false);				// カメラ視点移動

	// マップ読み込み
	LoadMap(GetStageNum());

	// スフィアの初期化
	m_sphere = DirectX::GeometricPrimitive::CreateSphere(
		GetDeviceResources()->GetD3DDeviceContext(), COMMON_SIZE);

	m_size = 0;
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
// 第１引数：時間(60FPS = 1sec) / 第２引数：キーボードのポインタ / 第３引数：マウスのポインタ
void PlayScene::Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,
	DirectX::Mouse::State& mouseState)
{
	elapsedTime;

	// キー入力情報を取得する
	GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetMouseTrack()->Update(mouseState);

	// カメラの更新
	m_camera->Update();

	// レイの更新
	GetRayCast()->Update(mouseState);

	// ボックスのサイズ値を変更
	if (GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Up))
	{
		m_spherePos.y += COMMON_SIZE / 2;
		m_size += COMMON_SIZE;
	}
	if (GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Down))
	{
		m_spherePos.y -= COMMON_SIZE / 2;
		m_size -= COMMON_SIZE;
	}

	// 移動したい位置を設定
	if (GetRayCast()->GetClickFlag())
	{
		float save = m_spherePos.y;
		m_spherePos = GetRayCast()->GetWorldMousePosition();
		m_spherePos.y = save;
	}

	// ESCキーで終了
	if (keyState.Escape) ExitApp();
	// Spaceキーでシーン切り替え
	if (GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::RESULT);
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void PlayScene::Draw()
{
	// 画面サイズの格納
	float width  = static_cast<float>(GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetDeviceResources()->GetOutputSize().bottom);

	// カメラ用行列
	DirectX::SimpleMath::Matrix world, view, projection;

	// ワールド行列
	world = DirectX::SimpleMath::Matrix::Identity;

	// ビュー行列
	view = m_camera->GetView();

	// プロジェクション行列
	projection = m_camera->GetProjection(width, height, 45.0f);

	// レイの設定
	GetRayCast()->SetMatrix(view, projection);

	// 球の描画
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_spherePos);

	m_sphere->Draw(world, view, projection, DirectX::Colors::Red);

	// 座標設定
	world = DirectX::SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
	
	world *= DirectX::SimpleMath::Matrix::CreateScale(0.001f);

	// ステージボックスの表示
	m_boxModel->Draw(GetDeviceResources()->GetD3DDeviceContext(),
		*GetCommonStates(), world, view, projection);

	// デバッグ表示
	DebugLog(view, projection);
}

//--------------------------------------------------------//
//終了処理                                                //
//--------------------------------------------------------//
void PlayScene::Finalize()
{
}

//--------------------------------------------------------//
//画面依存の初期化                                        //
//--------------------------------------------------------//
void PlayScene::CreateWindowDependentResources()
{
	// デバイスとデバイスコンテキストの取得
	auto device  = GetDeviceResources()->GetD3DDevice();
	auto context = GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	CreateUnique(device, context);

	// 画面サイズの格納
	float width  = static_cast<float>(GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetDeviceResources()->GetOutputSize().bottom);
	
	// カメラの設定
	m_camera->GetProjection(width, height, 45.0f);

	// 文字の設定
	GetString()->CreateString(device, context);

	// レイが及ぶ範囲を設定
	GetRayCast()->SetScreenSize(width, height);
	
	// モデルを作成する
	m_boxModel = ModelFactory(
		device,
		L"Resources/Models/GrassBox.cmo"
	);
}


//--------------------------------------------------------//
//デバッグ表示                                            //
//--------------------------------------------------------//
void PlayScene::DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	GetString()->ChangeFontColor(DirectX::Colors::Black);

	// シーン名の表示
	GetString()->DrawFormatString(GetCommonStates().get(), { 0,0 }, L"PlayScene");

	// 文字数設定
	wchar_t cam[64];

	// カメラのポジション
	swprintf_s(cam, 64, L"CameraPos = %d,%d,%d",
		static_cast<int>(m_camera->GetEyePosition().x),
		static_cast<int>(m_camera->GetEyePosition().y),
		static_cast<int>(m_camera->GetEyePosition().z)
	);
	GetString()->DrawFormatString(GetCommonStates().get(), { 0,20 }, cam);

	// マウスのワールド座標
	wchar_t mos[64];
	swprintf_s(mos, 64, L"WorldMousePos = %f,%f,%f",
		GetRayCast()->GetWorldMousePosition().x,
		GetRayCast()->GetWorldMousePosition().y,
		GetRayCast()->GetWorldMousePosition().z);

	GetString()->DrawFormatString(GetCommonStates().get(), { 0,40 }, mos);

	// 保存された座標
	swprintf_s(mos, 64, L"ClickPos = %f,%f,%f",
		m_spherePos.x,
		m_spherePos.y,
		m_spherePos.z);

	GetString()->DrawFormatString(GetCommonStates().get(), { 0,60 }, mos);

	// ステージ番号確認
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetString()->DrawFormatString(GetCommonStates().get(), { 0,80 }, num);

	
	// デバイスコンテキストの取得：グリッドの描画に使用
	auto context = GetDeviceResources()->GetD3DDeviceContext();
	// デバッググリッドの表示
	GetGridFloor()->Draw(context, GetCommonStates().get(), view, proj);
}

//--------------------------------------------------------//
//マップ読み込み                                          //
//--------------------------------------------------------//
void PlayScene::LoadMap(int num)
{
	// ファイル名の宣言
	const char* filename = "CleanData";

	// マップの変更
	switch (num)
	{
	case 1:	
		filename = "Resources/Maps/Stage1.csv";	
		break;
	case 2:	
		filename = "Resources/Maps/Stage2.csv";	
		break;
	case 3:
		filename = "Resources/Maps/Stage3.csv";
		break;
	default:
		filename = "NoStage";
		break;
	}

	// マップの読み込み
	m_map.SetMapData(filename);


	// マップの格納
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			m_mapData[y][x] = m_map.GetMapData(x, y);
			m_boxesPos[y][x] = { x * COMMON_SIZE, COMMON_SIZE / 2, y * COMMON_SIZE};
		}
	}
}