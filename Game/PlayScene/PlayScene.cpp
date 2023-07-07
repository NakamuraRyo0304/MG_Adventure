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

// シェーダー
#include "Objects/PlayerBill.h"

// プレイヤクラス
#include "Objects/Player.h"

#include "PlayScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
PlayScene::PlayScene() :
	IScene(),
	m_timer{0.0f},
	m_mapObj{},						// マップのブロック
	m_map{},						// マップ
	m_colObjList{},					// 当っているオブジェクトの格納
	is_boxCol{},					// 立方体当たり判定
	m_grassModel{ nullptr },		// 草ブロックのモデル
	m_coinModel{ nullptr },			// コインブロックのモデル
	m_clowdModel{ nullptr },		// 雲ブロックのモデル
	m_skyDomeModel{ nullptr },		// スカイドームモデル
	m_coinCount{0},					// 獲得コイン
	m_maxCoins{}					// 最大コイン
{

}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
PlayScene::~PlayScene()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// カメラ視点移動
	GetSystemManager()->GetCamera()->SetEagleMode(true);	
	
	// プレイヤの初期化
	m_player->Initialize(std::make_shared<SystemManager>());

	// マップ読み込み
	LoadMap(GetStageNum());

	// 判定の初期化
	m_colObjList.clear();
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">時間/fps</param>
/// <param name="keyState">キーボードポインタ</param>
/// <param name="mouseState">マウスポインタ</param>
/// <returns>なし</returns>
void PlayScene::Update(const float& elapsedTime, Keyboard::State& keyState,
	Mouse::State& mouseState)
{
	m_timer = elapsedTime;

	// キー入力情報を取得する
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// 相対移動
	m_playerBill->SetVertexMovePos(SimpleMath::Vector3{m_player->GetPosition().x,m_player->GetPosition().y + 2.0f,m_player->GetPosition().z});

	// コインをすべて獲得でリザルト
	if (m_coinCount == m_maxCoins)
	{
		GoNextScene(SCENE::RESULT);
		return;
	}

	// プレイヤの更新
	m_player->Update(keyState, elapsedTime);

	// 当たり判定の更新
	DoBoxCollision();

	// 衝突したオブジェクトごとに押し戻し処理を行う
	for (auto& obj : m_colObjList)
	{
		ApplyPushBack(obj);
	}
	
	// 落下したらリスタート（仮）
	if (m_player->GetDeathFlag())
	{
		GoNextScene(SCENE::PLAY);
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayScene::Draw()
{
	// 描画関連
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	SimpleMath::Matrix world, view, proj;

	// ワールド行列
	world = SimpleMath::Matrix::Identity;

	// ビュー行列
	view = GetSystemManager()->GetCamera()->GetView();

	// プロジェクション行列
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// プレイヤの描画
	m_player->Render(context, states, view, proj);

	// 回転行列
	SimpleMath::Matrix rotateY = SimpleMath::Matrix::CreateRotationY(m_timer);

	// オブジェクトの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		SimpleMath::Matrix boxMat =
			SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		if (m_mapObj[i].id == MapLoad::BoxState::GrassBox)
		{
			m_grassModel->Draw(context, states, boxMat, view, proj);
		}
		if (m_mapObj[i].id == MapLoad::BoxState::CoinBox)
		{
			m_coinModel->Draw(context, states, rotateY * boxMat, view, proj);
		}
		if (m_mapObj[i].id == MapLoad::BoxState::SwitchBox)
		{
			m_coinModel->Draw(context, states, rotateY * boxMat, view, proj);
		}
	}

	// スカイドームの描画
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);


	// ビルボード作成
	m_playerBill->CreateBillboard(
		GetSystemManager()->GetCamera()->GetTargetPosition(),	// カメラの注視点
		GetSystemManager()->GetCamera()->GetEye(),				// カメラの座標
		SimpleMath::Vector3::Up
	);
	// 描画
	m_playerBill->Render(m_player->GetPosition(), m_timer, view, proj);

	DebugLog(view, proj);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayScene::Finalize()
{
	// プレイヤの後処理
	m_player->Finalize();

	// マップの解放
	m_mapObj.clear();

	// 判定用配列を解放
	m_colObjList.clear();

	// モデルの解放
	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_clowdModel);
}

/// <summary>
/// 画面依存、デバイス依存の初期化
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
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
	m_grassModel = ModelFactory::GetCreateModel(						// 草ブロック
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_coinModel = ModelFactory::GetCreateModel(							// コインブロック
		device,
		L"Resources/Models/Coin.cmo"
	);
	m_clowdModel = ModelFactory::GetCreateModel(						// 雲ブロック
		device,
		L"Resources/Models/Clowd.cmo"
	);
	m_skyDomeModel = ModelFactory::GetCreateModel(						// スカイドーム
		device,
		L"Resources/Models/ShineSky.cmo"
	);
	m_skyDomeModel->UpdateEffects([](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			// 自己発光する
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	// モデルデータを渡す(move必要)
	m_player = std::make_unique<Player>(
		std::move(ModelFactory::GetCreateModel(device,L"Resources/Models/CatClean.cmo"))
	);

	// 位置情報のシェーダーの作成
	m_playerBill = std::make_unique<PlayerBill>();
	m_playerBill->Create(GetSystemManager()->GetDeviceResources());
}

/// <summary>
/// 当たっていたらリスト追加
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayScene::DoBoxCollision()
{
	// 衝突したオブジェクトリストを初期化
	m_colObjList.clear(); 

	// 当たり判定を取る
	for (auto& obj : m_mapObj)
	{
		// プレイヤの半径2.0fの範囲になければ処理しない
		if (UserUtility::CheckPointInSphere(
			m_player->GetPosition(),			// 中心点
			COMMON_SIZE * 2,					// 半径
			obj.position))						// あるか調べる点
		{

			// 判定を取る コインのみ判定を小さくする
			if (obj.id == MapLoad::BoxState::CoinBox)
			{
				is_boxCol.PushBox(
					m_player->GetPosition(),								// プレイヤ座標
					obj.position,											// コイン座標
					SimpleMath::Vector3{ m_player->GetSize() },				// プレイヤサイズ
					SimpleMath::Vector3{ COMMON_SIZE / 3 }					// コインサイズ
				);
			}
			else
			{
				is_boxCol.PushBox(
					m_player->GetPosition(),								// プレイヤ座標
					obj.position,											// ブロック座標
					SimpleMath::Vector3{ m_player->GetSize() },				// プレイヤサイズ
					SimpleMath::Vector3{ COMMON_SIZE }						// ブロックサイズ
				);
			}

			// 当たっていたら処理する
			if (is_boxCol.GetHitBoxFlag())
			{
				// 衝突したオブジェクトをリストに追加
				m_colObjList.push_back(obj);
			}
		}
	}
}

/// <summary>
/// 押し戻し処理
/// </summary>
/// <param name="obj">当たったオブジェクト</param>
/// <returns>なし</returns>
void PlayScene::ApplyPushBack(Object& obj)
{
	// 当っているオブジェが空気の場合は処理しない
	if (obj.id == MapLoad::BoxState::None)
	{
		is_boxCol.SetPushMode(false);
		return;
	}

	// 空気以外の場合は押し戻しを有効にする
	if (obj.id != MapLoad::BoxState::None)
	{
		is_boxCol.SetPushMode(true);
	}

	// コインの処理
	if (obj.id == MapLoad::BoxState::CoinBox)
	{ 
		// 該当コインの検索＆獲得処理
		for (auto& i : m_mapObj)
		{
			if (i == obj)
			{
				m_coinCount += 1;				// 獲得数加算
				i.id = MapLoad::BoxState::None;	// 獲得したらコインを削除
			}
		}
		return;
	}

	// プレイヤのポジションを保存
	SimpleMath::Vector3 playerPos = m_player->GetPosition();

	// 当たり判定を取って押し戻す
	is_boxCol.PushBox(
		&playerPos,												// プレイヤ座標
		obj.position,											// ブロック座標
		SimpleMath::Vector3{ m_player->GetSize() },				// プレイヤサイズ
		SimpleMath::Vector3{ COMMON_SIZE }						// ブロックサイズ
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

/// <summary>
/// デバッグログ
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <returns>なし</returns>
void PlayScene::DebugLog(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto state = GetSystemManager()->GetCommonStates().get();

	GetSystemManager()->GetString()->ChangeFontColor(Colors::Black);

	// シーン名の表示
	GetSystemManager()->GetString()->DrawFormatString(state, { 0,0 }, L"PlayScene");

	// 文字数設定
	wchar_t cam[64];

	// カメラのポジション
	swprintf_s(cam, 64, L"CameraPos = %d,%d,%d",
		static_cast<int>(GetSystemManager()->GetCamera()->GetEye().x),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEye().y),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEye().z)
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
	swprintf_s(coi, 32, L"Coin = %d", m_coinCount);

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,100 }, coi);

	// デバイスコンテキストの取得：グリッドの描画に使用
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	// デバッググリッドの表示
	GetSystemManager()->GetGridFloor()->Draw(context, state, view, proj);
}

/// <summary>
/// マップ読み込み
/// </summary>
/// <param name="num">ステージ番号</param>
/// <returns>なし</returns>
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

		// コインの枚数のカウント
		if (m_mapObj[i].id == MapLoad::BoxState::CoinBox)
		{
			m_maxCoins++;
		}
		// プレイヤの座標を代入
		if (m_mapObj[i].id == MapLoad::BoxState::PlayerPos)
		{
			m_player->SetPosition(SimpleMath::Vector3
				{
					m_mapObj[i].position.x, 
					m_mapObj[i].position.y + COMMON_SIZE / 2,
					m_mapObj[i].position.z 
				}
			);
			// 代入後に該当マスを空気に変える(判定除去)
			m_mapObj[i].id = MapLoad::BoxState::None;
		}
	}
}