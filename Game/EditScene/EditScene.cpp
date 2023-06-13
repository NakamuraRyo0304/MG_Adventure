/*
 *	@File	EditScene.cpp
 *	@Brief  エディットシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "EditScene.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
EditScene::EditScene() :
	IScene(),
	m_sphere{},						// 球
	m_spherePos{0.0f,0.0f,0.0f},
	m_box{},						// 箱
	m_mapObj{},						// 格納配列
	m_nowState{},					// 現在のブロックの種類
	m_map{},						// マップ
	is_boxCol{},						// 立方体当たり判定
	m_grassBlockModel{ nullptr },	// モデル
	m_grassBlockModel_D{ nullptr },	
	m_coinModel{ nullptr },
	m_clowdModel{ nullptr },
	m_saveTexPos{},					// 画像座標
	m_cameraTexPos{},
	m_penTexPos{},
	is_saveFlag{},					// 選択フラグ
	is_cameraFlag{true},			// カメラモードONでスタート
	is_upFlag{true}					// ペンモードでスタート			
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

	GetSystemManager()->GetCamera()->SetMoveMode(false);				// カメラ座標移動
	GetSystemManager()->GetCamera()->SetEagleMode(is_cameraFlag);		// カメラ視点移動

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

	// マップ読み込み//初回読み込み
	LoadMap(L"Resources/Maps/Stage1.csv");

	// 初期値は草ブロック
	m_nowState = MapLoad::BoxState::GrassBox;
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

	// UIの処理
	ClickUserInterface(mouseState);

	// ファイルの読み込み
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Left))
	{
		m_map.CreateNewMap();			// ファイル新規作成
		m_mapObj = m_map.GetMapData();	// 読み込み
		OffsetPosition_Read(&m_mapObj);	// 座標補正
	}

	// ステータス変更
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Right))
	{
		switch (m_nowState)
		{
		case MapLoad::BoxState::GrassBox:				// 草→コイン
			ChangeState(MapLoad::BoxState::CoinBox);
			break;
		case MapLoad::BoxState::CoinBox:				// コイン→消しゴム
			ChangeState(MapLoad::BoxState::None);
			break;
		case MapLoad::BoxState::None:					// 消しゴム→草
			ChangeState(MapLoad::BoxState::GrassBox);
			break;
		default:
			break;
		}
	}

	// カメラモードじゃなければ編集できる
	if (!is_cameraFlag)
	{
		EditMap();
	}

	// ESCキーで終了
	if (keyState.Escape) ExitApp();

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(DirectX::Keyboard::Space))
	{
		GoNextScene(SCENE::SELECT);
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

	// 非カメラモードのみ描画
	if (!is_cameraFlag)
	{
		m_sphere->Draw(world, view, projection, DirectX::Colors::Black);
	}
		
	// オブジェクトの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		DirectX::SimpleMath::Matrix boxMat = 
			DirectX::SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		if (m_mapObj[i].id == MapLoad::BoxState::GrassBox)
		{
			m_grassBlockModel->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
				*GetSystemManager()->GetCommonStates(), boxMat, view, projection);
		}
		if (m_mapObj[i].id == MapLoad::BoxState::CoinBox)
		{
			m_coinModel->Draw(GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext(),
				*GetSystemManager()->GetCommonStates(), boxMat, view, projection);
		}
	}

	// 画像の描画
	DrawImages();

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
	m_grassBlockModel = ModelFactory::GetModel(					// 草ブロック
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_grassBlockModel_D = ModelFactory::GetModel(				// 草選択ブロック
		device,
		L"Resources/Models/GrassBlock_Dark.cmo"
	);
	m_coinModel = ModelFactory::GetModel(						// コインブロック
		device,
		L"Resources/Models/Coin.cmo"

	);
	m_clowdModel = ModelFactory::GetModel(						// 雲ブロック
		device,
		L"Resources/Models/Clowd.cmo"

	);

	// 画像の設定
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch(context);
	// キー名　：　ファイルパス名　：　デバイス
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Save", L"Resources/Textures/SaveFile.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Camera", L"Resources/Textures/Camera.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"CameraMove", L"Resources/Textures/CameraMove.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Pen", L"Resources/Textures/AddBlock.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Erase", L"Resources/Textures/EraseBlock.dds", device);

	// 座標情報
	m_saveTexPos   = { width - 100, 80 };
	m_cameraTexPos = { width - 100,208 };
	m_penTexPos    = { width - 100,336 };
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
	swprintf_s(mos, 64, L"SpherePosition = %f,%f,%f",
		m_spherePos.x,
		m_spherePos.y,
		m_spherePos.z);

	GetSystemManager()->GetString()->DrawFormatString(GetSystemManager()->GetCommonStates().get(), { 0,60 }, mos);

	auto mouse = DirectX::Mouse::Get().GetState();
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
	auto mouse = DirectX::Mouse::Get().GetState();

	// 移動処理
	m_spherePos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_spherePos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;

	// カメラモードは処理しない
	if (is_cameraFlag) return;

	// マウスカーソルで移動
	m_spherePos.y = UserUtillity::Lerp(m_spherePos.y, mouse.scrollWheelValue / 640.0f + COMMON_LOW, 0.1f);

	// マップとの当たり判定
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		// 押し戻し処理を無効化
		is_boxCol.SetPushMode(false);

		// 当たり判定を取る
		is_boxCol.PushBox(&m_spherePos, m_mapObj[i].position,
			DirectX::SimpleMath::Vector3{ COMMON_SIZE / 2 },
			DirectX::SimpleMath::Vector3{ COMMON_SIZE });
		bool hit = is_boxCol.GetHitBoxFlag();
		
		if (hit && GetSystemManager()->GetMouseTrack()->leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED)
		{
			m_mapObj[i].id = m_nowState;
		}
	}
}

//--------------------------------------------------------//
//座標の補正                                              //
//--------------------------------------------------------//
// 読み込み時
void EditScene::OffsetPosition_Read(std::vector<Object>* obj)
{
	for (int i = 0; i < (*obj).size(); i++)
	{
		(*obj)[i].position.x -= (m_map.MAP_COLUMN / 2 * COMMON_SIZE);
		(*obj)[i].position.y += COMMON_LOW;
		(*obj)[i].position.z -= (m_map.MAP_COLUMN / 2 * COMMON_SIZE);
	}
}
// 書き込み時
void EditScene::OffsetPosition_Write(std::vector<Object>* obj)
{
	for (int i = 0; i < (*obj).size(); i++)
	{
		(*obj)[i].position.x += (m_map.MAP_COLUMN / 2 * COMMON_SIZE);
		(*obj)[i].position.y -= COMMON_LOW;
		(*obj)[i].position.z += (m_map.MAP_COLUMN / 2 * COMMON_SIZE);
	}
}

//--------------------------------------------------------//
//マップ読み込み                                          //
//--------------------------------------------------------//
void EditScene::LoadMap(std::wstring filename)
{
	// マップの読み込み
	m_map.LoadMap(filename);
	m_filePath = m_map.GetFilePath();
	m_mapObj = m_map.GetMapData();	// 読み込み
	OffsetPosition_Read(&m_mapObj);	// 座標補正
}

//--------------------------------------------------------//
//ファイルを書きだす                                      //
//--------------------------------------------------------//
void EditScene::SaveFile()
{	
	// ファイル書き出し
	OffsetPosition_Write(&m_mapObj);		// 書き出し用に座標補正
	m_map.WriteMap(m_mapObj);				// ファイルの書き出し
	OffsetPosition_Read(&m_mapObj);			// エディット用に座標補正
}

//--------------------------------------------------------//
//画像の描画                                              //
//--------------------------------------------------------//
void EditScene::DrawImages()
{
	// ファイルアイコン
	if (is_saveFlag)
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Save",							// 登録キー
			m_saveTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			0.5f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Save",							// 登録キー
			m_saveTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,0.3f },			// 色
			0.5f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// カメラアイコン
	if (is_cameraFlag)
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"CameraMove",						// 登録キー
			m_cameraTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			0.5f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Camera",							// 登録キー
			m_cameraTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,0.3f },			// 色
			0.5f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}

	// ペンアイコン
	if (is_upFlag)
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Pen",								// 登録キー
			m_penTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			0.5f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
	else
	{
		GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Erase",							// 登録キー
			m_penTexPos,						// 座標
			{ 1.0f,1.0f,1.0f,1.0f },			// 色
			0.5f,								// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }		// 中心位置
		);
	}
}

//--------------------------------------------------------//
//UIのクリック                                            //
//--------------------------------------------------------//
void EditScene::ClickUserInterface(DirectX::Mouse::State& mouseState)
{
	// 保存アイコンをクリック
	is_saveFlag = m_aabbCol.HitAABB_2D({ (float)mouseState.x,0.0f,(float)mouseState.y },// マウスの位置
		{ m_saveTexPos.x,0,m_saveTexPos.y },		        // 画像の位置
		DirectX::SimpleMath::Vector3{ 5.0f },		    // サイズ
		DirectX::SimpleMath::Vector3{ 100.0f });	    // サイズ
	// 当っていてクリックした時処理
	if (is_saveFlag && GetSystemManager()->GetMouseTrack()->leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED)
	{
		is_saveFlag = true;
		// ダイアログを表示してマップを出力
		SaveFile();
	}

	// カメラアイコンをクリック
	bool cameraFlag = m_aabbCol.HitAABB_2D({ (float)mouseState.x,0.0f,(float)mouseState.y },// マウスの位置
		{ m_cameraTexPos.x,0,m_cameraTexPos.y },	 	// 画像の位置
		DirectX::SimpleMath::Vector3{ 5.0f },		    // サイズ
		DirectX::SimpleMath::Vector3{ 100.0f });	    // サイズ

	// カメラ移動モード切り替え
	if (cameraFlag && GetSystemManager()->GetMouseTrack()->leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED)
	{
		is_cameraFlag = !is_cameraFlag;
		auto camera = GetSystemManager()->GetCamera();
		camera->SetEagleMode(is_cameraFlag);
	}

	// ペン/消しゴムアイコンをクリック
	bool toolFlag = m_aabbCol.HitAABB_2D({ (float)mouseState.x,0.0f,(float)mouseState.y },// マウスの位置
		{ m_penTexPos.x,0,m_penTexPos.y },	        	// 画像の位置
		DirectX::SimpleMath::Vector3{ 5.0f },		    // サイズ
		DirectX::SimpleMath::Vector3{ 100.0f });	    // サイズ

	// 描画モード切り替え
	if (toolFlag && GetSystemManager()->GetMouseTrack()->leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED)
	{
		is_upFlag = !is_upFlag;
	}
}
