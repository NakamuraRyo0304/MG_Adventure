/*
 *	@File	PlayScene.cpp
 *	@Brief  プレイシーン。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

 // CSV読み込み
#include "../../Libraries/SystemDatas/MapLoad.h"

// コライダークラス
#include "../../Libraries/SystemDatas/Collider.h"

// プレイヤクラス
#include "Objects/Player.h"

#include "PlayScene.h"

 //--------------------------------------------------------//
 //コンストラクタ                                          //
 //--------------------------------------------------------//
PlayScene::PlayScene() :
	IScene(),
	m_mapObj{},						// マップのブロック
	m_map{},						// マップ
	m_colObjList{},					// 当っているオブジェクトの格納
	is_boxCol{},					// 立方体当たり判定
	m_playerPos{},					// プレイヤの位置
	m_grassModel{ nullptr },		// 草ブロックのモデル
	m_coinModel{ nullptr },			// コインブロックのモデル
	m_clowdModel{ nullptr },		// 雲ブロックのモデル
	test_count{}
{

}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
PlayScene::~PlayScene()
{
	Finalize();
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
void PlayScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	GetSystemManager()->GetCamera()->SetEagleMode(true);		// カメラ視点移動

	// マップ読み込み
	LoadMap(GetStageNum());

	// プレイヤの初期化
	m_player->Initialize();
	m_player->SetPosition(DirectX::SimpleMath::Vector3{ 0.0f,5.0f,0.0f });

	// 判定の初期化
	m_colObjList.clear();
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

	// プレイヤの更新
	m_player->Update(keyState);

	// 当たり判定の更新
	DoBoxCollision();

	// 衝突したオブジェクトごとに押し戻し処理を行う
	for (auto& obj : m_colObjList)
	{
		ApplyPushBack(obj);
	}

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
	// 描画関連
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	DirectX::SimpleMath::Matrix world, view, proj;

	// ワールド行列
	world = DirectX::SimpleMath::Matrix::Identity;

	// ビュー行列
	view = GetSystemManager()->GetCamera()->GetView();

	// プロジェクション行列
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// プレイヤの描画
	m_player->Render(context, states, view, proj);

	// オブジェクトの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		DirectX::SimpleMath::Matrix boxMat =
			DirectX::SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		if (m_mapObj[i].id == MapLoad::BoxState::GrassBox)
		{
			m_grassModel->Draw(context, states, boxMat, view, proj);
		}
		if (m_mapObj[i].id == MapLoad::BoxState::CoinBox)
		{
			m_coinModel->Draw(context, states, boxMat, view, proj);
		}
	}

	DebugLog(view, proj);
}

//--------------------------------------------------------//
//終了処理                                                //
//--------------------------------------------------------//
void PlayScene::Finalize()
{
	// プレイヤの後処理
	m_player->Finalize();

	// マップの解放
	m_mapObj.clear();
	std::vector<Object>(m_mapObj).swap(m_mapObj);

	// 判定用配列を解放
	m_colObjList.clear();
	std::vector<Object>(m_colObjList).swap(m_colObjList);

	// モデルのリリース
	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_clowdModel);
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
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// 文字の設定
	GetSystemManager()->GetString()->CreateString(device, context);

	// モデルを作成する
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

	// プレイヤの作成
	std::unique_ptr<DirectX::Model> playerModel = ModelFactory::GetModel(
		device,
		L"Resources/Models/TestPlayer.cmo"
	);
	// モデルデータを渡す(move必要)
	m_player = std::make_unique<Player>(std::move(playerModel));

}

//--------------------------------------------------------//
//当っていたらリストに追加する                            //
//--------------------------------------------------------//
void PlayScene::DoBoxCollision()
{
	// 衝突したオブジェクトリストを初期化
	m_colObjList.clear(); 

	// 当たり判定を取る
	for (auto& obj : m_mapObj)
	{
		// 判定を取る
		is_boxCol.PushBox(
			m_player->GetPosition(),								// プレイヤ座標
			obj.position,											// ブロック座標
			DirectX::SimpleMath::Vector3{ m_player->GetSize() },	// プレイヤサイズ
			DirectX::SimpleMath::Vector3{ COMMON_SIZE }				// ブロックサイズ
		);

		// 当たっていたら処理する
		if (is_boxCol.GetHitBoxFlag())
		{
			// 衝突したオブジェクトをリストに追加
			m_colObjList.push_back(obj);
		}
	}
}

//--------------------------------------------------------//
//押し戻しをする                                          //
//--------------------------------------------------------//
void PlayScene::ApplyPushBack(Object& obj)
{
	// 当っているオブジェクトが空気以外の場合は押し戻しを有効にする
	if (obj.id != MapLoad::BoxState::None)
	{
		is_boxCol.SetPushMode(true);
	}
	else
	{
		is_boxCol.SetPushMode(false);
	}

	// 当っているのが空気の場合は処理しない
	if (obj.id == MapLoad::BoxState::None) return;

	// コインの処理
	if (obj.id == MapLoad::BoxState::CoinBox)
	{ 
		// 該当コインの検索＆獲得処理
		for (auto& i : m_mapObj)
		{
			if (i == obj)
			{
				test_count += 1;				// 獲得数加算
				i.id = MapLoad::BoxState::None;	// 獲得したらコインを削除
			}
		}
		return;
	}

	// プレイヤのポジションを保存
	DirectX::SimpleMath::Vector3 playerPos = m_player->GetPosition();

	// 当たり判定を取って押し戻す
	is_boxCol.PushBox(
		&playerPos,												// プレイヤ座標
		obj.position,											// ブロック座標
		DirectX::SimpleMath::Vector3{ m_player->GetSize() },	// プレイヤサイズ
		DirectX::SimpleMath::Vector3{ COMMON_SIZE }				// ブロックサイズ
	);

	// 変更後のプレイヤのポジションを反映
	m_player->SetPosition(playerPos);

	// ブロックの上に当たっていたら重力を初期化
	if (is_boxCol.GetHitFace() == Collider::BoxCollider::HIT_FACE::UP)
	{
		m_player->ResetGravity();
	}

	// 処理が終わったら要素を破棄
	m_colObjList.pop_back();
}

//--------------------------------------------------------//
//デバッグ表示                                            //
//--------------------------------------------------------//
void PlayScene::DebugLog(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto state = GetSystemManager()->GetCommonStates().get();

	GetSystemManager()->GetString()->ChangeFontColor(DirectX::Colors::Black);

	// シーン名の表示
	GetSystemManager()->GetString()->DrawFormatString(state, { 0,0 }, L"PlayScene");

	// 文字数設定
	wchar_t cam[64];

	// カメラのポジション
	swprintf_s(cam, 64, L"CameraPos = %d,%d,%d",
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().x),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().y),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEyePosition().z)
	);
	GetSystemManager()->GetString()->DrawFormatString(state, { 0,20 }, cam);

	// ステージ番号確認
	wchar_t num[32];
	swprintf_s(num, 32, L"StageNum = %d", GetStageNum());

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,40 }, num);
	
	// プレイヤの座標
	wchar_t plr[64];
	swprintf_s(plr, 64, L"PlayerPosition = %f,%f,%f", m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,60 }, plr);
	
	// プレイヤの重力
	wchar_t gra[32];
	swprintf_s(gra, 32, L"Gravity = %f", m_player->GetGravity());

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,80 }, gra);
		
	// コインテスト
	wchar_t coi[32];
	swprintf_s(coi, 32, L"Coin = %d", test_count);

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,100 }, coi);

	// デバイスコンテキストの取得：グリッドの描画に使用
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	// デバッググリッドの表示
	GetSystemManager()->GetGridFloor()->Draw(context, state, view, proj);
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

	// メモリの解放
	m_map.ReleaseMemory();
	
	// 座標補正
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		m_mapObj[i].position.x -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		m_mapObj[i].position.y += static_cast<float>(COMMON_LOW);
		m_mapObj[i].position.z -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
	}
}