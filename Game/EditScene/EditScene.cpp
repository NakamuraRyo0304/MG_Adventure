/*
 *	@File	EditScene.cpp
 *	@Brief  エディットシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

// UI
#include "Objects/UserInterface.h"

#include "EditScene.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
EditScene::EditScene() :
	IScene(),
	m_userInterface{},				// UI
	m_sphere{},						// 球
	m_cursorPos{0.0f,0.0f,0.0f},
	m_mapObj{0},					// 格納配列
	m_nowState{},					// 現在のブロックの種類
	m_timer{},
	m_map{},						// マップ
	is_boxCol{},					// 立方体当たり判定
	m_grassModel{ nullptr },		// モデル
	m_noneModel{ nullptr },	
	m_coinModel{ nullptr },
	m_clowdModel{ nullptr },
	m_switchModel{ nullptr },
	m_sharedSystem{}
	
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
EditScene::~EditScene()
{
	m_map.~MapLoad();
	Finalize();
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
void EditScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// カメラ視点移動を有効にする
	GetSystemManager()->GetCamera()->SetEagleMode(m_userInterface->GetCameraFlag());

	// スフィアの初期化(テスト)
	m_sphere = GeometricPrimitive::CreateSphere(
		GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(), 
		COMMON_SIZE / 2
	);

	// マップ読み込み//初回読み込み
	LoadMap(L"Resources/Maps/Stage1.csv");

	// 初期値は草ブロック
	m_nowState = MapState::GrassBox;
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
// 第１引数：時間(60FPS = 1sec) / 第２引数：キーボードのポインタ / 第３引数：マウスのポインタ
void EditScene::Update(const float& elapsedTime, Keyboard::State& keyState,
	Mouse::State& mouseState)
{
	m_timer = elapsedTime;

	// キー入力情報を取得する
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();
	
	// レイの更新
	GetSystemManager()->GetRayCast()->Update(mouseState);

	// UIの処理
	m_userInterface->Update(mouseState);

	// セーブフラグがたったらファイルを保存
	if (m_userInterface->GetSaveFlag() && 
		GetSystemManager()->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		SaveFile();
	}

	// オープンフラグがたったらファイルを開く
	if (m_userInterface->GetOpenFlag() &&
		GetSystemManager()->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		m_map.LoadMap(L"");				// ファイル新規作成
		m_mapObj = m_map.GetMapData();	// 読み込み
		OffsetPosition_Read(&m_mapObj);	// 座標補正
	}

	// シフトを押している間はカメラモードをTrueにする
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::LeftShift))
	{
		// インターフェースでカメラのフラグを取得
		m_userInterface->SetCameraFlag(!m_userInterface->GetCameraFlag());
		GetSystemManager()->GetCamera()->SetEagleMode(m_userInterface->GetCameraFlag());
	}

	// ステータス変更
	if (GetSystemManager()->GetMouseTrack()->rightButton == Mouse::ButtonStateTracker::RELEASED)
	{
		switch (m_nowState)
		{
		case MapState::GrassBox:				// 草→コイン
			ChangeState(MapState::CoinBox);
			break;
		case MapState::CoinBox:					// コイン→スイッチ
			ChangeState(MapState::SwitchBox);
			break;
		case MapState::SwitchBox:				// スイッチ→消しゴム
			ChangeState(MapState::None);
			m_userInterface->SetDrawFlag(false);
			break;
		case MapState::None:					// 消しゴム→草
			ChangeState(MapState::GrassBox);
			m_userInterface->SetDrawFlag(true);
			break;
		default:
			break;
		}
	}
	// 消しゴムモードなら、選択ブロックを消しゴムにする
	if (!m_userInterface->GetDrawFlag())
	{
		ChangeState(MapState::None);
	}


	// カメラモードじゃなければ編集できる
	if (!m_userInterface->GetCameraFlag())
	{
		EditMap();
	}

	// ESCキーで終了
	if (keyState.Escape) ExitApp();

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		GoNextScene(SCENE::SELECT);
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void EditScene::Draw()
{
	// 描画関連
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	SimpleMath::Matrix world, view, projection;

	// ワールド行列
	world = SimpleMath::Matrix::Identity;

	// ビュー行列
	view = GetSystemManager()->GetCamera()->GetView();

	// プロジェクション行列
	projection = GetSystemManager()->GetCamera()->GetProjection();

	// レイの設定
	GetSystemManager()->GetRayCast()->SetMatrix(view, projection);

	// 行列計算
	SimpleMath::Matrix scale = SimpleMath::Matrix::CreateScale(COMMON_SIZE / 2);
	SimpleMath::Matrix rotateX = SimpleMath::Matrix::CreateRotationX(m_timer);
	SimpleMath::Matrix rotateY = SimpleMath::Matrix::CreateRotationY(m_timer);
	SimpleMath::Matrix rotateZ = SimpleMath::Matrix::CreateRotationZ(m_timer);
	SimpleMath::Matrix trans = SimpleMath::Matrix::CreateTranslation(m_cursorPos);

	// オブジェクトの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		SimpleMath::Matrix boxMat = 
			SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		if (m_mapObj[i].id == MapState::GrassBox)
		{
			m_grassModel->Draw(context, states, boxMat, view, projection);
		}
		if (m_mapObj[i].id == MapState::CoinBox)
		{
			m_coinModel->Draw(context, states, rotateY * boxMat, view, projection);
		}
		if (m_mapObj[i].id == MapState::SwitchBox)
		{
			m_switchModel->Draw(context, states, boxMat, view, projection);
		}
	}


	// サイズ　×　回転　×　移動
	world *= scale * rotateX * rotateY * rotateZ * trans;

	// 選択しているオブジェを描画 
	if (m_nowState == MapState::GrassBox)
	{
		m_grassModel->Draw(context, states, world, view, projection);
	}
	if (m_nowState == MapState::CoinBox)
	{
		m_coinModel->Draw(context, states, world, view, projection);
	}
	if (m_nowState == MapState::SwitchBox)
	{
		m_switchModel->Draw(context, states, world, view, projection);
	}
	if (m_nowState == MapState::None)
	{
		m_noneModel->Draw(context, states, world, view, projection);
	}



	// 画像の描画
	m_userInterface->Render();

	// デバッグ表示
	DebugLog(view, projection);
}

//--------------------------------------------------------//
//終了処理                                                //
//--------------------------------------------------------//
void EditScene::Finalize()
{
	// マップの解放
	m_mapObj.clear();

	// モデルの破棄
	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_noneModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_clowdModel);
	ModelFactory::DeleteModel(m_switchModel);
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

	// UIの初期化
	m_userInterface = std::make_unique<UserInterface>();
	m_sharedSystem = GetSystemManager();
	m_userInterface->Initialize(m_sharedSystem, context, device);

	// 画面サイズの格納
	float width  = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);
	
	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// 文字の設定
	GetSystemManager()->GetString()->CreateString(device, context);

	// レイが及ぶ範囲を設定
	GetSystemManager()->GetRayCast()->SetScreenSize(width, height);
	
	// モデルを作成する
	m_noneModel = ModelFactory::GetModel(					// 消しゴムブロック
		device,
		L"Resources/Models/Eraser.cmo"
	);
	m_grassModel = ModelFactory::GetModel(					// 草ブロック
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_coinModel = ModelFactory::GetModel(					// コインブロック
		device,
		L"Resources/Models/Coin.cmo"
	);
	m_clowdModel = ModelFactory::GetModel(					// 雲ブロック
		device,
		L"Resources/Models/Clowd.cmo"
	);
	m_switchModel = ModelFactory::GetModel(					// スイッチブロック
		device,
		L"Resources/Models/Switch.cmo"
	);
}

//--------------------------------------------------------//
//デバッグ表示                                            //
//--------------------------------------------------------//
void EditScene::DebugLog(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	GetSystemManager()->GetString()->ChangeFontColor(Colors::Black);

	// シーン名の表示
	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,0 }, L"EditScene");

	// 文字数設定
	wchar_t cam[64];

	// カメラのポジション
	swprintf_s(cam, 64, L"CameraPos = %d,%d,%d",
		static_cast<int>(GetSystemManager()->GetCamera()->GetEye().x),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEye().y),
		static_cast<int>(GetSystemManager()->GetCamera()->GetEye().z)
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
	swprintf_s(mos, 64, L"SpherePosition = %f,%f,%f",
		m_cursorPos.x,
		m_cursorPos.y,
		m_cursorPos.z);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,60 }, mos);

	auto mouse = Mouse::Get().GetState();
	// マウス位置確認
	wchar_t num[32];
	swprintf_s(num, 32, L"Mouse = %d:%d", mouse.x, mouse.y);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,80 }, num);
	
	// マウス位置確認
	wchar_t mw[32];
	swprintf_s(mw, 32, L"MouseWheel = %d", mouse.scrollWheelValue);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,100 }, mw);

	// 位置確認
	wchar_t bs[32];
	swprintf_s(bs, 32, L"BlockState = %d", m_nowState);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,120 }, bs);

	// 経過時間
	wchar_t sec[32];
	swprintf_s(sec, 32, L"Time = %f", m_timer);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,140 }, sec);

	// カメラの角度
	wchar_t ang[32];
	swprintf_s(ang, 32, L"CameraAngle = %f", GetSystemManager()->GetCamera()->GetCameraAngle().x);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,160 }, ang);



	// デバイスコンテキストの取得：グリッドの描画に使用
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	// デバッググリッドの表示
	GetSystemManager()->GetGridFloor()->Draw(context, GetSystemManager()->GetCommonStates().get(), view, proj);
}

//--------------------------------------------------------//
//描画ボックスのステータスを変更する                      //
//--------------------------------------------------------//
void EditScene::ChangeState(const int& State)
{
	m_nowState = State;
}

//--------------------------------------------------------//
//マップを編集する                                        //
//--------------------------------------------------------//
void EditScene::EditMap()
{
	auto mouse = Mouse::Get().GetState();

	// カメラモードは処理しない
	if (m_userInterface->GetCameraFlag()) return;

	// 移動処理
	m_cursorPos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_cursorPos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;

	// マウスカーソルで移動
	m_cursorPos.y = UserUtillity::Lerp(
		m_cursorPos.y,															// 開始地点
		static_cast<float>(mouse.scrollWheelValue / WHEEL_SPAWN) + COMMON_LOW,	// 終了地点
		0.1f																	// 速度
	);

	// 制限をつける
	m_cursorPos.y = UserUtillity::Clamp(m_cursorPos.y, -2.0f, 15.0f);

	// マップとの当たり判定
	for (auto& i : m_mapObj)
	{
		// 押し戻し処理を無効化
		is_boxCol.SetPushMode(false);

		// 当たり判定を取る
		is_boxCol.PushBox(&m_cursorPos, i.position,
			SimpleMath::Vector3{ COMMON_SIZE / 2 },
			SimpleMath::Vector3{ COMMON_SIZE });

		// 瞬間の当たり判定を取得
		bool hit = is_boxCol.GetHitBoxFlag();
		i.hit = hit;

		if (hit && GetSystemManager()->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
		{
			i.id = m_nowState;
		}
	}
}

//--------------------------------------------------------//
//座標の補正                                              //
//--------------------------------------------------------//
// 読み込み時
void EditScene::OffsetPosition_Read(std::vector<Object>* obj)
{
	for (auto& i : *obj)
	{
		i.position.x -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		i.position.y += static_cast<float>(COMMON_LOW);
		i.position.z -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
	}
}
// 書き込み時
void EditScene::OffsetPosition_Write(std::vector<Object>* obj)
{
	for (auto& i : *obj)
	{
		i.position.x += static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		i.position.y -= static_cast<float>(COMMON_LOW);
		i.position.z += static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
	}
}

//--------------------------------------------------------//
//マップ読み書き                                          //
//--------------------------------------------------------//
// 読み込み時
void EditScene::LoadMap(std::wstring filename)
{ 
	// マップの読み込み
	m_map.LoadMap(filename);

	// パスが取得できなければ新しく平面のマップを作成する
	m_filePath = m_map.GetFilePath();

	// マップを格納する
	m_mapObj = m_map.GetMapData();

	// 座標補正
	OffsetPosition_Read(&m_mapObj);
}
// 書き込み時
void EditScene::SaveFile()
{	
	// ファイル書き出し
	OffsetPosition_Write(&m_mapObj);		// 書き出し用に座標補正
	m_map.WriteMap(m_mapObj);				// ファイルの書き出し
	OffsetPosition_Read(&m_mapObj);			// エディット用に座標補正
}