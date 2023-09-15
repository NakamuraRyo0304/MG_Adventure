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
SelectScene::SelectScene():
	IScene(),
	m_timer{},				// タイマー
	m_flashCount{},			// 点滅のカウンタ
	m_stageNum{1},			// ステージ番号
	m_noStageNum{},			// 未開放ステージ番号
	m_totalCoins{},			// 合計コイン数
	m_targetY{},			// カメラのターゲットのY座標
	m_mutex{}				// ロック
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

	// コイン数をセット
	m_userInterface->SetTotalCoins(m_totalCoins);

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
	m_userInterface->Update(elapsedTime, keyState.Right, keyState.Left);

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		// ステージ番号が0ならエディタに、それ以外はプレイへ
		m_stageNum == 0 ? ChangeScene(SCENE::EDIT) : ChangeScene(SCENE::PLAY);
	}

	// エスケープで終了
	GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape) ? ChangeScene(SCENE::ENDGAME) : void();
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
	if(m_blocks[m_stageNum] != nullptr)
	{
		m_blocks[m_stageNum]->Render(context, states, view, proj, m_timer);
	}

	// スカイドームの描画
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);


	//-------------------------------------------------------------------------------------//

	// UIの表示
	m_userInterface->Render(m_stageNum);

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
	m_blocks[0]->Finalize();
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
	GetSystemManager()->GetString()->CreateString(device, context);

	// 画面サイズの格納
	float width =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, CAMERA_ANGLE);

	// スカイドームモデルを作成する
	{
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
	}

	// ブロックの作成を裏で処理
	{
		std::lock_guard<std::mutex>guard(m_mutex);

		m_loadTask = std::async(std::launch::async, [&]() { CreateStages(device); });
	}

	// ステージ番号のモデルの作成
	{
		for (int i = 0; i < MAX_STAGE_NUM - m_noStageNum; ++i)
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
		//パスの格納---------------------------------------------------------------------------//
		const wchar_t* grassPath = L"Resources/Models/GrassBlock.cmo";		// 草ブロック      //
		const wchar_t* coinPath  = L"Resources/Models/Coin.cmo";			// コイン          //
		const wchar_t* cloudPath = L"Resources/Models/MoveBlock.cmo";		// 雲              //
		const wchar_t* resetPath = L"Resources/Models/ResetPt.cmo";			// リセットポイント//
		//-------------------------------------------------------------------------------------//

		// 作成されていない場合は作成する
		if (!m_blocks[i])
		{
			// ブロックの作成
			m_blocks[i] = std::make_unique<Blocks>();

			// 草ブロックの作成
			m_blocks[i]->CreateModels(std::move(ModelFactory::GetCreateModel(device, grassPath)), m_blocks[i]->GRASS);

			// コインの作成
			m_blocks[i]->CreateModels(std::move(ModelFactory::GetCreateModel(device, coinPath)), m_blocks[i]->COIN);

			// 雲ブロックの作成
			m_blocks[i]->CreateModels(std::move(ModelFactory::GetCreateModel(device, cloudPath)), m_blocks[i]->CLOWD);

			// 雲リセットブロックの作成
			m_blocks[i]->CreateModels(std::move(ModelFactory::GetCreateModel(device, resetPath)), m_blocks[i]->RECLOWD);

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
	//パスの格納---------------------------------------------------------------------------//
	const wchar_t* grassPath = L"Resources/Models/GrassBlock.cmo";		// 草ブロック      //
	const wchar_t* coinPath  = L"Resources/Models/Coin.cmo";			// コイン          //
	const wchar_t* cloudPath = L"Resources/Models/MoveBlock.cmo";		// 雲              //
	const wchar_t* resetPath = L"Resources/Models/ResetPt.cmo";			// リセットポイント//
	//-------------------------------------------------------------------------------------//

	// ブロックの作成
	m_blocks[m_stageNum] = std::make_unique<Blocks>();

	// 草ブロックの作成
	m_blocks[m_stageNum]->CreateModels(std::move(ModelFactory::GetCreateModel(device, grassPath)),
																				m_blocks[m_stageNum]->GRASS);
	// コインの作成
	m_blocks[m_stageNum]->CreateModels(std::move(ModelFactory::GetCreateModel(device, coinPath)),
																				m_blocks[m_stageNum]->COIN);
	// 雲ブロックの作成
	m_blocks[m_stageNum]->CreateModels(std::move(ModelFactory::GetCreateModel(device, cloudPath)),
																				m_blocks[m_stageNum]->CLOWD);
	// 雲リセットブロックの作成
	m_blocks[m_stageNum]->CreateModels(std::move(ModelFactory::GetCreateModel(device, resetPath)),
																				m_blocks[m_stageNum]->RECLOWD);
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

	if (keyState.Right)
	{
		// ステージ番号が最大なら処理しない
		if (m_stageNum == MAX_STAGE_NUM - 1 - m_noStageNum) return;

		// 選択音を鳴らす
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);

		m_targetY = UserUtility::Lerp(m_targetY, UP_VALUE, 0.8f);
		m_stageNum++;
		m_flashCount = 0.0f;
	}
	if (keyState.Left)
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
void SelectScene::SetTotalCoins(const int& num)
{
	m_totalCoins = num;
}
