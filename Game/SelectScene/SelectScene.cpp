/*
 *	@File	SelectScene.cpp
 *	@Brief  セレクトシーン。
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include <thread>
#include <mutex>

// マップローダー
#include "Libraries/SystemDatas/MapLoad.h"

// ブロックの作成
#include "../PlayScene/Objects/Blocks.h"

// UI
#include "Objects/SelectUI.h"

#include "SelectScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
SelectScene::SelectScene()
	: IScene()					// 基底クラスの初期化
	, m_timer{}					// タイマー
	, m_flashCount{}			// 点滅のカウンタ
	, m_stageNum{1}				// ステージ番号
	, m_safeStages{}			// 未開放ステージ番号
	, m_allCoins{}				// 合計コイン数
	, m_useCoins{}				// 使用コイン数
	, m_initCoins{}				// 開始コイン数
	, m_targetY{}				// カメラのターゲットのY座標
	, m_mutex{}					// アシンクロック
	, is_selectEdit{false}		// ステージエディットを選択
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
SelectScene::~SelectScene()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// カメラ視点移動を切る
	GetSystemManager()->GetCamera()->SetEagleMode(false);

	// 変数の初期化
	SetSceneValues();

	// BGMを鳴らす
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_TITLESELECT, true);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">時間/fps</param>
/// <param name="keyState">キーボードポインタ</param>
/// <param name="mouseState">マウスポインタ</param>
/// <returns>なし</returns>
void SelectScene::Update(const float& elapsedTime, Keyboard::State& keyState,
	Mouse::State& mouseState)
{
	m_timer = elapsedTime;

	// キー入力情報を取得する
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// サウンドの更新
	GetSystemManager()->GetSoundManager()->Update();

	// 選択変更時の演出
	DirectionSelectChange();

	// ステージの変更
	ChangeStageNumber(keyState);

	// UIの更新
	m_userInterface->Update(keyState.Right, keyState.Left);

	// エスケープで終了
	GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape) ? ChangeScene(SCENE::ENDGAME) : void();

	// コインの数を保存
	m_allCoins = m_initCoins - static_cast<int>(m_useCoins);

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		// フェード中は処理しない
		if (static_cast<int>(GetFadeValue()) != 0) return;

		// ステージ番号が0ならエディタに、それ以外はプレイへ
		if (m_stageNum == 0)
		{
			is_selectEdit = m_allCoins >= STAGE_CREATE_PRICE;

			// メッセージ
			if(m_allCoins < STAGE_CREATE_PRICE)
			{
				MessageBox(NULL,
					TEXT("コインを10枚以上取得していないため、この機能は使用できません。"),
					TEXT("コイン不足"), MB_OK);
			}
		}
		else
		{
			ChangeScene(SCENE::PLAY);
		}

		// 決定音を鳴らす
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}

	// コイン演出
	if (is_selectEdit)
	{
		// コイン数をセットする
		m_userInterface->SetAllCoins(m_initCoins - static_cast<int>(m_useCoins));
		m_userInterface->MoveCoins(m_useCoins);

		// 使用料を支払ったら遷移する
		if (static_cast<int>(m_useCoins) == STAGE_CREATE_PRICE)
		{
			ChangeScene(SCENE::EDIT);
		}
		else
		{
			m_useCoins += COUNT_SPEED;
		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectScene::Draw()
{
	// 描画関連
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	SimpleMath::Matrix view, proj;

	// 回転量を計算
	float rotValue = m_timer * 0.5f;
	// 上下移動量を計算
	float verticalValue = sinf(m_timer * 1.5f) * 1.5f;

	// ビュー行列
	SimpleMath::Vector3    eye(CAMERA_RADIUS * sinf(rotValue),		// X:回転(ステージと逆回転)
						       CAMERA_POS_Y  + verticalValue,		// Y:移動(上下)
						       CAMERA_RADIUS * cosf(rotValue));		// Z:回転(ステージと逆回転)
	SimpleMath::Vector3 target(0.0f, m_targetY, 0.0f);

	view = SimpleMath::Matrix::CreateLookAt(eye, target, SimpleMath::Vector3::Up);

	// プロジェクション行列
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// マップの描画
	m_blocks[m_stageNum] != nullptr ? m_blocks[m_stageNum]->Render(context, states, view, proj, m_timer) : void();

	// スカイドームの描画
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);

	// UIの表示
	m_userInterface->Render(m_stageNum, MAX_STAGE_NUM - 1);

	// 点滅させる
	if (m_flashCount > MAX_FLASH * 0.5f) return;

	// テキストの移動アニメーション
	SimpleMath::Matrix stageMat = CreateTextMatrix(rotValue);

	// ステージ番号表示
	m_stageModels[m_stageNum]->Draw(context, states, stageMat, view, proj);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectScene::Finalize()
{
	// モデル削除
	for (size_t i = 0; i < MAX_STAGE_NUM; ++i)
	{
		m_blocks[i]->Finalize();
	}
	m_userInterface->Finalize();
}

/// <summary>
/// 画面依存、デバイス依存の初期化
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectScene::CreateWindowDependentResources()
{
	// デバイスとデバイスコンテキストの取得
	auto device = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique(device, context);

	// 画面サイズの格納
	float width =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// スカイドームモデルを作成する
	{
		m_skyDomeModel = ModelFactory::GetCreateModel(device, L"Resources/Models/ShineSky.cmo");
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

	// ブロックの作成を裏で処理
	{
		std::lock_guard<std::mutex>guard(m_mutex);

		m_loadTask = std::async(std::launch::async, [&]() { CreateStages(device); });
	}

	// ステージ番号のモデルの作成
	{
		for (int i = 0; i < MAX_STAGE_NUM - m_safeStages; ++i)
		{
			// 0番目はエディタの文字
			if (i == 0)
			{
				m_stageModels[0] = ModelFactory::GetCreateModel(device, L"Resources/Models/StageEdit.cmo");
			}
			else
			{
				std::wstring path = L"Resources/Models/Stage" + std::to_wstring(i) + L".cmo";
				m_stageModels[i] = ModelFactory::GetCreateModel(device, path.c_str());
			}
		}
	}

	// UIの作成
	m_userInterface = std::make_unique<SelectUI>(GetSystemManager(), context, device);
	m_userInterface->Initialize(SimpleMath::Vector2{ width, height });
}

/// <summary>
/// シーン内の変数初期化関数
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectScene::SetSceneValues()
{
	// 見上げ距離
	m_targetY = UP_VALUE;

	// スタートが0
	m_flashCount = 0.0f;

	// コイン数をセット
	m_userInterface->SetAllCoins(m_allCoins);

	// 使用コイン数をリセット
	m_useCoins = 0.0f;

	// 開始コイン数を保存
	m_initCoins = m_allCoins;

	// ステージエディットフラグを初期化
	is_selectEdit = false;
}

/// <summary>
/// ステージモデルのロード
/// </summary>
/// <param name="device">デバイスポインタ</param>
/// <returns>なし</returns>
void SelectScene::CreateStages(ID3D11Device1* device)
{
	// 選択中のステージを先に作成
	CreateFirstStage(device);

	for (int i = 0; i < MAX_STAGE_NUM; ++i)
	{
		// ファクトリーで生成
		auto grass   = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo"));
		auto coin    = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo"));
		auto cloud   = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Cloud.cmo"));
		auto gravity = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo"));

		// 作成されていない場合は作成する
		if (!m_blocks[i])
		{
			// ブロックの作成
			m_blocks[i] = std::make_unique<Blocks>();
			m_blocks[i]->CreateShader(device);

			// モデルの受け渡し
			m_blocks[i]->CreateModels(std::move(grass),   m_blocks[i]->GRASS);
			m_blocks[i]->CreateModels(std::move(coin),    m_blocks[i]->COIN);
			m_blocks[i]->CreateModels(std::move(cloud),   m_blocks[i]->CLOWD);
			m_blocks[i]->CreateModels(std::move(gravity), m_blocks[i]->GRAVITY);

			// 初期化処理
			m_blocks[i]->Initialize(i);
		}
	}
}

/// <summary>
/// 最初のステージだけ先に作成する(async用)
/// </summary>
/// <param name="device">デバイスポインタ</param>
/// <returns>なし</returns>
void SelectScene::CreateFirstStage(ID3D11Device1* device)
{
	// ファクトリーで生成
	auto grass   = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo"));
	auto coin    = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo"));
	auto cloud   = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Cloud.cmo"));
	auto gravity = std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo"));

	// ブロックの作成
	m_blocks[m_stageNum] = std::make_unique<Blocks>();
	m_blocks[m_stageNum]->CreateShader(device);

	// モデルの受け渡し
	m_blocks[m_stageNum]->CreateModels(std::move(grass),   m_blocks[m_stageNum]->GRASS);
	m_blocks[m_stageNum]->CreateModels(std::move(coin),    m_blocks[m_stageNum]->COIN);
	m_blocks[m_stageNum]->CreateModels(std::move(cloud),   m_blocks[m_stageNum]->CLOWD);
	m_blocks[m_stageNum]->CreateModels(std::move(gravity), m_blocks[m_stageNum]->GRAVITY);

	// 初期化処理
	m_blocks[m_stageNum]->Initialize(m_stageNum);
}

/// <summary>
/// ステージの選択
/// </summary>
/// <param name="keyState">キーボード</param>
/// <returns>なし</returns>
void SelectScene::ChangeStageNumber(Keyboard::State keyState)
{
	// 切り替え可能なタイミングはここで変更
	if (m_targetY >= UP_VALUE * 0.25f) return;

	if (keyState.Right || keyState.D)
	{
		// ステージ番号が最大なら処理しない
		if (m_stageNum == MAX_STAGE_NUM - 1 - m_safeStages) return;

		// 選択音を鳴らす
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);

		m_targetY = UserUtility::Lerp(m_targetY, UP_VALUE, 0.8f);
		m_stageNum++;
		m_flashCount = 0.0f;
	}
	if (keyState.Left || keyState.A)
	{
		// ステージ番号が0なら処理しない
		if (m_stageNum == 0) return;

		// 選択音を鳴らす
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);

		m_targetY = UserUtility::Lerp(m_targetY, UP_VALUE, 0.8f);
		m_stageNum--;
		m_flashCount = 0.0f;
	}
}

/// <summary>
/// セレクト変更時の演出
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectScene::DirectionSelectChange()
{
	// 動きが終わっていなければ見下げる
	if (m_targetY > 1.0f)
	{
		m_targetY -= DOWN_SPEED;
	}
	else if (m_targetY > 0.0f)
	{
		m_targetY -= DOWN_SPEED * 0.5f;
	}
	else
	{
		m_targetY = 0.0f;
	}

	// フラッシュカウンタ
	m_flashCount++;
	m_flashCount = m_flashCount > MAX_FLASH * 0.75f ? 0.0f : m_flashCount;
}

/// <summary>
///	テキストのマトリックスを作成
/// </summary>
/// <param name="rotValue">回転する値</param>
/// <returns>なし</returns>
SimpleMath::Matrix SelectScene::CreateTextMatrix(const float& rotValue)
{
	SimpleMath::Matrix mat = SimpleMath::Matrix::Identity;
	mat *= SimpleMath::Matrix::CreateRotationY(rotValue);
	mat *= SimpleMath::Matrix::CreateScale(10.0f);
	mat *= SimpleMath::Matrix::CreateTranslation(sinf(rotValue), 10.0f, cosf(rotValue));

	return mat;
}

/// <summary>
/// コインセッター
/// </summary>
/// <param name="num">合計コイン数</param>
/// <returns>なし</returns>
void SelectScene::SetAllCoins(const int& num)
{
	m_allCoins = num;
}
