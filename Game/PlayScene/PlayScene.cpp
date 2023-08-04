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
#include "Objects/PlayerShadow.h"

// プレイヤクラス
#include "Objects/Player.h"

// ブロッククラス
#include "Objects/Blocks.h"

// インターフェース
#include "Objects/PlayUI.h"

#include "PlayScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
PlayScene::PlayScene() :
	IScene(),
	m_timer{0.0f},					// タイマー
	m_startTimer{0.0f},				// 開始時間
	m_timeLimit{0.0f},				// 制限時間
	m_returnTimeVal{0.0f},			// 制限時間(戻り値)
	m_mapLoad{},					// マップ
	m_stageNum{1},					// ステージ番号
	m_fallValue{0.0f},				// 落下用変数
	m_prevIndex{},					// 過去に当たったインデックス番号
	m_hitObjects{},					// 当っているオブジェクトの格納
	m_lastObj{},					// 最後に当たったオブジェクトを保存
	is_boxCol{},					// 立方体当たり判定
	m_skyDomeModel{ nullptr },		// スカイドームモデル
	m_skyColor{}					// 空の変化
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
	m_blocks->Initialize(m_stageNum);

	// プレイヤー座標設定
	m_player->SetPosition(m_blocks->GetPlayerPosition());

	// 判定の初期化
	m_hitObjects.clear();

	// 制限時間の初期化
	// 時間　×　フレームレート
	m_timeLimit = TIME_LIMIT * FLAME_RATE;

	// 開始カウントダウン(フェードも考慮)
	m_startTimer = 4 * FLAME_RATE;

	// 死亡エフェクトを切る
	m_userInterFace->SetEffectFlag(false);

	// 空の色の初期化
	m_skyColor = { 1.0f,1.0f,1.0f };
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

	// カウントダウンが終わったらスタート
	if (StartTimer() == false) return;

	// コインをすべて獲得でリザルト
	if (m_blocks->IsCollectedFlag() || m_timeLimit < 0.0f)
	{
		ChangeScene(SCENE::RESULT);
		return;
	}
	else // クリアしていなければデクリメント
	{
		// 制限時間の計算
		m_timeLimit--;

		// クリアタイムを格納
		m_returnTimeVal = m_timeLimit / FLAME_RATE;

		// 空の処理
		UpdateSky();
	}

	// プレイヤの更新
	m_player->Update(keyState, elapsedTime);

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

		// プレイヤーの影シェーダーの更新
		m_playerShadow->Update(
			SimpleMath::Vector3(
				m_player->GetPosition().x,
				m_lastObj.position.y + 0.55f,
				m_player->GetPosition().z
			)
		);

		// 当たり判定の更新
		Judgement();
	}

	// UIの更新
	m_userInterFace->Update(m_timeLimit);

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

	// ビュー行列
	view = GetSystemManager()->GetCamera()->GetView();

	// プロジェクション行列
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// プレイヤの描画
	m_player->Render(context, states, view, proj);

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

	// ビルボード作成
	m_playerBill->CreateBillboard(
		GetSystemManager()->GetCamera()->GetTargetPosition(),	// カメラの注視点
		GetSystemManager()->GetCamera()->GetEye(),				// カメラの座標
		SimpleMath::Vector3::Up
	);
	// ビルボードの描画
	m_playerBill->Render(m_player->GetPosition(), m_timer, view, proj);

	// プレイヤーの影の描画
	m_playerShadow->Render(context, view, proj);

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

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// 文字の設定
	GetSystemManager()->GetString()->CreateString(device, context);

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
	m_player = std::make_unique<Player>(
		std::move(ModelFactory::GetCreateModel(device,L"Resources/Models/Head.cmo")),	// 頭
		std::move(ModelFactory::GetCreateModel(device,L"Resources/Models/Body.cmo")),	// ボディ
		std::move(ModelFactory::GetCreateModel(device,L"Resources/Models/LegR.cmo")),	// 右足
		std::move(ModelFactory::GetCreateModel(device,L"Resources/Models/LegL.cmo"))	// 左足
	);

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

	// プレイヤーの影シェーダーの作成
	m_playerShadow = std::make_unique<PlayerShadow>();
	m_playerShadow->Create(GetSystemManager()->GetDeviceResources());

	//-------------------------------------------------------------------------------------//

	// UIの作成
	m_userInterFace = std::make_unique<PlayUI>(SimpleMath::Vector2(width, height));
	m_userInterFace->Create(GetSystemManager(),context, device);
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
		// プレイヤの半径2.0fの範囲になければ処理しない
		if (UserUtility::CheckPointInSphere(
			m_player->GetPosition(),									// 中心点
			JUDGE_AREA,													// 半径
			obj.position))												// あるか調べる点
		{
			// 判定を取る
			is_boxCol.PushBox(
				m_player->GetPosition(),								// プレイヤ座標
				obj.position,											// コイン座標
				SimpleMath::Vector3{ m_player->GetSize() },				// プレイヤサイズ
				SimpleMath::Vector3{ m_blocks->GetObjSize(obj.id) }		// ブロックサイズ
			);

			// 当たっていたら処理する
			if (is_boxCol.GetHitBoxFlag())
			{
				// 衝突したオブジェクトをリストに追加
				m_hitObjects.push_back(obj);
			}
		}
	}

	// 衝突したオブジェクトごとに押し戻し処理を行う
	for (auto& obj : m_hitObjects)
	{
		ApplyPushBack(obj);
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
	if (obj.id == MapState::None)
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
	if (obj.id == MapState::CoinBox)
	{
		// 押し戻ししない
		is_boxCol.SetPushMode(false);

		// コインカウントアップ
		m_blocks->CountUpCoin(obj.index);
	}

	//-------------------------------------------------------------------------------------//

	// 雲の処理
	if (obj.id == MapState::ClowdBox)
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

		// 当たっている判定を出す
		m_blocks->SetClowdHitFlag(m_prevIndex.front(), true);

		// 空なら処理しない
		if (m_prevIndex.empty()) return;

		// 入っていたら先頭を削除
		m_prevIndex.pop_front();
	}


	// リセット処理
	if (obj.id == MapState::ResetClowd)
	{
		is_boxCol.SetPushMode(false);
		m_blocks->RestoreClowdPosition();
	}

	//-------------------------------------------------------------------------------------//

	// プレイヤのポジションを保存
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

	//-------------------------------------------------------------------------------------//

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
		m_timeLimit / (TIME_LIMIT * FLAME_RATE), // 赤
		m_timeLimit / (TIME_LIMIT * FLAME_RATE), // 緑
		m_timeLimit / (TIME_LIMIT * FLAME_RATE)  // 青
	};
}

/// <summary>
/// スタートのカウントダウン
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
bool PlayScene::StartTimer()
{
	m_startTimer--;

	if (m_startTimer <= 0.0f)
	{
		m_startTimer = 0.0f;
		return true;
	}

	return false;
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

	// プレイヤの座標
	wchar_t plr[64];
	swprintf_s(plr, 64, L"PlayerPosition = %f,%f,%f", m_player->GetPosition().x, m_player->GetPosition().y, m_player->GetPosition().z);

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,20 }, plr);

	// プレイヤの重力
	wchar_t gra[32];
	swprintf_s(gra, 32, L"Gravity = %f", m_player->GetGravity());

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,40 }, gra);

	// コインカウンタ
	wchar_t coi[32];
	swprintf_s(coi, 32, L"Coin = %d", m_blocks->GetCoinCount());

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,60 }, coi);

	// タイムリミット
	wchar_t time[32];
	swprintf_s(time, 32, L"Limit = %d", static_cast<int>(m_timeLimit));

	GetSystemManager()->GetString()->DrawFormatString(state, { 0,80 }, time);

	// デバイスコンテキストの取得：グリッドの描画に使用
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	// デバッググリッドの表示
	GetSystemManager()->GetGridFloor()->Draw(context, state, view, proj);
}

/// <summary>
/// コインゲッター
/// </summary>
/// <param name="引数無し"></param>
/// <returns>獲得したコインの枚数</returns>
const int& PlayScene::GetCoinNum()
{
	return m_blocks->GetCoinCount();
}
