/*
 *	@File	EditScene.cpp
 *	@Brief  エディットシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "EditScene.h"

// マップサイズ(Stage)
#define			COMMON_SIZE			1.0f

// 最低高度
#define			COMMON_LOW			COMMON_SIZE / 2

// カメラアングル
#define			CAMERA_ANGLE		45.0f


 //--------------------------------------------------------//
 //コンストラクタ                                          //
 //--------------------------------------------------------//
EditScene::EditScene() :
	IScene(),
	m_sphere{},						// 球
	m_spherePos{},
	m_box{},						// 箱
	m_obj{},
	m_map{},						// マップ
	m_boxCol{},						// 立方体当たり判定
	m_grassBox{ nullptr },			// モデル
	m_grassBoxDark{ nullptr }
{

}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
EditScene::~EditScene()
{
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
void EditScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	GetSystemManager()->GetCamera()->SetMoveMode(true);	    	// カメラ座標移動
	GetSystemManager()->GetCamera()->SetEagleMode(true);		// カメラ視点移動

	// スフィアの初期化(テスト)
	m_sphere = DirectX::GeometricPrimitive::CreateSphere(
		GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(), 
		COMMON_SIZE / 2
	);

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
void EditScene::Update(const float& elapsedTime, DirectX::Keyboard::State& keyState,
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
	
	if (keyState.Z) m_spherePos.y += 0.1f;
	if (keyState.X) m_spherePos.y -= 0.1f;

	// 当たり判定
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			m_boxCol.PushBox(&m_spherePos, m_obj[y][x].position,							// スフィア＆ボックス
				DirectX::SimpleMath::Vector3{ COMMON_SIZE / 2},								// サイズ
				DirectX::SimpleMath::Vector3{ COMMON_SIZE }									// サイズ
			);
			
			// 当っていたらTrueにする
			m_obj[y][x].hitFlag = m_boxCol.GetHitBoxFlag();

			m_aabbCol.HitAABB(m_spherePos, m_obj[y][x].position,	// スフィア＆ボックス
				DirectX::SimpleMath::Vector3{ COMMON_SIZE / 2},	    // サイズ
				DirectX::SimpleMath::Vector3{ COMMON_SIZE }			// サイズ
			);

			// 当っているときに右クリックで変動
			if (m_obj[y][x].hitFlag && 
				GetSystemManager()->GetMouseTrack()->rightButton && !keyState.LeftShift)
			{
				m_obj[y][x].state += 1;
				if (m_obj[y][x].state % 100 > 15)
				{
					m_obj[y][x].state = MapLoad::BoxState::GrassBox;
				}
			}
			// 当っているときに右クリックで変動
			if (m_obj[y][x].hitFlag &&
				GetSystemManager()->GetMouseTrack()->rightButton && keyState.LeftShift)
			{
				m_obj[y][x].state -= 1;
				if (m_obj[y][x].state % 100 < 1)
				{
					m_obj[y][x].state = MapLoad::BoxState::GrassBox + 15;
				}
			}
			// ポジションの変更
			m_obj[y][x].position =
			{
				x * COMMON_SIZE - (m_map.MAP_COLUMN / 2 * COMMON_SIZE),		// ブロックの位置 - オフセット
				COMMON_LOW + m_obj[y][x].state % 100 * COMMON_SIZE,			// 最低高度 + 高度 * サイズ
				y * COMMON_SIZE - (m_map.MAP_RAW / 2 * COMMON_SIZE)			// ブロックの位置 - オフセット
			};
		}
	}

	// カメラ移動モード切り替え　デバッグ用
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::M))
	{
		auto flag = GetSystemManager()->GetCamera();
		flag->SetMoveMode(!flag->GetMoveMode());
		flag->SetEagleMode(!flag->GetEagleMode());
	}

	// ESCキーで終了
	if (keyState.Escape) ExitApp();
	
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(DirectX::Keyboard::Enter))
	{
		// 内容を記録
		for (int y = 0; y < m_map.MAP_RAW; y++)
		{
			for (int x = 0; x < m_map.MAP_COLUMN; x++)
			{
				m_map.SetMapData(m_obj[y][x].state, x, y);
			}
		}
		// ファイル書き出し
		m_map.WriteMap();
	}

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::RESULT);
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void EditScene::Draw()
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

	// ボックスの描画
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			for (int h = 0; h < m_obj[y][x].state % 100; h++)
			{
				m_obj[y][x].position.y = h * COMMON_SIZE;

				// ボックスの移動
				DirectX::SimpleMath::Matrix boxWorldMat =
					DirectX::SimpleMath::Matrix::CreateTranslation(m_obj[y][x].position);

				if (m_obj[y][x].state % 100 == 0) return;	// ボックスがなければ描画しない
				
				// 描画処理
				if (m_obj[y][x].hitFlag && GetSystemManager()->GetRayCast()->GetClickFlag())
				{
					m_grassBoxDark->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
						*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, true);
				}
				else if (m_obj[y][x].hitFlag)
				{
					m_grassBoxDark->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
						*GetSystemManager()->GetCommonStates(), boxWorldMat, view, projection, false);
				}
				else
				{
					m_grassBox->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
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
void EditScene::Finalize()
{
}

//--------------------------------------------------------//
//画面依存の初期化                                        //
//--------------------------------------------------------//
void EditScene::CreateWindowDependentResources()
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
	m_grassBox = ModelFactory::GetModel(					// 草ブロック-通常時
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_grassBoxDark = ModelFactory::GetModel(				// 草ブロック-選択時
		device,
		L"Resources/Models/GrassBlock_Dark.cmo"
	);
}


//--------------------------------------------------------//
//デバッグ表示                                            //
//--------------------------------------------------------//
void EditScene::DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	GetSystemManager()->GetString()->ChangeFontColor(DirectX::Colors::Black);

	// シーン名の表示
	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,0 }, L"EditScene");

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
void EditScene::LoadMap(int num)
{
	// 空のファイルパスを用意する
	std::wstring filePath = L"";

	// マップの読み込み
	m_map.LoadMap(filePath);

	// マップの格納
	for (int y = 0; y < m_map.MAP_RAW; y++)
	{
		for (int x = 0; x < m_map.MAP_COLUMN; x++)
		{
			// 読み込んだデータを格納
			m_obj[y][x].state = m_map.GetMapData(x, y);
			
			// 配列のごみを除去
			m_obj[y][x].position = DirectX::SimpleMath::Vector3::Zero;

			// ボックスの位置を初期化
			m_obj[y][x].position =
			{
				x * COMMON_SIZE - (m_map.MAP_COLUMN / 2 * COMMON_SIZE),		// ブロックの位置 - オフセット
				COMMON_LOW,													// ブロックの最低高度
				y * COMMON_SIZE - (m_map.MAP_RAW / 2 * COMMON_SIZE)			// ブロックの位置 - オフセット
			};
		}
	}
}