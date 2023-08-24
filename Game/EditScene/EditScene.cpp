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

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
EditScene::EditScene() :
	IScene(),
	m_timer{0.0f},					// タイマー
	m_cursorPos{0.0f,0.0f,0.0f},	// カーソルの位置
	m_mapObj{0},					// 格納配列
	m_nowState{},					// 現在のブロックの種類
	m_map{},						// マップ
	is_boxCol{},					// 立方体当たり判定
	m_grassModel{ nullptr },		// モデル＿草
	m_noneModel{ nullptr },			// モデル＿消しゴム
	m_coinModel{ nullptr },			// モデル＿コイン
	m_clowdModel{ nullptr },		// モデル＿雲
	m_resetPtModel{ nullptr },		// モデル＿リセットポイント
	m_skyDomeModel{ nullptr },		// モデル＿スカイドーム
	m_sharedSystem{}				// システムデータ
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
EditScene::~EditScene()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void EditScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// カメラ視点移動を有効にする
	GetSystemManager()->GetCamera()->SetEagleMode(m_userInterface->GetCameraFlag());

	// マップ読み込み//初回読み込み
	LoadMap(L"Resources/Maps/StageEdit.csv");

	// 初期値は草ブロック
	m_nowState = MapState::GrassBox;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">時間/fps</param>
/// <param name="keyState">キーボードポインタ</param>
/// <param name="mouseState">マウスポインタ</param>
/// <returns>なし</returns>
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

	// 選択しているオブジェクトを格納
	m_nowState = m_userInterface->GetNowState();

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
		m_map.LoadMap(L"");						// ファイル新規作成
		m_mapObj = m_map.GetMapData();			// 読み込み
		OffsetPosition(&m_mapObj,READ);			// 座標補正
	}

	// シフトを押したら、カメラモードを反転する
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::LeftShift))
	{
		// インターフェースでカメラのフラグを取得
		m_userInterface->SetCameraFlag(!m_userInterface->GetCameraFlag());
		GetSystemManager()->GetCamera()->SetEagleMode(m_userInterface->GetCameraFlag());
	}

	// カメラモードじゃなければ編集できる
	if (m_userInterface->GetCameraFlag() == false)
	{
		EditMap();
	}

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		ChangeScene(SCENE::SELECT);
	}

	// エスケープで終了
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape))
	{
		ExitApp();
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void EditScene::Draw()
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

	// レイの設定
	GetSystemManager()->GetRayCast()->SetMatrix(view, proj);

	// 行列計算
	SimpleMath::Matrix scale = SimpleMath::Matrix::CreateScale(COMMON_SIZE / 2);
	SimpleMath::Matrix rotY  = SimpleMath::Matrix::CreateRotationY(m_timer);
	SimpleMath::Matrix trans = SimpleMath::Matrix::CreateTranslation(m_cursorPos);

	// サイズ　×　回転　×　移動
	world *= scale *  rotY * trans;

	// オブジェクトの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		SimpleMath::Matrix boxMat =
			SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		// 押し戻し処理を無効化
		is_boxCol.SetPushMode(false);

		// 当たり判定を取る
		is_boxCol.PushBox(&m_cursorPos, m_mapObj[i].position,
			SimpleMath::Vector3{ COMMON_SIZE / 2 },
			SimpleMath::Vector3{ COMMON_SIZE }
		);

		m_mapObj[i].hit = is_boxCol.IsHitBoxFlag();

		if (m_mapObj[i].hit) // 選択中のマスにオブジェクトを描画
		{
			SwitchDraw(m_nowState, context, &states, boxMat, view, proj);
		}
		else				 // 該当オブジェクトの描画
		{
			SwitchDraw(m_mapObj[i].id, context, &states, boxMat, view, proj);
		}
	}

	// マウス位置に描画
	if (m_nowState == MapState::None) // 削除時以外は通常の描画
	{
		m_noneModel->Draw(context, states, world, view, proj);
	}
	else
	{
		SwitchDraw(m_nowState, context, &states, world, view, proj);
	}

	// スカイドームの描画
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * 0.1f);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);

	// 画像の描画
	m_userInterface->Render();
}

/// <summary>
/// 描画オブジェクト切り替え
/// </summary>
/// <param name="objNum">オブジェクト番号</param>
/// <param name="context">デバイスコンテキスト</param>
/// <param name="states">コモンステート</param>
/// <param name="world">ワールド行列</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <returns>なし</returns>
void EditScene::SwitchDraw(const int& objNum, ID3D11DeviceContext* context,	CommonStates* states,
	SimpleMath::Matrix world, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// 行列計算
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_timer);

	switch (objNum)
	{
	case MapState::GrassBox:	// 草
		m_grassModel->Draw(context, *states, world, view, proj);
		break;
	case MapState::CoinBox:		// コイン
		m_coinModel->Draw(context, *states, rotY * world, view, proj);
		break;
	case MapState::ClowdBox:	// 雲
		m_clowdModel->Draw(context, *states, world, view, proj);
		break;
	case MapState::ResetClowd:	// 雲リセット
		m_resetPtModel->Draw(context, *states, world, view, proj);
		break;
	case MapState::PlayerPos:	// プレイヤー
		m_playerModel->Draw(context, *states, rotY * world, view, proj);
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
void EditScene::Finalize()
{
	// マップの解放
	m_mapObj.clear();

	// モデルの破棄
	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_clowdModel);
	ModelFactory::DeleteModel(m_resetPtModel);
	ModelFactory::DeleteModel(m_playerModel);
	ModelFactory::DeleteModel(m_skyDomeModel);
	ModelFactory::DeleteModel(m_noneModel);
}

/// <summary>
/// 画面依存、デバイス依存の初期化
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void EditScene::CreateWindowDependentResources()
{
	// デバイスとデバイスコンテキストの取得
	auto device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique(device, context);

	// 画面サイズの格納
	float width = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// UIの初期化
	m_userInterface = std::make_unique<UserInterface>(SimpleMath::Vector2(width, height));
	m_sharedSystem = GetSystemManager();
	m_userInterface->Initialize(m_sharedSystem, context, device);

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// 文字の設定
	GetSystemManager()->GetString()->CreateString(device, context);

	// レイが及ぶ範囲を設定
	GetSystemManager()->GetRayCast()->SetScreenSize(width, height);

	// モデルを作成する
	m_grassModel = ModelFactory::GetCreateModel(		// 草ブロック
		device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_coinModel = ModelFactory::GetCreateModel(			// コインブロック
		device,
		L"Resources/Models/Coin.cmo"
	);
	m_clowdModel = ModelFactory::GetCreateModel(		// 雲ブロック
		device,
		L"Resources/Models/MoveBlock.cmo"
	);
	m_resetPtModel = ModelFactory::GetCreateModel(		// リセットブロック
		device,
		L"Resources/Models/ResetPt.cmo"
	);
	m_playerModel = ModelFactory::GetCreateModel(		// プレイヤブロック
		device,
		L"Resources/Models/Bird.cmo"
	);
	m_noneModel = ModelFactory::GetCreateModel(			// 消しゴムブロック
		device,
		L"Resources/Models/Eraser.cmo"
	);

	// スカイドームモデルを作成する
	m_skyDomeModel = ModelFactory::GetCreateModel(
		device,
		L"Resources/Models/Spacedome.cmo"
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
}

/// <summary>
/// マップの編集
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void EditScene::EditMap()
{
	auto mouse = Mouse::Get().GetState();

	// カメラモードは処理しない
	if (m_userInterface->GetCameraFlag()) return;

	// 移動処理
	m_cursorPos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_cursorPos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;

	// マウスカーソルで移動
	m_cursorPos.y = UserUtility::Lerp(
		m_cursorPos.y,															// 開始地点
		static_cast<float>(mouse.scrollWheelValue / WHEEL_SPAWN) + COMMON_LOW,	// 終了地点
		0.1f																	// 速度
	);

	// 制限をつける
	m_cursorPos.y = UserUtility::Clamp(m_cursorPos.y, CURSOR_MIN, CURSOR_MAX);

	// マップとの当たり判定
	for (auto& obj : m_mapObj)
	{
		// 押し戻し処理を無効化
		is_boxCol.SetPushMode(false);

		// 当たり判定を取る
		is_boxCol.PushBox(&m_cursorPos, obj.position,
			SimpleMath::Vector3{ COMMON_SIZE / 2 },
			SimpleMath::Vector3{ COMMON_SIZE }
		);

		// 瞬間の当たり判定を取得
		obj.hit = is_boxCol.IsHitBoxFlag();

		// クリックでブロック設置
		if (obj.hit && GetSystemManager()->GetMouseTrack()->leftButton ==
			Mouse::ButtonStateTracker::RELEASED)
		{
			obj.id = m_nowState;
		}
	}
}

/// <summary>
/// 座標補正
/// </summary>
/// <param name="object">マップデータ</param>
/// <param name="mode">読み書きモード</param>
/// <returns>なし</returns>
void EditScene::OffsetPosition(std::vector<Object>* object, const int& mode)
{
	// 読み込み
	if (mode == READ)
	{
		for (auto& obj : *object)
		{
			obj.position.x -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
			obj.position.y += static_cast<float>(COMMON_LOW);
			obj.position.z -= static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		}
	}
	// 書き込み
	if (mode == WRITE)
	{
		for (auto& i : *object)
		{
			i.position.x += static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
			i.position.y -= static_cast<float>(COMMON_LOW);
			i.position.z += static_cast<float>(m_map.MAP_COLUMN) / 2 * COMMON_SIZE;
		}
	}
}

/// <summary>
/// マップ読み込み
/// </summary>
/// <param name="filename">ファイルパス</param>
/// <returns>なし</returns>
void EditScene::LoadMap(std::wstring filename)
{
	// マップの読み込み
	m_map.LoadMap(filename);

	// パスが取得できなければ新しく平面のマップを作成する
	m_filePath = m_map.GetFilePath();

	// マップを格納する
	m_mapObj = m_map.GetMapData();

	// 座標補正
	OffsetPosition(&m_mapObj,READ);
}

/// <summary>
/// マップ書き出し
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void EditScene::SaveFile()
{
	// ファイル書き出し
	OffsetPosition(&m_mapObj,WRITE);	// 書き出し用に座標補正
	m_map.WriteMap(m_mapObj);			// ファイルの書き出し
	OffsetPosition(&m_mapObj,READ);		// 読み込み用に座標補正
}