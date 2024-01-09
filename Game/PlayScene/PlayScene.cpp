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
#include "System/ThirdPersonCamera.h"
#include "../CommonObjects/Blocks.h"
#include "PlayScene.h"

// コンストラクタ
PlayScene::PlayScene(const int& stageNum, const int& coins)
	: IScene()						// 基底クラスの初期化
	, m_stageNum{ stageNum }		// ステージ番号
	, m_allCoins{ coins }			// 保有コイン数
	, m_startTimer{0.0f}			// 開始時間
	, m_gameTimer{0.0f}				// 制限時間
	, m_clearTime{0.0f}				// クリア時間
	, m_fallValue{0.0f}				// 落下用変数
	, m_lighting{}					// ライティング設定
	, is_thirdPersonMode{false}		// サードパーソンモード
	, is_helpFlag{false}			// ヘルプ表示フラグ
{
}

// デストラクタ
PlayScene::~PlayScene()
{
	m_player.reset();
	m_blocks.reset();
	m_playerBill.reset();
	m_playUI.reset();
	m_thirdCamera.reset();
	m_playSky.reset();
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
	if (is_thirdPersonMode) // 三人称カメラ
	{
		m_thirdCamera->UpdateFollow(m_player->GetPosition(), m_player->GetNeckRotate(), THIRD_DISTANCE);
	}
	else // 見下ろしカメラ
	{
		GetSystemManager()->GetCamera()->Update();
	}

	// カウントダウンが終わったらスタート
	if (StartTimer() == false)
	{
		// スタートの動き
		MoveStart();
		return;
	}

	// カメラ操作制限を解除
	GetSystemManager()->GetCamera()->SetEagleMode(true);
	GetSystemManager()->GetCamera()->SetArrowMode(true);

	// サードパーソンモードの切り替え
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::Space))
	{
		is_thirdPersonMode = not is_thirdPersonMode;
	}

	// コインをすべて獲得かタイムアップでリザルト
	if (m_blocks->IsCollectedFlag() || m_gameTimer < 0.0f)
	{
		// クリアタイムを格納
		m_clearTime = m_gameTimer / FLAME_RATE;

		ChangeScene(SCENE::RESULT);
		return;
	}
	else // クリアしていなければデクリメント
	{
		// 制限時間の計算
		m_gameTimer--;

		// 空の処理
		m_playSky->Update(m_gameTimer / (TIME_LIMIT * FLAME_RATE));

		// 時間が半分になったら合図を鳴らす
		if (static_cast<int>(m_gameTimer / FLAME_RATE) == TIME_LIMIT / 2)
		{
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_EVENINGBELL, false);
		}
	}

	// プレイヤの更新
	m_player->Update(is_thirdPersonMode);

	// 相対移動
	m_playerBill->SetVertexMovePos(m_player->GetPosition());

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
	m_playUI->Update(m_gameTimer);

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
	if (StartTimer() == false)
	{
		// スタート演出カメラ
		_view = m_camera->CreateView();
		_projection = m_camera->GetProjection();
	}
	else if (is_thirdPersonMode)
	{
		// リアル視点(三人称)カメラ
		_view = m_thirdCamera->GetFollowView();
		_projection = GetSystemManager()->GetCamera()->GetProjection();
	}
	else
	{
		// 通常カメラ
		_view = GetSystemManager()->GetCamera()->GetView();
		_projection = GetSystemManager()->GetCamera()->GetProjection();
	}

	// ライティングの更新
	InitializeLighting();

	// マップの描画
	m_blocks->Render(*_states, _view, _projection, _timer, m_lighting);

	// プレイヤの描画
	m_player->Render(*_states, _view, _projection, m_lighting);

	// スカイドームの描画
	m_playSky->Draw(*_states, _view, _projection, _timer);

	// ビルボードの描画
	if (not is_thirdPersonMode)
	{
		m_playerBill->CreateBillboard(
			GetSystemManager()->GetCamera()->GetTarget(),	// カメラの注視点
			GetSystemManager()->GetCamera()->GetPosition(),	// カメラの座標
			SimpleMath::Vector3::Up
		);
		m_playerBill->Render(m_player->GetPosition(), _timer, _view, _projection);
	}
	// 回りの靄
	else
	{
		m_thirdCamera->DrawAdhesion();
	}

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

	// サードパーソンカメラの作成
	m_thirdCamera = std::make_unique<ThirdPersonCamera>(GetSystemManager());
	m_thirdCamera->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// スタート用カメラの作成
	m_camera = std::make_unique<PlayCamera>(SimpleMath::Vector2(GetScreenSize().x, GetScreenSize().y));
	m_camera->SetPosition(START_CAMERA_POS);

	// スカイドームの作成
	m_playSky = std::make_unique<PlaySky>(GetFactoryManager(), L"Resources/Models/ShineSky.cmo");

	// プレイヤーの作成
	MakePlayer();

	// ブロックの作成
	MakeBlocks();

	// 位置情報のシェーダーの作成
	m_playerBill = std::make_unique<PlayerBill>(GetFactoryManager());
	m_playerBill->Create();

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
	m_gameTimer = TIME_LIMIT * FLAME_RATE;

	m_clearTime = 0.0f;

	// 開始カウントダウン(フェードも考慮)
	m_startTimer = START_COUNT * FLAME_RATE;

	// 死亡エフェクトを切る
	m_playUI->SetEffectFlag(false);

	// サードパーソンモードを切る
	is_thirdPersonMode = false;

	// ヘルプを表示しない
	is_helpFlag = false;

	// プレイヤの初期化
	m_player->SetAllCoins(m_allCoins);
	m_player->Initialize(GetSystemManager());

	// マップ読み込み
	m_blocks->Initialize(m_stageNum);

	// プレイヤー座標設定
	m_player->SetPosition(m_blocks->GetPlayerPosition());

	// ライティング設定
	m_lighting = -SimpleMath::Vector3::One;
}

// ライティングの設定
void PlayScene::InitializeLighting()
{
	m_player->InitializeLighting(m_lighting);
	m_blocks->InitializeLighting(m_lighting);
}

// プレイヤー作成関数
void PlayScene::MakePlayer()
{
	// ファクトリマネージャ
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	// ファクトリーからモデルをもらう
	auto _head = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Head.cmo");
	auto _body = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Body.cmo");
	auto _legR = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegR.cmo");
	auto _legL = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegL.cmo");
	auto _wink = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Head_w.cmo");

	_fm->LeaveModelFactory();
	// プレイヤーを作成する
	m_player = std::make_unique<Player>(
		std::move(_head),			// 頭のモデル
		std::move(_body),			// 身体のモデル
		std::move(_legR),			// 右足のモデル
		std::move(_legL),			// 左足のモデル
		std::move(_wink)			// ウインク差分
	);
}

// ブロック作成関数
void PlayScene::MakeBlocks()
{
	m_blocks = std::make_unique<Blocks>(GetFactoryManager());
	m_blocks->CreateShader();

	// ファクトリマネージャ
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	// ファクトリーからモデルをもらう
	auto _grass   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/GrassBlock.cmo");
	auto _coin    = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Coin.cmo");
	auto _cloud   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Cloud.cmo");
	auto _gravity = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

	_fm->LeaveModelFactory();

	// モデルの受け渡し
	m_blocks->CreateModels(std::move(_grass),   m_blocks->GRASS);
	m_blocks->CreateModels(std::move(_coin),    m_blocks->COIN);
	m_blocks->CreateModels(std::move(_cloud),   m_blocks->CLOWD);
	m_blocks->CreateModels(std::move(_gravity), m_blocks->GRAVITY);
}

// カウントダウン
bool PlayScene::StartTimer()
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
void PlayScene::MoveStart()
{
	auto& _camera = GetSystemManager()->GetCamera();

	//-------------------//
	// 現在位置
	// 終点位置
	// 移動速度
	//-------------------//

	// カメラ座標の移動
	m_camera->SetPosition(
		UserUtility::Lerp(
			m_camera->GetPosition(),
			_camera->GetPosition(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }
		)
	);
	m_camera->SetTarget(
		UserUtility::Lerp(
			m_camera->GetTarget(),
			_camera->GetTarget(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }
		)
	);
}

// UIの更新
bool PlayScene::UpdateUI()
{
	auto& _input = Input::GetInstance();
	auto& _se = GetSystemManager()->GetSoundManager();

	// ヘルプ表示を切り替える
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::Enter) && StartTimer())
	{
		is_helpFlag = not is_helpFlag;
		m_playUI->SetHelpFlag(is_helpFlag);
		_se->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}

	if (is_helpFlag)
	{
		// 表示切替
		m_playUI->UpdatePage(
			(_input.GetKeyTrack()->IsKeyPressed(Keyboard::A) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Left)),
			(_input.GetKeyTrack()->IsKeyPressed(Keyboard::D) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Right)));

		// ページをめくる音を鳴らす
		if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::A) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Left) ||
			_input.GetKeyTrack()->IsKeyPressed(Keyboard::D) || _input.GetKeyTrack()->IsKeyPressed(Keyboard::Right))
		{
			_se->PlaySound(XACT_WAVEBANK_SKBX_SE_NEXTHELP, false);
		}

		// 遷移ページの場合、UpdateTransitionにUpDownWSを渡す
		if (m_playUI->GetTransitionPage())
		{
			// 上下キーで選択
			m_playUI->UpdateTransition(
				(_input.GetKeyTrack()->IsKeyPressed(Keyboard::W) ||
				 _input.GetKeyTrack()->IsKeyPressed(Keyboard::Up)),
				(_input.GetKeyTrack()->IsKeyPressed(Keyboard::S) ||
				 _input.GetKeyTrack()->IsKeyPressed(Keyboard::Down)));

			if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::W) ||
				_input.GetKeyTrack()->IsKeyPressed(Keyboard::Up)||
				_input.GetKeyTrack()->IsKeyPressed(Keyboard::S) ||
				_input.GetKeyTrack()->IsKeyPressed(Keyboard::Down))
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
