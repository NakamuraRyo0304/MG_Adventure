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

// 影の表示
#include "Resources/Shaders/Shadow.h"

#include "PlayScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
PlayScene::PlayScene()
	: IScene()						// 基底クラスの初期化
	, m_timer{0.0f}					// タイマー
	, m_startTimer{0.0f}			// 開始時間
	, m_gameTimer{0.0f}				// 制限時間
	, m_clearTime{0.0f}				// クリア時間
	, m_allCoins{0}					// 保有コイン数
	, m_mapLoad{}					// マップ
	, m_stageNum{1}					// ステージ番号
	, m_fallValue{0.0f}				// 落下用変数
	, m_prevIndex{}					// 過去に当たったインデックス番号
	, m_hitObjects{}				// 当っているオブジェクトの格納
	, m_lastObj{}					// 最後に当たったオブジェクトを保存
	, m_skyDomeModel{nullptr}		// スカイドームモデル
	, m_skyColor{}					// 空の変化
	, is_boxCol{}					// 立方体当たり判定
	, is_thirdPersonMode{false}		// サードパーソンモード
	, is_helpFlag{false}			// ヘルプ表示フラグ
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
	GetSystemManager()->GetCamera()->SetEagleMode(false);
	GetSystemManager()->GetCamera()->SetArrowMode(false);

	// 変数の初期化
	SetSceneValues();

	// BGMを鳴らす
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_PLAY, true);
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

	// サウンドの更新
	GetSystemManager()->GetSoundManager()->Update();

	// 風の音を鳴らす
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_WIND, true);

	// エスケープで終了
	GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape) ? ChangeScene(SCENE::ENDGAME) : void();

	// ヘルプ表示を切り替える
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Enter) && StartTimer())
	{
		is_helpFlag = !is_helpFlag;
		m_userInterFace->SetHelpFlag(is_helpFlag);
	}

	// ヘルプ表示中は処理しない
	if (is_helpFlag) return;

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
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Space))
	{
		is_thirdPersonMode = !is_thirdPersonMode;
	}

	// コインすべて獲得でクリア音を鳴らす なんかならないから要修正
	if (m_blocks->IsCollectedFlag())
	{
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_CLEAR, false);
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
	}

	// プレイヤの更新
	m_player->Update(keyState, elapsedTime ,is_thirdPersonMode);

	// 相対移動
	m_playerBill->SetVertexMovePos(m_player->GetPosition());

	// ステージの下に落ちたらステージ崩壊演出
	if (m_player->GetPosition().y < DURATION_FLOOR_LINE)
	{
		// 落下させる
		m_fallValue -= FALL_SPEED;

		for (auto& obj : m_blocks->GetMapData())
		{
			// エフェクトをオフ
			m_userInterFace->SetEffectFlag(false);

			// オブジェクトの振動
			GetSystemManager()->GetCamera()->ShakeObject(
				SHAKE_DURATION,							// 振動時間
				SHAKE_TREMOR * 2.5f,					// 振動範囲
				&m_blocks->GetBlockPosition(obj.index)	// 振動オブジェクト
			);

			// 座標のセット
			m_blocks->SetBlockPosition(
				SimpleMath::Vector3(
				obj.position.x,							// X軸
				obj.position.y + m_fallValue,			// Y軸
				obj.position.z),						// Z軸
				obj.index								// 配列番号
			);
		}
	}
	// 落下したら死亡エフェクトを出す
	else if (m_player->GetPosition().y < DURATION_FLOOR_LINE / 1.5f)
	{
		// オブジェクトの振動
		for (auto& obj : m_blocks->GetMapData())
		{
			// エフェクトをオン
			if (static_cast<int>(m_player->GetPosition().y) % 2 == 0)
			{
				m_userInterFace->SetEffectFlag(true);
			}
			else
			{
				m_userInterFace->SetEffectFlag(false);
			}
			GetSystemManager()->GetCamera()->ShakeObject(
				1.0f,									// 振動時間
				SHAKE_TREMOR,							// 振動範囲
				&m_blocks->GetBlockPosition(obj.index)	// 振動オブジェクト
			);
		}
	}
	// プレイヤー生存時のみ処理
	else
	{
		// ブロックの更新
		m_blocks->Update(elapsedTime);

		// 当たり判定の更新
		Judgement();

		// 衝突したオブジェクトごとに押し戻し処理を行う
		for (auto& obj : m_hitObjects)
		{
			ApplyPushBack(obj);
		}
	}

	// UIの更新
	m_userInterFace->Update(m_gameTimer);

	// 落下したらリスタート
	if (m_player->GetDeathFlag())
	{
		ChangeScene(SCENE::PLAY);
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
	SimpleMath::Matrix view, proj;

	// ビュー行列&プロジェクション行列
	if (StartTimer() == false)
	{
		// スタート演出カメラ
		view = m_playCamera->CreateView();
		proj = m_playCamera->GetProjection();
	}
	else if (is_thirdPersonMode)
	{
		// リアル視点(三人称)カメラ
		view = m_thirdCamera->GetFollowView();
		proj = GetSystemManager()->GetCamera()->GetProjection();
	}
	else
	{
		// 通常カメラ
		view = GetSystemManager()->GetCamera()->GetView();
		proj = GetSystemManager()->GetCamera()->GetProjection();
	}

	// プレイヤの描画
	m_player->Render(context, states, view, proj);
	//m_shadow->Draw(L"head", context, &states);

	// マップの描画
	m_blocks->Render(context, states, view, proj, m_timer);

	// スカイドームの描画
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);
	m_skyDomeModel->UpdateEffects([&](IEffect* effect)
		{
			// 色を徐々に暗くする
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// 徐々に暗くなっていく
				basicEffect->SetEmissiveColor(
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
		m_playerBill->Render(m_player->GetPosition(), m_timer, view, proj);
	}
	else
	{
		m_thirdCamera->DrawAdhesion();
	}

	// UIの描画
	if (static_cast<int>(m_startTimer) == 0)
	{
		m_userInterFace->Render();
	}
	else
	{
		m_userInterFace->RenderCountDown(m_startTimer);
	}
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

	// マップの後処理
	m_blocks->Finalize();

	// 判定用配列を解放
	m_hitObjects.clear();
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

	// デフォルトカメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// サードパーソンカメラの作成
	m_thirdCamera = std::make_unique<ThirdPersonCamera>(GetSystemManager(), context, device);
	m_thirdCamera->CreateProjection(width, height, CAMERA_ANGLE);

	// スタート用カメラの作成
	m_playCamera = std::make_unique<PlayCamera>(SimpleMath::Vector2(width, height));
	m_playCamera->SetPosition(START_CAMERA_POS);

	//-------------------------------------------------------------------------------------//
	// スカイドームモデルを作成する
	m_skyDomeModel = ModelFactory::GetCreateModel(
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

	//-------------------------------------------------------------------------------------//
	// プレイヤーの作成
	MakePlayer(device);

	//-------------------------------------------------------------------------------------//
	// ブロックの作成
	m_blocks = std::make_unique<Blocks>();

	// 草ブロックの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo")),
		m_blocks->GRASS
	);
	// コインの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo")),
		m_blocks->COIN
	);
	// 雲ブロックの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/MoveBlock.cmo")),
		m_blocks->CLOWD
	);
	// 雲リセットブロックの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo")),
		m_blocks->RECLOWD
	);

	//-------------------------------------------------------------------------------------//
	// 位置情報のシェーダーの作成
	m_playerBill = std::make_unique<PlayerBill>();
	m_playerBill->Create(GetSystemManager()->GetDeviceResources());

	//-------------------------------------------------------------------------------------//
	// UIの作成
	m_userInterFace = std::make_unique<PlayUI>(SimpleMath::Vector2(width, height));
	m_userInterFace->Create(GetSystemManager(),context, device);

	//-------------------------------------------------------------------------------------//
	// 影の作成
	m_shadow = std::make_unique<Shadow>();
	m_shadow->CreateShadow(device);
	//m_shadow->SetModel(L"head", std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Head.cmo")));
}

/// <summary>
/// シーン内の変数初期化関数
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayScene::SetSceneValues()
{
	// 判定の初期化
	m_hitObjects.clear();

	// 制限時間の初期化
	// 時間　×　フレームレート
	m_gameTimer = TIME_LIMIT * FLAME_RATE;

	m_clearTime = 0.0f;

	// 開始カウントダウン(フェードも考慮)
	m_startTimer = 4 * FLAME_RATE;

	// 死亡エフェクトを切る
	m_userInterFace->SetEffectFlag(false);

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
}

/// <summary>
/// 当たり判定処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayScene::Judgement()
{
	// 衝突したオブジェクトリストを初期化
	m_hitObjects.clear();

	// 当たり判定を取る
	for (auto& obj : m_blocks->GetMapData())
	{
		// プレイヤの半径1.5fの範囲になければ処理しない
		// 引数（基準点、検索範囲、検索点）
		if (UserUtility::CheckPointInSphere(
			m_player->GetPosition(),JUDGE_AREA,	obj.position))
		{
			// 判定を取る
			is_boxCol.PushBox(
				m_player->GetPosition(),								// プレイヤ座標
				obj.position,											// コイン座標
				SimpleMath::Vector3{ m_player->GetSize() },				// プレイヤサイズ
				SimpleMath::Vector3{ m_blocks->GetObjSize(obj.id) }		// ブロックサイズ
			);

			// 当たっていたら処理する
			if (is_boxCol.IsHitBoxFlag())
			{
				// 衝突したオブジェクトをリストに追加
				m_hitObjects.push_back(obj);
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
	if (obj.id == MAPSTATE::NONE)
	{
		is_boxCol.SetPushMode(false);
		return;
	}
	else
	{
		is_boxCol.SetPushMode(true);
	}

	//-------------------------------------------------------------------------------------//

	// コインの処理
	if (obj.id == MAPSTATE::COIN)
	{
		// 押し戻ししない
		is_boxCol.SetPushMode(false);

		// コインカウントアップ
		m_blocks->CountUpCoin(obj.index);

		// コインのサウンドを鳴らす
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_COINGETTER, false);
	}

	//-------------------------------------------------------------------------------------//

	// 雲の処理
	if (obj.id == MAPSTATE::CLOUD)
	{
		// プレイヤーが下にいたら押し戻ししない
		if (m_player->GetPosition().y < obj.position.y + m_blocks->GetObjSize(obj.id))
		{
			is_boxCol.SetPushMode(false);
			return;
		}

		// 判定を有効化
		is_boxCol.SetPushMode(true);

		// インデックス番号を格納
		m_prevIndex.push_back(obj.index);

		// 空なら処理しない
		if (m_prevIndex.empty()) return;

		// 当たっている判定を出す
		m_blocks->SetCloudHitFlag(m_prevIndex.front(), true);

		// 入っていたら先頭を削除
		m_prevIndex.pop_front();
	}

	// リセット処理
	if (obj.id == MAPSTATE::RESET)
	{
		is_boxCol.SetPushMode(false);
		m_blocks->RestoreCloudPosition();
	}

	//-------------------------------------------------------------------------------------//

	// 直前のプレイヤのポジションを保存
	SimpleMath::Vector3 playerPos = m_player->GetPosition();

	// 当たり判定を取って押し戻す
	is_boxCol.PushBox(
		&playerPos,												// プレイヤ座標
		obj.position,											// ブロック座標
		SimpleMath::Vector3{ m_player->GetSize() },				// プレイヤサイズ
		SimpleMath::Vector3{ m_blocks->GetObjSize(obj.id)}		// ブロックサイズ
	);

	// 変更後のプレイヤのポジションを反映
	m_player->SetPosition(playerPos);

	// ブロックの上に当たっていたら重力を初期化
	if (is_boxCol.GetHitFace() == Collider::BoxCollider::HIT_FACE::UP)
	{
		m_player->ResetGravity();
	}

	// 処理が終わったら要素を破棄
	m_hitObjects.pop_back();
}

/// <summary>
/// 空の更新
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayScene::UpdateSky()
{
	m_skyColor =
	{
		m_gameTimer / (TIME_LIMIT * FLAME_RATE), // 赤
		m_gameTimer / (TIME_LIMIT * FLAME_RATE), // 緑
		m_gameTimer / (TIME_LIMIT * FLAME_RATE)  // 青
	};
}

/// <summary>
/// スタートのカウントダウン
/// </summary>
/// <param name="引数無し"></param>
/// <returns>終わっていたらTrueを返す</returns>
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

/// <summary>
/// スタート演出
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void PlayScene::MoveStart()
{
	Camera cam = *GetSystemManager()->GetCamera();

	//-------------------//
	// 現在位置
	// 終点位置
	// 移動速度
	//-------------------//

	// カメラ座標の移動
	m_playCamera->SetPosition(
		UserUtility::Lerp(
			m_playCamera->GetPosition(),
			cam.GetPosition(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }
		)
	);
	m_playCamera->SetTarget(
		UserUtility::Lerp(
			m_playCamera->GetTarget(),
			cam.GetTarget(),
			SimpleMath::Vector3{ MOVE_CAMERA_SPEED }
		)
	);
}

/// <summary>
/// プレイヤーの作成
/// </summary>
/// <param name="device">デバイスポインタ</param>
/// <returns>なし</returns>
void PlayScene::MakePlayer(ID3D11Device1* device)
{
	//-------------------------------------------------------------------------------------//
	// パスの格納
	std::unique_ptr<Model> head =
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Head.cmo"));
	std::unique_ptr<Model> body =
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Body.cmo"));
	std::unique_ptr<Model> legR =
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/LegR.cmo"));
	std::unique_ptr<Model> legL =
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/LegL.cmo"));
	//-------------------------------------------------------------------------------------//

	// プレイヤーを作成する
	m_player = std::make_unique<Player>(
		std::move(head),			// 頭のモデル
		std::move(body),			// 身体のモデル
		std::move(legR),			// 右足のモデル
		std::move(legL)				// 左足のモデル
	);
}

/// <summary>
/// 獲得したコインゲッター
/// </summary>
/// <param name="引数無し"></param>
/// <returns>獲得したコインの枚数</returns>
const int& PlayScene::GetCoinNum()
{
	return m_blocks->GetCoinCount();
}

/// <summary>
/// ステージの最大コインゲッター
/// </summary>
/// <param name="引数無し"></param>
/// <returns>ステージの最大コインの枚数</returns>
const int& PlayScene::GetMaxCoinCount()
{
	return m_blocks->GetMaxCoinCount();
}
