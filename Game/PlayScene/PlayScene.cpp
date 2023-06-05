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

// カメラアングル
#define			CAMERA_ANGLE		45.0f


 //--------------------------------------------------------//
 //コンストラクタ                                          //
 //--------------------------------------------------------//
PlayScene::PlayScene() :
	IScene(),
	m_sphere{},			// 球
	m_spherePos{},
	m_box{},			// 箱
	m_boxesPos{},
	m_map{},			// マップ
	m_mapData{},
	m_boxCol{},			// 立方体当たり判定
	is_boxesHitFlag{},
	m_boxModel{nullptr}	// モデル
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

	GetSystemManager()->GetCamera()->SetMoveMode(true);	    	// カメラ座標移動
	GetSystemManager()->GetCamera()->SetEagleMode(false);		// カメラ視点移動

	// スフィアの初期化(テスト)
	m_sphere = DirectX::GeometricPrimitive::CreateSphere(
		GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(), COMMON_SIZE / 2);

	// ボックスの初期化(テスト)
	m_box = DirectX::GeometricPrimitive::CreateBox(
		GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(), 
		DirectX::XMFLOAT3(COMMON_SIZE, COMMON_SIZE, COMMON_SIZE)
	);

	// マップ読み込み
	LoadMap(GetStageNum());

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
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();
	
	// レイの更新
	GetSystemManager()->GetRayCast()->Update(mouseState);

	// 移動したい位置を設定
	m_spherePos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_spherePos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;
	
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Z)) m_spherePos.y+=1;
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::X)) m_spherePos.y-=1;

	// 当たり判定
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			m_boxCol.PushBox(&m_spherePos, m_boxesPos[y][x],		// スフィア＆ボックス
				DirectX::SimpleMath::Vector3{ COMMON_SIZE / 2 },	// サイズ
				DirectX::SimpleMath::Vector3{ COMMON_SIZE }			// サイズ
			);
			is_boxesHitFlag[y][x] = m_boxCol.GetHitBoxFlag();

			//// 当っているときに右クリックで変動
			//if (is_boxesHitFlag[y][x] && mouseState.rightButton)
			//{
			//	m_boxesPos[y][x].y += COMMON_SIZE / 2;
			//	if (m_boxesPos[y][x].y > COMMON_SIZE / 2 * 10)
			//	{
			//		m_boxesPos[y][x].y = COMMON_SIZE / 2;
			//	}
			//}
		}
	}

	// ESCキーで終了
	if (keyState.Escape) ExitApp();

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
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
	float width  = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// カメラ用行列
	DirectX::SimpleMath::Matrix world, view, projection;

	// ワールド行列
	world = DirectX::SimpleMath::Matrix::Identity;

	// ビュー行列
	view = GetSystemManager()->GetCamera()->GetView();

	// プロジェクション行列
	projection = GetSystemManager()->GetCamera()->GetProjection(width, height, CAMERA_ANGLE);

	// レイの設定
	GetSystemManager()->GetRayCast()->SetMatrix(view, projection);

	// 球の描画
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_spherePos);

	m_sphere->Draw(world, view, projection, DirectX::Colors::Red);

	// 座標設定
	world = DirectX::SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
	

	// ボックスの描画
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			// ボックスの移動
			DirectX::SimpleMath::Matrix boxWorldMat = DirectX::SimpleMath::Matrix::CreateTranslation(m_boxesPos[y][x]);

			if (m_mapData[y][x] != 0)
			{
				if (is_boxesHitFlag[y][x] && GetSystemManager()->GetRayCast()->GetClickFlag())
				{
					m_box->Draw(boxWorldMat, view, projection, DirectX::Colors::Red);
				}
				else if (is_boxesHitFlag[y][x])
				{
					m_box->Draw(boxWorldMat, view, projection, DirectX::Colors::LightPink);
				}
				else
				{
					m_boxModel->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
						*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, false);
				}
			}
		}
	}

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
	auto device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique(device, context);

	// 画面サイズの格納
	float width  = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);
	
	// カメラの設定
	GetSystemManager()->GetCamera()->GetProjection(width, height, CAMERA_ANGLE);

	// 文字の設定
	GetSystemManager()->GetString()->CreateString(device, context);

	// レイが及ぶ範囲を設定
	GetSystemManager()->GetRayCast()->SetScreenSize(width, height);
	
	// モデルを作成する
	m_boxModel = ModelFactory::GetModel(
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
}


//--------------------------------------------------------//
//デバッグ表示                                            //
//--------------------------------------------------------//
void PlayScene::DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	GetSystemManager()->GetString()->ChangeFontColor(DirectX::Colors::Black);

	// シーン名の表示
	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,0 }, L"PlayScene");

	// 文字数設定
	wchar_t cam[64];

	// カメラのポジション
	swprintf_s(cam, 64, L"CameraPos = %d,%d,%d",
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().x),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().y),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().z)
	);
	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,20 }, cam);

	// マウスのワールド座標
	wchar_t mos[64];
	swprintf_s(mos, 64, L"WorldMousePos = %f,%f,%f",
		GetSystemManager()->GetRayCast()->GetWorldMousePosition().x,
		GetSystemManager()->GetRayCast()->GetWorldMousePosition().y,
		GetSystemManager()->GetRayCast()->GetWorldMousePosition().z);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,40 }, mos);

	// 保存された座標
	swprintf_s(mos, 64, L"ClickPos = %f,%f,%f",
		m_spherePos.x,
		m_spherePos.y,
		m_spherePos.z);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,60 }, mos);

	// ステージ番号確認
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,80 }, num);

	
	// デバイスコンテキストの取得：グリッドの描画に使用
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	// デバッググリッドの表示
	GetSystemManager()->GetGridFloor()->Draw(context, GetSystemManager()->GetCommonStates().get(), view, proj);
}

//--------------------------------------------------------//
//マップ読み込み                                          //
//--------------------------------------------------------//
void PlayScene::LoadMap(int num)
{
	// ファイル名の宣言
	const wchar_t* filename = L"CleanData";

	// マップの変更
	switch (num)
	{
	case 1:	
		filename = L"Resources/Maps/Stage1.csv";
		break;
	case 2:	
		filename = L"Resources/Maps/Stage2.csv";
		break;
	case 3:
		filename = L"Resources/Maps/Stage3.csv";
		break;
	default:
		filename = L"NoStage";
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
			
			m_boxesPos[y][x] = { 0,0,0 };

			m_boxesPos[y][x] =
			{
				x * COMMON_SIZE - (m_map.MAP_COLUMN / 2 * COMMON_SIZE) ,	// ブロックの位置 - オフセット
				COMMON_SIZE / 2,											// 高度
				y * COMMON_SIZE - (m_map.MAP_RAW / 2 * COMMON_SIZE)			// ブロックの位置 - オフセット
			};
		}
	}
}