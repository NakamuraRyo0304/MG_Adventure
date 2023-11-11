/*
 *	@File	EditScene.cpp
 *	@Brief  エディットシーン。
 *	@Date	2023-09-24
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Objects/EditUI.h"
#include "Objects/EditSky.h"
#include "System/MouseCursor.h"
#include "System/ClearChecker.h"
#include "EditScene.h"

// MementPatternの参考サイト
//-------------------------------------------------------------------------------------//
// 　REFACTORING・GURU様
//   https://refactoring.guru/ja/design-patterns/memento
//   shiraberu.tech様
//   https://shiraberu.tech/2021/11/19/memento-patten/
//-------------------------------------------------------------------------------------//

// コンストラクタ
EditScene::EditScene()
	: IScene()									// 基底クラスの初期化
	, m_mapLoader{}								// マップローダー
	, m_mapObj{0}								// 格納配列
	, m_nowState{}								// 現在のブロックの種類
	, m_blockCount{0}							// オブジェクトの数
	, m_grassModel{ nullptr }					// 草
	, m_noneModel{ nullptr }					// 消しゴム
	, m_coinModel{ nullptr }					// コイン
	, m_cloudModel{ nullptr }					// 雲
	, m_gravityModel{ nullptr }					// 重力
	, m_cursorPos{ SimpleMath::Vector3::Zero }	// カーソルの位置
	, m_history{}								// ログ管理
	, m_XZCheck{}								// XZのブロックの位置を求める
	, is_hitCol{}								// 立方体当たり判定
{
	ShowCursor(false);
}

// デストラクタ
EditScene::~EditScene()
{
	Finalize();
	ShowCursor(true);
}

// 初期化処理
void EditScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// 変数の初期化
	SetSceneValues();
}

// 更新処理
void EditScene::Update()
{
	// インプットの更新
	auto& _input = Input::GetInstance();

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// レイの更新
	GetSystemManager()->GetRayCast()->Update();

	// UIの処理
	m_editUI->Update();

	// マウスカーソルの位置を更新
	m_mouseCursor->Update();

	// エスケープでセレクトに戻る
	if(_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::SELECT);}

	// サウンド
	auto& _sound = GetSystemManager()->GetSoundManager();

	// 選択しているオブジェクトを格納
	m_nowState = m_editUI->GetNowState();

	// 編集の取り消し等処理
	DoUndoRedo();

	// モデルのカウント
	if (_input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		ResetObjNum();
		CountObjNum();
	}

	// アイコンクリックの更新
	ClickIcons();

	// Cキーを押したら、カメラモードを反転する
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::C))
	{
		// インターフェースでカメラのフラグを取得
		_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		m_editUI->SetCameraFlag(!m_editUI->GetCameraFlag());
		GetSystemManager()->GetCamera()->SetEagleMode(m_editUI->GetCameraFlag());
	}
}

// 描画処理
void EditScene::Draw()
{
	// 描画関連
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& _states = *GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

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
	SimpleMath::Matrix _rotateY  = SimpleMath::Matrix::CreateRotationY(_timer);
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
			SwitchDraw(m_nowState, _context, _states, _rotateY * _boxMat, _view, _projection);
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
	m_skyDome->Draw(_states, _view, _projection, _timer);

	// 画像の描画
	m_editUI->Render();

	// デバッグ情報を表示
	GetSystemManager()->GetString()->DrawFormatString(
		_states, { 25,200 }, Colors::Yellow,SimpleMath::Vector2(1.5f),
		L"Grass:%d\nCoin:%d\nCloud:%d\nGravity:%d\nPlayer:%d",
		m_blockCount[MAPSTATE::GRASS],
		m_blockCount[MAPSTATE::COIN],
		m_blockCount[MAPSTATE::CLOUD],
		m_blockCount[MAPSTATE::GRAVITY],
		m_blockCount[MAPSTATE::PLAYER]
	);

	// マウスカーソルの描画
	m_mouseCursor->Render();
}

// 描画オブジェクト切り替え
void EditScene::SwitchDraw(const int& objNum, ID3D11DeviceContext* context,	CommonStates& states,
	SimpleMath::Matrix world, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	switch (objNum)
	{
	case MAPSTATE::GRASS:	// 草
		m_grassModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::COIN:	// コイン
		m_coinModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::CLOUD:	// 雲
		m_cloudModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::GRAVITY:	// 重力
		m_gravityModel->Draw(context, states, world, view, proj);
		break;
	case MAPSTATE::PLAYER:	// プレイヤー
		m_playerModel->Draw(context, states, world, view, proj);
		break;
	default:
		break;
	}
}

// 狩猟処理
void EditScene::Finalize()
{
	// マップの解放
	m_mapObj.clear();

	// モデルの破棄
	auto& _mf = GetFactoryManager();
	_mf->BuildModelFactory();

	_mf->VisitModelFactory()->DeleteModel(m_grassModel);
	_mf->VisitModelFactory()->DeleteModel(m_coinModel);
	_mf->VisitModelFactory()->DeleteModel(m_cloudModel);
	_mf->VisitModelFactory()->DeleteModel(m_gravityModel);
	_mf->VisitModelFactory()->DeleteModel(m_playerModel);
	_mf->VisitModelFactory()->DeleteModel(m_noneModel);

	_mf->LeaveModelFactory();
}

// 画面依存、デバイス依存の初期化
void EditScene::CreateWindowDependentResources()
{
	// システムの作成
	GetSystemManager()->CreateSystem();
	GetFactoryManager()->CreateFactory();

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// UIの初期化
	m_editUI = std::make_unique<EditUI>();
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_editUI->Create(GetSystemManager(),GetScreenSize());

	// レイが及ぶ範囲を設定
	GetSystemManager()->GetRayCast()->SetScreenSize(GetScreenSize().x, GetScreenSize().y);

	// モデルを作成する
	GetFactoryManager()->BuildModelFactory();
	CreateModels(GetFactoryManager());
	GetFactoryManager()->LeaveModelFactory();

	// スカイドームの作成
	m_skyDome = std::make_unique<EditSky>(GetFactoryManager(), L"Resources/Models/EditSky.cmo");

	// マウスカーソルの作成
	m_mouseCursor = std::make_unique<MouseCursor>();
	m_mouseCursor->Initialize(L"Resources/Textures/EDITS/MouseCursor.dds");

	// クリアチェッカーの作成
	m_checker = std::make_unique<ClearChecker>();
}

// シーン変数初期化関数
void EditScene::SetSceneValues()
{
	// カメラ視点移動を有効にする
	GetSystemManager()->GetCamera()->SetEagleMode(m_editUI->GetCameraFlag());

	// 初期値は草ブロック
	m_nowState = MAPSTATE::GRASS;

	// マップ読み込み//初回読み込み
	LoadMap(L"Datas/Maps/StageEdit.csv");

	// クリアチェッカーに配列を渡す
	m_checker->SetMap(m_mapObj);

	// 読み込んだ後に一時保存する
	SaveModification();

	// カメラの位置をマップの中心にする
	SimpleMath::Vector2 _XZ = { m_mapLoader.MAP_COLUMN,m_mapLoader.MAP_RAW };
	GetSystemManager()->GetCamera()->AddEyePosition(SimpleMath::Vector3{ _XZ.x / 2,3.0f,_XZ.y / 2 });

	// XZ保存変数を初期化
	m_XZCheck = SimpleMath::Vector3::Zero;
}

// モデルの作成
void EditScene::CreateModels(std::shared_ptr<FactoryManager> fm)
{
	m_grassModel = // 草ブロック
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/GrassBlock.cmo");

	m_coinModel = // コインブロック
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Coin.cmo");

	m_cloudModel = // 雲ブロック
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Cloud.cmo");

	m_gravityModel = // 重力ブロック
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

	m_playerModel = // プレイヤモデル
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Bird.cmo");

	m_noneModel = // 消しゴムモデル
		fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Eraser.cmo");
}

// モデルの数をリセットする
void EditScene::ResetObjNum()
{
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		m_blockCount[m_mapObj[i].id] = 0;
	}
}

// モデルの数をカウントする
void EditScene::CountObjNum()
{
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		m_blockCount[m_mapObj[i].id]++;
	}
}

// アイコンをクリックする
void EditScene::ClickIcons()
{
	auto& _input = Input::GetInstance();
	auto& _sound = GetSystemManager()->GetSoundManager();

	// セーブフラグがたったらファイルを保存
	if (m_editUI->GetSaveFlag() &&
		_input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		// 要素チェックして保存可能なら実行
		if (IsCanSave())
		{
			_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
			SaveFile();
		}
	}

	// オープンフラグがたったらファイルを開く
	if (m_editUI->GetOpenFlag() &&
		_input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		if (!m_mapLoader.LoadMap(L""))
		{
			return;
		}
		m_mapObj = m_mapLoader.GetMapData();	// 読み込み
	}

	// プレイシーンに行く
	if (m_editUI->GetGoPlayFlag() &&
		_input.GetMouseTrack()->leftButton == Mouse::ButtonStateTracker::RELEASED)
	{
		_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		ChangeScene(SCENE::PLAY);
	}

	// カメラモードじゃなければ編集できる
	if (not m_editUI->GetCameraFlag())
	{
		EditMap();
	}

	// ボタンクリックでセレクトに戻る
	if (m_editUI->GetBackSelectFlag())
	{
		_sound->PlaySound(XACT_WAVEBANK_SKBX_SE_ICONTAP, false);
		ChangeScene(SCENE::SELECT);
	}
}

// マップの編集
void EditScene::EditMap()
{
	auto _mouse = Mouse::Get().GetState();

	// 移動処理
	m_cursorPos.x = GetSystemManager()->GetRayCast()->GetWorldMousePosition().x;
	m_cursorPos.z = GetSystemManager()->GetRayCast()->GetWorldMousePosition().z;

	// マウスカーソルで移動
	m_cursorPos.y = UserUtility::Lerp(
		m_cursorPos.y,												// 開始地点
		static_cast<float>((_mouse.scrollWheelValue) / WHEEL_SPAWN),// 終了地点
		CURSOR_MOVE_SPEED											// 速度
	);

	// 選択行を保存
	m_XZCheck = m_cursorPos;

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

			// 一時保存する
			SaveModification();

			// オブジェクトをセット
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SETBOX, false);
			i.id = m_nowState;
		}
	}
}

// マップ読み込み
void EditScene::LoadMap(std::wstring filename)
{
	// マップの読み込み
	m_mapLoader.LoadMap(filename);

	// パスが取得できなければ新しく平面のマップを作成する
	m_filePath = m_mapLoader.GetFilePath();

	// マップを格納する
	m_mapObj = m_mapLoader.GetMapData();

	// モデルの数を数える
	ResetObjNum();
	CountObjNum();
}

// ファイル書き出し
void EditScene::SaveFile()
{
	m_mapLoader.WriteMap(m_mapObj);		// ファイルの書き出し
}

// 要素チェック
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

// UndoRedoを実行
void EditScene::DoUndoRedo()
{
	auto& _input = Input::GetInstance();

	// 前に戻る
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::Z))
	{
		RestoreHistory(m_history.GetUndo());
		ResetObjNum();
		CountObjNum();
	}
	// Undoを取り消す
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::X))
	{
		RestoreHistory(m_history.GetRedo());
		ResetObjNum();
		CountObjNum();
	}
}

// UndoRedo保存
void EditScene::SaveModification()
{
	// 状態を保存
	m_history.AddHistory(MementoMap(m_mapObj));
}

// UndoRedoの適用
void EditScene::RestoreHistory(MementoMap mement)
{
	m_mapObj = mement.GetState();
}
