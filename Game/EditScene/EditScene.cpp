/*
 *	@File	EditScene.cpp
 *	@Brief  エディットシーン。
 *	@Date	2023-09-24
 *  @Author NakamuraRyo
 */

#include "pch.h"

// UI
#include "Objects/EditUI.h"

// マウスカーソル
#include "../../Libraries/SystemDatas/MouseCursor.h"

// クリア難易度チェッカー
#include "System/ClearChecker.h"

#include "EditScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
EditScene::EditScene()
	: IScene()									// 基底クラスの初期化
	, m_timer{ 0.0f }							// タイマー
	, m_mapLoader{}								// マップローダー
	, m_mapObj{0}								// 格納配列
	, m_nowState{}								// 現在のブロックの種類
	, m_grassModel{ nullptr }					// モデル＿草
	, m_noneModel{ nullptr }					// モデル＿消しゴム
	, m_coinModel{ nullptr }					// モデル＿コイン
	, m_cloudModel{ nullptr }					// モデル＿雲
	, m_gravityModel{ nullptr }					// モデル＿重力
	, m_skyDomeModel{ nullptr }					// モデル＿スカイドーム
	, m_cursorPos{ SimpleMath::Vector3::Zero }	// カーソルの位置
	, is_hitCol{}								// 立方体当たり判定
{
	ShowCursor(false);
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
EditScene::~EditScene()
{
	Finalize();
	ShowCursor(true);
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

	// 変数の初期化
	SetSceneValues();
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

	// マウスカーソルの位置を更新
	m_mouseCursor->Update(SimpleMath::Vector2{ static_cast<float>(mouseState.x),static_cast<float>(mouseState.y) });

	// エスケープで終了
	GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape) ? ChangeScene(SCENE::ENDGAME) : void();

	// 選択しているオブジェクトを格納
	m_nowState = m_userInterface->GetNowState();

	// セーブフラグがたったらファイルを保存
	if (m_userInterface->GetSaveFlag() &&
		GetSystemManager()->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		// 要素チェックして保存可能なら実行
		if (IsCanSave())
		{
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
			SaveFile();
		}
	}

	// オープンフラグがたったらファイルを開く
	if (m_userInterface->GetOpenFlag() &&
		GetSystemManager()->GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		if (!m_mapLoader.LoadMap(L""))
		{
			return;
		}
		m_mapObj = m_mapLoader.GetMapData();	// 読み込み
		OffsetPosition(&m_mapObj,READ);			// 座標補正
	}

	// Zキーを押したら、カメラモードを反転する
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Z))
	{
		// インターフェースでカメラのフラグを取得
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		m_userInterface->SetCameraFlag(!m_userInterface->GetCameraFlag());
		GetSystemManager()->GetCamera()->SetEagleMode(m_userInterface->GetCameraFlag());
	}

	// カメラモードじゃなければ編集できる
	if (m_userInterface->GetCameraFlag() == false)
	{
		EditMap();
	}

	// ボタンクリックでセレクトに戻る
	if (m_userInterface->GetBackSelectFlag())
	{
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		ChangeScene(SCENE::SELECT);
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
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& _states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	SimpleMath::Matrix _cursorMat, _view, _projection;

	// ビュー行列
	_view = GetSystemManager()->GetCamera()->GetView();

	// プロジェクション行列
	_projection = GetSystemManager()->GetCamera()->GetProjection();

	// レイの設定
	GetSystemManager()->GetRayCast()->SetMatrix(_view, _projection);

	// 行列計算
	SimpleMath::Matrix _scale	 = SimpleMath::Matrix::CreateScale(COMMON_SIZE / 2);
	SimpleMath::Matrix _rotateY  = SimpleMath::Matrix::CreateRotationY(m_timer);
	SimpleMath::Matrix _trans	 = SimpleMath::Matrix::CreateTranslation(m_cursorPos);

	// サイズ　×　回転　×　移動
	_cursorMat *= _scale *  _rotateY * _trans;

	// オブジェクトの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		SimpleMath::Matrix _boxMat =
			SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		// 押し戻し処理を無効化
		is_hitCol.SetPushMode(false);

		// 当たり判定を取る
		is_hitCol.PushBox(&m_cursorPos, m_mapObj[i].position,
			SimpleMath::Vector3{ COMMON_SIZE / 2 },
			SimpleMath::Vector3{ COMMON_SIZE }
		);

		m_mapObj[i].hit = is_hitCol.IsHitBoxFlag();

		if (m_mapObj[i].hit) // 選択中のマスにオブジェクトを描画
		{
			SwitchDraw(m_nowState, _context, _states, _boxMat, _view, _projection);
		}
		else				 // 該当オブジェクトの描画
		{
			SwitchDraw(m_mapObj[i].id, _context, _states, _boxMat, _view, _projection);
		}
	}

	// マウス位置に描画
	if (m_nowState == MAPSTATE::NONE) // 削除時以外は通常の描画
	{
		m_noneModel->Draw(_context, _states, _cursorMat, _view, _projection);
	}
	else
	{
		SwitchDraw(m_nowState, _context, _states, _cursorMat, _view, _projection);
	}

	// スカイドームの描画
	SimpleMath::Matrix _skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROTATE_RATE);
	m_skyDomeModel->Draw(_context, _states, _skyMat, _view, _projection);

	// 画像の描画
	m_userInterface->Render();

	// マウスカーソルの描画
	m_mouseCursor->Render();
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
void EditScene::SwitchDraw(const int& objNum, ID3D11DeviceContext* context,	CommonStates& states,
	SimpleMath::Matrix world, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// 行列計算
	SimpleMath::Matrix _rotateY = SimpleMath::Matrix::CreateRotationY(m_timer);

	switch (objNum)
	{
	case MAPSTATE::GRASS:	// 草
		m_grassModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::COIN:	// コイン
		m_coinModel->Draw(context, states, _rotateY * world, view, proj);
		break;
	case MAPSTATE::CLOUD:	// 雲
		m_cloudModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::GRAVITY:	// 重力
		m_gravityModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::PLAYER:	// プレイヤー
		m_playerModel->Draw(context, states, _rotateY * world, view, proj);
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
	ModelFactory::DeleteModel(m_cloudModel);
	ModelFactory::DeleteModel(m_gravityModel);
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
	auto _device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique(_device, _context);

	// 画面サイズの格納
	float _width = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float _height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// UIの初期化
	m_userInterface = std::make_unique<EditUI>(SimpleMath::Vector2(_width, _height));
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch(_context);
	m_userInterface->Initialize(GetSystemManager(), _device);

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(_width, _height, CAMERA_ANGLE);

	// レイが及ぶ範囲を設定
	GetSystemManager()->GetRayCast()->SetScreenSize(_width, _height);

	// モデルを作成する
	m_grassModel = ModelFactory::GetCreateModel(		// 草ブロック
		_device,
		L"Resources/Models/GrassBlock.cmo"
	);
	m_coinModel = ModelFactory::GetCreateModel(			// コインブロック
		_device,
		L"Resources/Models/Coin.cmo"
	);
	m_cloudModel = ModelFactory::GetCreateModel(		// 雲ブロック
		_device,
		L"Resources/Models/Cloud.cmo"
	);
	m_gravityModel = ModelFactory::GetCreateModel(		// 重力ブロック
		_device,
		L"Resources/Models/ResetPt.cmo"
	);
	m_playerModel = ModelFactory::GetCreateModel(		// プレイヤブロック
		_device,
		L"Resources/Models/Bird.cmo"
	);
	m_noneModel = ModelFactory::GetCreateModel(			// 消しゴムブロック
		_device,
		L"Resources/Models/Eraser.cmo"
	);
	m_skyDomeModel = ModelFactory::GetCreateModel(		// スカイドーム
		_device,
		L"Resources/Models/EditSky.cmo"
	);
	m_skyDomeModel->UpdateEffects([](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				// ライトの数分回す
				for (int i = 0; i < 3; ++i)
				{
					_lights->SetLightEnabled(i, false);
				}
			}
			// 自己発光する
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				_basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	// マウスカーソルの作成
	m_mouseCursor = std::make_unique<MouseCursor>(_context);
	m_mouseCursor->Initialize(L"Resources/Textures/EDITS/MouseCursor.dds", _device);

	// クリアチェッカーの作成
	m_checker = std::make_unique<ClearChecker>();
}

/// <summary>
/// シーン内の変数初期化関数
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void EditScene::SetSceneValues()
{
	// 初期値は草ブロック
	m_nowState = MAPSTATE::GRASS;

	// マップ読み込み//初回読み込み
	LoadMap(L"Datas/Maps/StageEdit.csv");

	// クリアチェッカーに配列を渡す
	m_checker->SetMap(m_mapObj);

	GetSystemManager()->GetCamera()->SetAddPosition(SimpleMath::Vector3{ 0.0f,5.0f,0.0f });
}

/// <summary>
/// マップの編集
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void EditScene::EditMap()
{
	auto _mouse = Mouse::Get().GetState();

	// 移動処理
	m_cursorPos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_cursorPos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;

	// マウスカーソルで移動
	m_cursorPos.y = UserUtility::Lerp(
		m_cursorPos.y,																// 開始地点
		static_cast<float>((_mouse.scrollWheelValue) / WHEEL_SPAWN) + COMMON_LOW,	// 終了地点
		CURSOR_MOVE_SPEED															// 速度
	);

	// 制限をつける
	m_cursorPos.y = UserUtility::Clamp(m_cursorPos.y, CURSOR_MIN, CURSOR_MAX);

	// マップとの当たり判定
	for (auto& i : m_mapObj)
	{
		// 押し戻し処理を無効化
		is_hitCol.SetPushMode(false);

		// 当たり判定を取る
		is_hitCol.PushBox(&m_cursorPos, i.position,
			SimpleMath::Vector3{ COMMON_SIZE / 2 },
			SimpleMath::Vector3{ COMMON_SIZE }
		);

		// 瞬間の当たり判定を取得
		i.hit = is_hitCol.IsHitBoxFlag();

		// クリックでブロック設置
		if (i.hit &&  _mouse.leftButton)
		{
			// 既に同じオブジェクトなら処理しない
			if (i.id == m_nowState) continue;

			// オブジェクトをセット
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SETBOX, false);
			i.id = m_nowState;
		}
	}
}

/// <summary>
/// 座標補正
/// </summary>
/// <param name="object">マップデータ</param>
/// <param name="mode">読み書きモード</param>
/// <returns>なし</returns>
void EditScene::OffsetPosition(std::vector<Object>* object, const MODE& mode)
{
	// 読み込み
	if (mode == READ)
	{
		for (auto& i : *object)
		{
			i.position.x -= static_cast<float>(m_mapLoader.MAP_COLUMN) / 2 * COMMON_SIZE;
			i.position.y += static_cast<float>(COMMON_LOW);
			i.position.z -= static_cast<float>(m_mapLoader.MAP_COLUMN) / 2 * COMMON_SIZE;
		}
	}
	// 書き込み
	if (mode == WRITE)
	{
		for (auto& i : *object)
		{
			i.position.x += static_cast<float>(m_mapLoader.MAP_COLUMN) / 2 * COMMON_SIZE;
			i.position.y -= static_cast<float>(COMMON_LOW);
			i.position.z += static_cast<float>(m_mapLoader.MAP_COLUMN) / 2 * COMMON_SIZE;
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
	m_mapLoader.LoadMap(filename);

	// パスが取得できなければ新しく平面のマップを作成する
	m_filePath = m_mapLoader.GetFilePath();

	// マップを格納する
	m_mapObj = m_mapLoader.GetMapData();

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
	m_mapLoader.WriteMap(m_mapObj);		// ファイルの書き出し
	OffsetPosition(&m_mapObj,READ);		// 読み込み用に座標補正
}

/// <summary>
/// セーブ可能かを確認する
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
bool EditScene::IsCanSave()
{
	// カーソルをつける
	ShowCursor(true);

	// クリアチェッカーに配列を渡す
	m_checker->SetMap(m_mapObj);
	if (m_checker->RunCheck())
	{
		// クリアしやすいかチェックし、難しければ警告を出す
		if (!m_checker->GetClearPossibility())
		{
			if (MessageBox(NULL,
				TEXT("必要条件は満たしていますが、\nコインの数が多いか、離れているためクリアが困難な可能性があります。"),
				TEXT("注意"), MB_YESNO) == IDNO)
			{
				return false;
			}
		}

		// カーソルを消す
		ShowCursor(false);

		// 要件を満たしていたらTrue
		return true;
	}
	else
	{
		if (m_checker->GetPlayerNum() > 1)
		{
			MessageBox(NULL, TEXT("プレイヤーの数が多すぎます。"),
				TEXT("報告"), MB_OK);
		}
		else if (m_checker->GetPlayerNum() < 1)
		{
			MessageBox(NULL, TEXT("プレイヤーを設置してください。"),
				TEXT("報告"), MB_OK);
		}
		if (m_checker->GetCoinNum() < 1)
		{
			MessageBox(NULL, TEXT("コインを設置してください。"),
				TEXT("報告"), MB_OK);
		}
		if (m_checker->GetCloudFlag() == false)
		{
			MessageBox(NULL, TEXT("全ての雲の位置がプレイヤーより高いです。\n上に上がるには下に設置する必要があります。"),
				TEXT("報告"), MB_OK);
		}
		if (m_checker->GetCanStart() == false)
		{
			MessageBox(NULL, TEXT("プレイヤーの下にブロックがないため、スタートできません。"),
				TEXT("報告"), MB_OK);
		}

		// カーソルを消す
		ShowCursor(false);

		// 要件を満たしていなければFalse
		return false;
	}
}
