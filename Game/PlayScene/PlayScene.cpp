/*
 *	@File	PlayScene.cpp
 *	@Brief  プレイシーン。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "PlayScene.h"

 // マップサイズ(Stage)
#define			COMMON_SIZE			1.0f

// 最低高度
#define			COMMON_LOW			COMMON_SIZE / 2

// カメラアングル
#define			CAMERA_ANGLE		45.0f


 //--------------------------------------------------------//
 //コンストラクタ                                          //
 //--------------------------------------------------------//
PlayScene::PlayScene() :
	IScene(),
	m_obj{},
	m_map{},						// マップ
	m_boxCol{},						// 立方体当たり判定
	m_grassBox{ nullptr }			// モデル
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

	GetSystemManager()->GetCamera()->SetMoveMode(false);    	// カメラ座標移動
	GetSystemManager()->GetCamera()->SetEagleMode(true);		// カメラ視点移動

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
	float width = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
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

	// ボックスの描画
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			for (int h = 0; h < m_obj[y][x].state % 100; h++)
			{
				m_obj[y][x].position.y = COMMON_LOW + h * COMMON_SIZE; // 最低座標＋任意の高さ

				// ボックスの移動
				DirectX::SimpleMath::Matrix boxWorldMat =
					DirectX::SimpleMath::Matrix::CreateTranslation(m_obj[y][x].position);

				if (m_obj[y][x].state % 100 == MapLoad::BoxState::None) return;	// ボックスがなければ描画しない

				m_grassBox->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
					*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, false);

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
	auto device = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique(device, context);

	// 画面サイズの格納
	float width = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// カメラの設定
	GetSystemManager()->GetCamera()->GetProjection(width, height, CAMERA_ANGLE);

	// 文字の設定
	GetSystemManager()->GetString()->CreateString(device, context);

	// レイが及ぶ範囲を設定
	GetSystemManager()->GetRayCast()->SetScreenSize(width, height);

	// モデルを作成する
	m_grassBox = ModelFactory::GetModel(					// 草ブロック-通常時
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

	// ステージ番号確認
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,60 }, num);


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
	std::wstring filePath = L"";

	// マップの変更
	switch (num)
	{
	case 1:
		filePath = L"Resources/Maps/Stage1.csv";
		break;
	case 2:
		filePath = L"Resources/Maps/Stage2.csv";
		break;
	case 3:
		filePath = L"Resources/Maps/Stage3.csv";
		break;
	default:
		filePath = L"NoStage";
		break;
	}

	// マップの読み込み
	m_map.LoadMap(filePath);

	// マップの格納
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			// ステートをセット
			m_obj[y][x].state = m_map.GetMapData(x, y);

			// 配列のごみを除去
			m_obj[y][x].position = DirectX::SimpleMath::Vector3::Zero;
								
			// ボックスの位置を初期化
			m_obj[y][x].position =
			{
				x * COMMON_SIZE - (m_map.MAP_COLUMN / 2 * COMMON_SIZE),							// ブロックの位置 - オフセット
				COMMON_LOW,						 												// ブロック初期位置
				y * COMMON_SIZE - (m_map.MAP_RAW / 2 * COMMON_SIZE)								// ブロックの位置 - オフセット
			};
			
		}
	}
}