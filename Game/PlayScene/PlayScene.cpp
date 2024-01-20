/*
 *	@File	PlayScene.cpp
 *	@Brief  プレイシーン。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/ColliderHelper.h"
#include "Libraries/SystemDatas/MapLoad.h"
#include "Objects/Player.h"
#include "Objects/PlayerBill.h"
#include "Objects/PlayUI.h"
#include "Objects/PlaySky.h"
#include "System/PlayCamera.h"
#include "../CommonObjects/Blocks.h"
#include "PlayScene.h"

// コンストラクタ
PlayScene::PlayScene(const int& stageNum, const int& coins)
	: IScene()						// 基底クラスの初期化
	, m_stageNum{ stageNum }		// ステージ番号
	, m_allCoins{ coins }			// 保有コイン数
	, m_startTimer{}				// 開始時間
	, m_timeLinits{}				// 制限時間
	, m_clearTime{}					// クリア時間
	, m_fallValue{}					// 落下用変数
	, m_lighting{}					// ライティング設定
	, is_helpFlag{false}			// ヘルプ表示フラグ
{
}

// デストラクタ
PlayScene::~PlayScene()
{
	m_player.reset();
	m_blocks.reset();
	m_playerPoint.reset();
	m_playUI.reset();
	m_sky.reset();
	m_camera.reset();
	m_collider.reset();
}

// 初期化処理
void PlayScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// 変数の初期化
	SetSceneValues();
}

// 更新処理
void PlayScene::Update()
{
	// インプットの更新
	auto& _input = Input::GetInstance();

	// サウンドの更新
	GetSystemManager()->GetSoundManager()->Update();

	// エスケープでセレクトに戻る
	if(_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::SELECT);}

	// UI表示中は以降処理しない
	if (UpdateUI()) return;

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// カウントダウンが終わったらスタート
	if (Countdown() == false)
	{
		// スタートの動き
		FirstMovement();
		return;
	}

	// カメラ操作制限を解除
	GetSystemManager()->GetCamera()->SetEagleMode(true);
	GetSystemManager()->GetCamera()->SetArrowMode(true);

	// コインをすべて獲得かタイムアップでリザルト
	if (m_blocks->IsCollectedFlag() || m_timeLinits < 0.0f)
	{
		// クリアタイムを格納
		m_clearTime = m_timeLinits / FLAME_RATE;

		ChangeScene(SCENE::RESULT);
		return;
	}
	else // クリアしていなければデクリメント
	{
		// 制限時間の計算
		m_timeLinits--;

		// 空の処理
		m_sky->Update(m_timeLinits / (TIME_LIMIT * FLAME_RATE));

		// 時間が半分になったら合図を鳴らす
		if (static_cast<int>(m_timeLinits / FLAME_RATE) == TIME_LIMIT / 2)
		{
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_EVENINGBELL, false);
		}
	}

	// プレイヤの更新
	m_player->Update();

	// プレイヤーの位置にポイント画像を表示
	m_playerPoint->SetVertexMovePos(m_player->GetPosition());

	// ステージの下に落ちたらステージ崩壊演出
	if (m_player->GetPosition().y < DURATION_FLOOR_LINE)
	{
		// 落下させる
		m_fallValue -= FALL_SPEED;

		for (auto& i : m_blocks->GetMapData())
		{
			// エフェクトをオフ
			m_playUI->SetEffectFlag(false);

			// オブジェクトの振動
			GetSystemManager()->GetCamera()->ShakeObject(
				SHAKE_DURATION,							// 振動時間
				SHAKE_TREMOR * SHAKE_OBJ_POWER,			// 振動範囲
				&m_blocks->GetBlockPosition(i.index)	// 振動オブジェクト
			);

			// 座標のセット
			m_blocks->SetBlockPosition(
				SimpleMath::Vector3(
				i.position.x,						// X軸
				i.position.y + m_fallValue,			// Y軸
				i.position.z),						// Z軸
				i.index								// 配列番号
			);
		}
	}
	// 落下したら死亡エフェクトを出す
	else if (m_player->GetPosition().y < DEATH_EFFECT_LINE)
	{
		// オブジェクトの振動
		for (auto& i : m_blocks->GetMapData())
		{
			// エフェクトをオン
			if (static_cast<int>(m_player->GetPosition().y) % 2 == 0)
			{
				m_playUI->SetEffectFlag(true);
			}
			else
			{
				m_playUI->SetEffectFlag(false);
			}
			GetSystemManager()->GetCamera()->ShakeObject(
				1.0f,									// 振動時間
				SHAKE_TREMOR,							// 振動範囲
				&m_blocks->GetBlockPosition(i.index)	// 振動オブジェクト
			);
		}
	}
	// プレイヤー生存時のみ処理
	else
	{
		// ブロックの更新
		m_blocks->Update();

		// 当たり判定の更新
		m_collider->Update(m_player.get(),m_blocks.get());
	}

	// UIの更新
	m_playUI->Update(m_timeLinits);

	// 落下したらリスタート
	if (m_player->GetDeathFlag())
	{
		ChangeScene(SCENE::PLAY);
	}
}

// 描画処理
void PlayScene::Draw()
{
	// 描画関連
	auto& _states = GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// カメラ用行列
	SimpleMath::Matrix _view, _projection;

	// ビュー行列&プロジェクション行列
	if (Countdown() == false)
	{
		// スタート演出カメラ
		_view = m_camera->CreateView();
		_projection = m_camera->GetProjection();
	}
	else
	{
		// 通常カメラ
		_view = GetSystemManager()->GetCamera()->GetView();
		_projection = GetSystemManager()->GetCamera()->GetProjection();
	}

	// ライティングの更新
	m_blocks->SetLighting(m_lighting);

	// マップの描画
	m_blocks->Render(*_states, _view, _projection, _timer, m_lighting);

	// プレイヤーの描画
	m_player->Render(*_states, _view, _projection);

	// スカイドームの描画
	m_sky->Draw(*_states, _view, _projection, _timer);

	// ビルボードの描画
	auto& _position = GetSystemManager()->GetCamera()->GetPosition();
	auto& _target = GetSystemManager()->GetCamera()->GetTarget();
	m_playerPoint->CreateBillboard(_target, _position, SimpleMath::Vector3::Up);
	m_playerPoint->Render(m_player->GetPosition(), _timer, _view, _projection);

	// UIの描画
	m_playUI->IsCountDownEnd() ? m_playUI->Render() : m_playUI->RenderCountDown(m_startTimer);
}

// 終了処理
void PlayScene::Finalize()
{
	// プレイヤの後処理
	m_player->Finalize();

	// マップの後処理
	m_blocks->Finalize();
}

// 画面依存、デバイス依存の初期化
void PlayScene::CreateWindowDependentResources()
{
	// システムの作成
	GetSystemManager()->CreateSystem();
	GetFactoryManager()->CreateFactory();

	// デフォルトカメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// スタート用カメラの作成
	m_camera = std::make_unique<PlayCamera>(SimpleMath::Vector2(GetScreenSize().x, GetScreenSize().y));
	m_camera->SetPosition(START_CAMERA_POS);

	// スカイドームの作成
	m_sky = std::make_unique<PlaySky>(GetFactoryManager(), L"Resources/Models/ShineSky.cmo");

	// プレイヤーの作成
	CreatePlayer();

	// ブロックの作成
	CreateBlock();

	// 位置情報のシェーダーの作成
	m_playerPoint = std::make_unique<PlayerBill>(GetFactoryManager());
	m_playerPoint->Create();

	// UIの作成
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_playUI = std::make_unique<PlayUI>();
	m_playUI->Create(GetSystemManager(), GetScreenSize());

	// 当たり判定の作成
	m_collider = std::make_unique<ColliderHelper>(JUDGE_AREA, GetSystemManager());
}

// シーン変数初期化関数
void PlayScene::SetSceneValues()
{
	// カメラ視点移動
	GetSystemManager()->GetCamera()->SetEagleMode(false);
	GetSystemManager()->GetCamera()->SetArrowMode(false);

	// BGMを鳴らす
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_PLAY, true);

	// 制限時間の初期化
	// 時間　×　フレームレート
	m_timeLinits = TIME_LIMIT * FLAME_RATE;

	m_clearTime = 0.0f;

	// 開始カウントダウン(フェードも考慮)
	m_startTimer = START_COUNT * FLAME_RATE;

	// 死亡エフェクトを切る
	m_playUI->SetEffectFlag(false);

	// ヘルプを表示しない
	is_helpFlag = false;

	// プレイヤの初期化
	m_player->SetAllCoins(m_allCoins);
	m_player->Initialize();

	// マップ読み込み
	m_blocks->Initialize(m_stageNum);

	// プレイヤー座標設定
	m_player->SetPosition(m_blocks->GetPlayerPosition());

	// ライティング設定
	m_lighting = -SimpleMath::Vector3::One;
}

// プレイヤー作成関数
void PlayScene::CreatePlayer()
{
	// ファクトリマネージャ
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	// ファクトリーからモデルをもらう
	auto _head = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Head.cmo");
	auto _body = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Body.cmo");
	auto _legR = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegR.cmo");
	auto _legL = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegL.cmo");

	_fm->LeaveModelFactory();

	// プレイヤーを作成する
	m_player = std::make_unique<Player>(std::move(_head),
		std::move(_body), std::move(_legR), std::move(_legL));
}

// ブロック作成関数
void PlayScene::CreateBlock()
{
	// ファクトリマネージャ
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	// ファクトリーからモデルをもらう
	auto _grass   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineBox.cmo");
	auto _coin    = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineCoin.cmo");
	auto _cloud   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineCl.cmo");
	auto _gravity = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

	_fm->LeaveModelFactory();

	// ブロックを作成する
	m_blocks = std::make_unique<Blocks>(GetFactoryManager());
	m_blocks->CreateShader();

	// モデルの受け渡し
	m_blocks->CreateModels(std::move(_grass),   m_blocks->GRASS);
	m_blocks->CreateModels(std::move(_coin),    m_blocks->COIN);
	m_blocks->CreateModels(std::move(_cloud),   m_blocks->CLOWD);
	m_blocks->CreateModels(std::move(_gravity), m_blocks->GRAVITY);
}

// カウントダウン
bool PlayScene::Countdown()
{
	m_startTimer -= COUNT_SPEED;

	if (m_startTimer <= 0.0f)
	{
		m_startTimer = 0.0f;
		return true;
	}

	return false;
}

// スタートの動き
void PlayScene::FirstMovement()
{
	auto& _camera = GetSystemManager()->GetCamera();

	// カメラ座標の移動
	m_camera->SetPosition(
		UserUtility::Lerp(m_camera->GetPosition(), _camera->GetPosition(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }));

	// カメラ注視点の移動
	m_camera->SetTarget(
		UserUtility::Lerp(m_camera->GetTarget(), _camera->GetTarget(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }));
}

// UIの更新
bool PlayScene::UpdateUI()
{
	auto& _input = Input::GetInstance();
	auto& _se = GetSystemManager()->GetSoundManager();

	// ヘルプ表示を切り替える
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::H) && Countdown())
	{
		is_helpFlag = not is_helpFlag;
		m_playUI->SetHelpFlag(is_helpFlag);
		_se->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}

	if (is_helpFlag)
	{
		bool _leftFlag = (_input.GetKeyTrack()->IsKeyPressed(Keyboard::A) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Left));
		bool _rightFlag = (_input.GetKeyTrack()->IsKeyPressed(Keyboard::D) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Right));
		bool _upFlag = (_input.GetKeyTrack()->IsKeyPressed(Keyboard::W) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Up));
		bool _downFlag = (_input.GetKeyTrack()->IsKeyPressed(Keyboard::S) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Down));

		// 表示切替
		m_playUI->UpdatePage(_leftFlag, _rightFlag);

		// ページをめくる音を鳴らす
		if (_leftFlag || _rightFlag)
		{
			_se->PlaySound(XACT_WAVEBANK_SKBX_SE_NEXTHELP, false);
		}

		// 遷移ページの場合、UpdateTransitionにUpDownWSを渡す
		if (m_playUI->GetTransitionPage())
		{
			// 上下キーで選択
			m_playUI->UpdateTransition(_upFlag, _downFlag);

			if (_upFlag || _downFlag)
			{
				_se->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
			}

			// 決定する
			if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::Space))
			{
				// シーンの遷移
				HelpNext();
				_se->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
			}
		}
	}

	// ヘルプ表示中は他の処理をしない
	return is_helpFlag;
}

// ヘルプからシーン遷移
void PlayScene::HelpNext()
{
	using NextScene = PlayUI::GO;
	switch (m_playUI->GetTransNum())
	{
	case NextScene::RETRY:
		ChangeScene(SCENE::PLAY);
		break;
	case NextScene::SELECT:
		ChangeScene(SCENE::SELECT);
		break;
	case NextScene::TITLE:
		ChangeScene(SCENE::TITLE);
		break;
	case NextScene::EXIT:
		ChangeScene(SCENE::ENDGAME);
		break;
	default:
		break;
	}
}

// 獲得コイン数
const int& PlayScene::GetCoinNum() { return m_blocks->GetCoinCount(); }

// ステージ内合計コイン数
const int& PlayScene::GetMaxCoinCount() { return m_blocks->GetMaxCoinCount(); }
