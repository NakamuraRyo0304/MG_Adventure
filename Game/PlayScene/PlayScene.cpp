/*
 *	@File	PlayScene.cpp
 *	@Brief  プレイシーン。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

// CSV読み込みクラス
#include "../../Libraries/SystemDatas/MapLoad.h"

// コライダークラス
#include "../../Libraries/SystemDatas/Collider.h"

// シェーダー位置情報クラス
#include "Objects/PlayerBill.h"

// プレイヤクラス
#include "Objects/Player.h"

// ブロッククラス
#include "Objects/Blocks.h"

// インターフェース
#include "Objects/PlayUI.h"

// サードパーソンカメラクラス
#include "System/ThirdPersonCamera.h"

// プレイカメラクラス(スタート演出用)
#include "System/PlayCamera.h"

#include "PlayScene.h"


// コンストラクタ
PlayScene::PlayScene()
	: IScene()						// 基底クラスの初期化
	, m_startTimer{0.0f}			// 開始時間
	, m_gameTimer{0.0f}				// 制限時間
	, m_clearTime{0.0f}				// クリア時間
	, m_allCoins{0}					// 保有コイン数
	, m_mapLoad{}					// マップ
	, m_stageNum{1}					// ステージ番号
	, m_fallValue{0.0f}				// 落下用変数
	, m_prevIndex{}					// 過去に当たったインデックス番号
	, m_hitObj{}					// 当っているオブジェクトの格納
	, m_lastObj{}					// 最後に当たったオブジェクトを保存
	, m_skyDomeModel{nullptr}		// スカイドームモデル
	, m_lighting{}					// ライティング設定
	, m_skyColor{}					// 空の変化
	, is_hitCol{}					// 立方体当たり判定
	, is_thirdPersonMode{false}		// サードパーソンモード
	, is_helpFlag{false}			// ヘルプ表示フラグ
{
}

// デストラクタ
PlayScene::~PlayScene()
{
	Finalize();
}

// 初期化処理
void PlayScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// カメラ視点移動
	GetSystemManager()->GetCamera()->SetEagleMode(false);
	GetSystemManager()->GetCamera()->SetArrowMode(false);

	// 変数の初期化
	SetSceneValues();

	// BGMを鳴らす
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_PLAY, true);
}

// 更新処理
void PlayScene::Update()
{
	// インプットの更新
	auto& _input = Input::GetInstance();

	// サウンドの更新
	GetSystemManager()->GetSoundManager()->Update();

	// エスケープで終了
	if(_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::ENDGAME);}

	// UI表示中は以降処理しない
	if (UpdateUI()) return;

	// カメラの更新
	if (is_thirdPersonMode) // 三人称カメラ
	{
		m_thirdCamera->UpdateFollow(
			m_player->GetPosition(),				// ターゲット座標
			m_player->GetNeckRotate(),				// 回転量
			SimpleMath::Vector3(0.0f, 2.5f, 6.0f)	// ターゲットからの距離
		);
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
		is_thirdPersonMode = !is_thirdPersonMode;
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
		UpdateSky();

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
				SHAKE_TREMOR * 2.5f,					// 振動範囲
				&m_blocks->GetBlockPosition(i.index)	// 振動オブジェクト
			);

			// 座標のセット
			m_blocks->SetBlockPosition(
				SimpleMath::Vector3(
				i.position.x,							// X軸
				i.position.y + m_fallValue,			// Y軸
				i.position.z),						// Z軸
				i.index								// 配列番号
			);
		}
	}
	// 落下したら死亡エフェクトを出す
	else if (m_player->GetPosition().y < DURATION_FLOOR_LINE / 1.5f)
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
		Judgement();

		// 衝突したオブジェクトごとに押し戻し処理を行う
		for (auto& i : m_hitObj)
		{
			ApplyPushBack(i);
		}
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
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& _states = *GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// カメラ用行列
	SimpleMath::Matrix _view, _projection;

	// ビュー行列&プロジェクション行列
	if (StartTimer() == false)
	{
		// スタート演出カメラ
		_view = m_playCamera->CreateView();
		_projection = m_playCamera->GetProjection();
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
	m_blocks->Render(_states, _view, _projection, _timer, m_lighting);

	// プレイヤの描画
	m_player->Render(_states, _view, _projection, m_lighting);

	// スカイドームの描画
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(_context, _states, skyMat, _view, _projection);
	m_skyDomeModel->UpdateEffects([&](IEffect* effect)
		{
			// 色を徐々に暗くする
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				// 徐々に暗くなっていく
				_basicEffect->SetEmissiveColor(
					SimpleMath::Color{
						m_skyColor.red,		 // 赤
						m_skyColor.green,	 // 緑
						m_skyColor.blue,	 // 青
						1.0f				 // 透明度
					}
				);
			}
		}
	);

	// ビルボードの描画
	if (!is_thirdPersonMode)
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
	if (m_playUI->IsCountDownEnd())
	{
		m_playUI->Render();
	}
	else
	{
		m_playUI->RenderCountDown(m_startTimer);
	}
}

// 終了処理
void PlayScene::Finalize()
{
	// プレイヤの後処理
	m_player->Finalize();

	// マップの後処理
	m_blocks->Finalize();

	// 判定用配列を解放
	m_hitObj.clear();
}

// 画面依存、デバイス依存の初期化
void PlayScene::CreateWindowDependentResources()
{
	// デバイスの取得
	auto _device = GetSystemManager()->GetDeviceResources()->GetD3DDevice();

	// メイクユニーク
	GetSystemManager()->CreateUnique();

	// デフォルトカメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// サードパーソンカメラの作成
	m_thirdCamera = std::make_unique<ThirdPersonCamera>(GetSystemManager());
	m_thirdCamera->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// スタート用カメラの作成
	m_playCamera = std::make_unique<PlayCamera>(SimpleMath::Vector2(GetScreenSize().x, GetScreenSize().y));
	m_playCamera->SetPosition(START_CAMERA_POS);

	//-------------------------------------------------------------------------------------//
	// スカイドームモデルを作成する
	m_skyDomeModel = ModelFactory::GetCreateModel(
		_device,
		L"Resources/Models/ShineSky.cmo"
	);
	m_skyDomeModel->UpdateEffects([](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				_lights->SetLightEnabled(0, false);
				_lights->SetLightEnabled(1, false);
				_lights->SetLightEnabled(2, false);
			}
			// 自己発光する
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				_basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	// プレイヤーの作成
	MakePlayer(_device);

	// ブロックの作成
	MakeBlocks(_device);

	// 位置情報のシェーダーの作成
	m_playerBill = std::make_unique<PlayerBill>();
	m_playerBill->Create();

	// UIの作成
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_playUI = std::make_unique<PlayUI>();
	m_playUI->Create(GetSystemManager(), GetScreenSize());
}

// シーン変数初期化関数
void PlayScene::SetSceneValues()
{
	// 判定の初期化
	m_hitObj.clear();

	// 制限時間の初期化
	// 時間　×　フレームレート
	m_gameTimer = TIME_LIMIT * FLAME_RATE;

	m_clearTime = 0.0f;

	// 開始カウントダウン(フェードも考慮)
	m_startTimer = 4 * FLAME_RATE;

	// 死亡エフェクトを切る
	m_playUI->SetEffectFlag(false);

	// 空の色の初期化
	m_skyColor = { 1.0f,1.0f,1.0f };

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
void PlayScene::MakePlayer(ID3D11Device1* device)
{
	// ファクトリーからモデルをもらう
	auto _head = ModelFactory::GetCreateModel(device, L"Resources/Models/Head.cmo");
	auto _body = ModelFactory::GetCreateModel(device, L"Resources/Models/Body.cmo");
	auto _legR = ModelFactory::GetCreateModel(device, L"Resources/Models/LegR.cmo");
	auto _legL = ModelFactory::GetCreateModel(device, L"Resources/Models/LegL.cmo");
	auto _wink = ModelFactory::GetCreateModel(device, L"Resources/Models/Head_w.cmo");

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
void PlayScene::MakeBlocks(ID3D11Device1* device)
{
	m_blocks = std::make_unique<Blocks>();
	m_blocks->CreateShader();

	// ファクトリーからモデルをもらう
	auto _grass   = ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo");
	auto _coin    = ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo");
	auto _cloud   = ModelFactory::GetCreateModel(device, L"Resources/Models/Cloud.cmo");
	auto _gravity = ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo");

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
	auto& _cam = GetSystemManager()->GetCamera();

	//-------------------//
	// 現在位置
	// 終点位置
	// 移動速度
	//-------------------//

	// カメラ座標の移動
	m_playCamera->SetPosition(
		UserUtility::Lerp(
			m_playCamera->GetPosition(),
			_cam->GetPosition(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }
		)
	);
	m_playCamera->SetTarget(
		UserUtility::Lerp(
			m_playCamera->GetTarget(),
			_cam->GetTarget(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }
		)
	);
}

// 空の更新
void PlayScene::UpdateSky()
{
	m_skyColor =
	{
		m_gameTimer / (TIME_LIMIT * FLAME_RATE), // 赤
		m_gameTimer / (TIME_LIMIT * FLAME_RATE), // 緑
		m_gameTimer / (TIME_LIMIT * FLAME_RATE)  // 青
	};
}

// UIの更新
bool PlayScene::UpdateUI()
{
	auto& _input = Input::GetInstance();
	auto& _se = GetSystemManager()->GetSoundManager();

	// ヘルプ表示を切り替える
	if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::Enter) && StartTimer())
	{
		is_helpFlag = !is_helpFlag;
		m_playUI->SetHelpFlag(is_helpFlag);
		_se->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}

	if (is_helpFlag)
	{
		// 表示切替
		m_playUI->UpdatePage(
			(_input.GetKeyTrack()->IsKeyPressed(Keyboard::A) ||  // 左
		     _input.GetKeyTrack()->IsKeyPressed(Keyboard::Left)),
			(_input.GetKeyTrack()->IsKeyPressed(Keyboard::D) ||  // 右
		     _input.GetKeyTrack()->IsKeyPressed(Keyboard::Right)));

		// ページをめくる音を鳴らす
		if (_input.GetKeyTrack()->IsKeyPressed(Keyboard::A)    ||
			_input.GetKeyTrack()->IsKeyPressed(Keyboard::Left) ||
			_input.GetKeyTrack()->IsKeyPressed(Keyboard::D)    ||
			_input.GetKeyTrack()->IsKeyPressed(Keyboard::Right))
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
	switch (m_playUI->GetTransNum())
	{
	case PlayUI::GO::RETRY:
		ChangeScene(SCENE::PLAY);
		break;
	case PlayUI::GO::SELECT:
		ChangeScene(SCENE::SELECT);
		break;
	case PlayUI::GO::TITLE:
		ChangeScene(SCENE::TITLE);
		break;
	case PlayUI::GO::EXIT:
		ChangeScene(SCENE::ENDGAME);
		break;
	default:
		break;
	}
}

// 判定処理
void PlayScene::Judgement()
{
	// 衝突したオブジェクトリストを初期化
	m_hitObj.clear();

	// 当たり判定を取る
	for (auto& i : m_blocks->GetMapData())
	{
		// マップステータスがなしの時は飛ばす
		if (i.id == MAPSTATE::NONE) continue;

		// プレイヤの判定範囲外は処理しない
		// 引数（基準点、検索範囲、検索点）
		if (!UserUtility::CheckPointInSphere(
			m_player->GetPosition(), JUDGE_AREA, i.position)) continue;

		// 判定を取る
		is_hitCol.PushBox(
			m_player->GetPosition(), i.position,					// プレイヤ、オブジェクトの座標
			SimpleMath::Vector3{ m_player->GetSize() },				// プレイヤサイズ
			SimpleMath::Vector3{ m_blocks->GetObjSize(i.id) }		// ブロックサイズ
		);

		// 当たっていたら処理する
		if (is_hitCol.IsHitBoxFlag())
		{
			// 衝突したオブジェクトをリストに追加
			m_hitObj.push_back(i);
		}
	}
}

// 押し戻し処理
void PlayScene::ApplyPushBack(Object& obj)
{
	// 当っているオブジェがなしの場合は処理しない
	if (obj.id == MAPSTATE::NONE)
	{
		// 要素を消して終了
		m_hitObj.pop_back();
		return;
	}

	// デフォルトで判定をつける
	is_hitCol.SetPushMode(true);

	// コインの獲得処理
	if (obj.id == MAPSTATE::COIN)
	{
		// 押し戻ししない
		is_hitCol.SetPushMode(false);

		// コインカウントアップ
		m_blocks->CountUpCoin(obj.index);

		// コインのサウンドを鳴らす
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_COINGETTER, false);
	}

	// 雲の浮上処理
	if (obj.id == MAPSTATE::CLOUD)
	{
		// プレイヤーが下にいたら押し戻ししない
		if (m_player->GetPosition().y < obj.position.y + m_blocks->GetObjSize(MAPSTATE::CLOUD))
		{
			// 要素を消して終了
			m_hitObj.pop_back();
			return;
		}

		// 判定を有効化
		is_hitCol.SetPushMode(true);

		// インデックス番号を格納
		m_prevIndex.push_back(obj.index);

		// 空なら処理しない
		if (m_prevIndex.empty()) return;

		// 当たっている判定を出す
		m_blocks->SetCloudHitFlag(m_prevIndex.front(), true);

		// 要素を消す
		m_prevIndex.pop_front();
	}

	// 重力処理
	if (obj.id == MAPSTATE::GRAVITY)
	{
		is_hitCol.SetPushMode(false);
		m_blocks->CallGravity();
	}

	// 直前のプレイヤのポジションを保存
	SimpleMath::Vector3 _playerPos = m_player->GetPosition();

	// 当たり判定を取って押し戻す
	is_hitCol.PushBox(
		&_playerPos,obj.position,								// プレイヤ、オブジェクトの座標
		SimpleMath::Vector3{ m_player->GetSize() },				// プレイヤサイズ
		SimpleMath::Vector3{ m_blocks->GetObjSize(obj.id)}		// ブロックサイズ
	);

	// 変更後のプレイヤのポジションを反映
	m_player->SetPosition(_playerPos);

	// ブロックの上に乗っていたら重力をなくす
	if (is_hitCol.GetHitFace() == Collider::BoxCollider::HIT_FACE::UP)
	{
		m_player->ResetGravity();
	}

	// 要素を消して終了
	m_hitObj.pop_back();
}

// 獲得コイン数
const int& PlayScene::GetCoinNum()
{
	return m_blocks->GetCoinCount();
}

// ステージ内合計コイン数
const int& PlayScene::GetMaxCoinCount()
{
	return m_blocks->GetMaxCoinCount();
}
