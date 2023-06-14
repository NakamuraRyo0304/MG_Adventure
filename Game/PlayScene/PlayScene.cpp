/*
 *	@File	PlayScene.cpp
 *	@Brief  プレイシーン。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "PlayScene.h"

 //--------------------------------------------------------//
 //コンストラクタ                                          //
 //--------------------------------------------------------//
PlayScene::PlayScene() :
	IScene(),
	m_mapObj{},						// マップのブロック
	m_map{},						// マップ
	is_boxCol{},					// 立方体当たり判定
	m_playerModel{ nullptr },		// プレイヤのモデル
	m_playerPos{},					// プレイヤの位置
	m_grassModel{ nullptr },		// 草ブロックのモデル
	m_coinModel{ nullptr },			// コインブロックのモデル
	m_clowdModel{ nullptr },		// 雲ブロックのモデル
	m_gravity{}
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

	m_playerPos = { 0.0f,5.0f,0.0f };
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

	//　クラス分け　Player Boxも作る
	if (keyState.W) m_playerPos.z -= 0.05f;
	if (keyState.S) m_playerPos.z += 0.05f;
	if (keyState.A) m_playerPos.x -= 0.05f;
	if (keyState.D) m_playerPos.x += 0.05f;

	m_gravity += 0.01f;

	m_playerPos.y -= m_gravity;

	// 当たり判定の更新
	DoBoxCollision();


	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::PLAY);
	}

	// ESCキーで終了
	if (keyState.Escape) ExitApp();

	// Enterキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Enter))
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

	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

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

	// プレイヤの描画
	DirectX::SimpleMath::Matrix playerWorldMat =
		DirectX::SimpleMath::Matrix::CreateTranslation(m_playerPos);
	m_coinModel->Draw(context, states, playerWorldMat, view, projection, false);

	// オブジェクトの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		DirectX::SimpleMath::Matrix boxMat =
			DirectX::SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		if (m_mapObj[i].id == MapLoad::BoxState::GrassBox)
		{
			m_grassModel->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
				*GetSystemManager()->GetCommonStates(), boxMat, view, projection);
		}
		if (m_mapObj[i].id == MapLoad::BoxState::CoinBox)
		{
			m_coinModel->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
				*GetSystemManager()->GetCommonStates(), boxMat, view, projection);
		}
	}

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
	m_playerModel = ModelFactory::GetModel(						// プレイヤ
		device,
		L"Resources/Models/TestPlayer.cmo"
	);
	m_grassModel = ModelFactory::GetModel(						// 草ブロック
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_coinModel = ModelFactory::GetModel(						// コインブロック
		device,
		L"Resources/Models/Coin.cmo"
	);
	m_clowdModel = ModelFactory::GetModel(						// 雲ブロック
		device,
		L"Resources/Models/Clowd.cmo"
	);
}

//--------------------------------------------------------//
//当たり判定の処理をまとめた関数                          //
//--------------------------------------------------------//
void PlayScene::DoBoxCollision()
{
	// マップとの当たり判定
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		// 当たり判定を取る
		is_boxCol.PushBox(m_playerPos, m_mapObj[i].position,
			DirectX::SimpleMath::Vector3{ COMMON_SIZE },
			DirectX::SimpleMath::Vector3{ COMMON_SIZE });

		if (is_boxCol.GetHitBoxFlag()&& m_mapObj[i].id == MapLoad::BoxState::GrassBox)
		{
			switch (is_boxCol.GetHitFace())
			{
			case Collider::BoxCollider::HIT_FACE::UP:
				m_gravity = 0.0f;
				m_playerPos.y = m_mapObj[i].position.y + COMMON_SIZE;
				break;
			default:
				break;
			}
		}
	}
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
	
	// プレイヤの座標
	wchar_t plr[64];
	swprintf_s(plr, 64, L"Player = %f,%f,%f", m_playerPos.x,m_playerPos.y,m_playerPos.z);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,80 }, plr);

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
	m_mapObj = m_map.GetMapData();

	// 座標補正
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		m_mapObj[i].position.x -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		m_mapObj[i].position.y += static_cast<float>(COMMON_LOW);
		m_mapObj[i].position.z -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
	}
}