/*
 *	@File	SelectScene.cpp
 *	@Brief  セレクトシーン。
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

// マップローダー
#include "Libraries/SystemDatas/MapLoad.h"

// ブロックの作成
#include "../PlayScene/Objects/Blocks.h"

#include "SelectScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
SelectScene::SelectScene():
	IScene(),
	m_timer{},
	m_flashCount{},
	m_stageNum{1},
	m_changeMapMove{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
SelectScene::~SelectScene()
{
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

	// 見上げ距離
	m_changeMapMove = UP_VALUE;

	// スタートが0
	m_flashCount = 0.0f;
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

	// 動きが終わっていなければ見下げる
	if (m_changeMapMove > 1.0f)
	{
		m_changeMapMove -= DOWN_SPEED;
	}
	else if(m_changeMapMove > 0.0f)
	{
		m_changeMapMove -= DOWN_SPEED / 2;
	}
	else
	{
		m_changeMapMove = 0.0f;
	}


	// ステージ番号変更
	if (static_cast<int>(m_changeMapMove) >= UP_VALUE / 2) return;

	// フラッシュカウンタ
	m_flashCount++;
	m_flashCount = m_flashCount > MAX_FLASH * 2 / 3 ? 0.0f : m_flashCount;

	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Right))
	{
		m_changeMapMove = UP_VALUE;
		m_stageNum++;
		m_flashCount = 0.0f;

		// TODO: [ステージ番号]マップ数はMAX_STAGE_NUMを変更！
		m_stageNum = UserUtility::Clamp(m_stageNum, 0, MAX_STAGE_NUM - 1);
	}
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Left))
	{
		m_changeMapMove = UP_VALUE;
		m_stageNum--;
		m_flashCount = 0.0f;

		// TODO: [ステージ番号]マップ数はMAX_STAGE_NUMを変更！
		m_stageNum = UserUtility::Clamp(m_stageNum, 0, MAX_STAGE_NUM - 1);
	}

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		if (m_stageNum == 0)
		{
			// 0番目を選択したらステージエディタのシーンへ飛ぶ
			ChangeScene(SCENE::EDIT);
		}
		else
		{
			ChangeScene(SCENE::PLAY);
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
	float verticalValue = sinf(m_timer * 2.0f) * 2.0f;

	// ビュー行列
	SimpleMath::Vector3    eye(CAMERA_RADIUS * sin(rotValue), 30.0f + verticalValue, CAMERA_RADIUS * cos(rotValue));
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, m_changeMapMove, 0.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

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

	// 点滅させる
	if (static_cast<int>(m_flashCount) > static_cast<int>(MAX_FLASH) / 2) return;

	// テキストの移動アニメーション
	SimpleMath::Matrix stageMat = SimpleMath::Matrix::Identity;
	stageMat *= SimpleMath::Matrix::CreateRotationY(rotValue);
	stageMat *= SimpleMath::Matrix::CreateScale(10.0f);
	stageMat *= SimpleMath::Matrix::CreateTranslation(sinf(rotValue), 10.0f, cosf(rotValue));


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
	// マップの後処理
	// モデル削除
	for (int i = 0; i < MAX_STAGE_NUM; ++i)
	{
		m_blocks[i]->Finalize();
		ModelFactory::DeleteModel(m_stageModels[i]);
	}
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

	// ブロックの作成を裏で処理
	m_loadTask = std::async(std::launch::async, [&]() { LoadStage(device); });

	// ステージ番号のモデル
	for (int i = 0; i < MAX_STAGE_NUM; i++)
	{
		// 0番目はエディタの文字
		if (i == 0)
		{
			m_stageModels[0] = ModelFactory::GetCreateModel(device, L"Resources/Models/StageEdit.cmo");
		}
		else
		{
			std::wstring stageNumber = std::to_wstring(i);
			std::wstring path = L"Resources/Models/Stage" + stageNumber + L".cmo";
			m_stageModels[i] = ModelFactory::GetCreateModel(device, path.c_str());
		}
	}
}

/// <summary>
/// ステージモデルのロード
/// </summary>
/// <param name="device">デバイスポインタ</param>
/// <returns>なし</returns>
void SelectScene::LoadStage(ID3D11Device1* device)
{
	for (int i = 0; i < MAX_STAGE_NUM; ++i)
	{
		m_blocks[i] = std::make_unique<Blocks>();

		// 草ブロックの作成
		m_blocks[i]->CreateModels(
			std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo")),
			m_blocks[i]->GRASS
		);
		// コインの作成
		m_blocks[i]->CreateModels(
			std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo")),
			m_blocks[i]->COIN
		);
		// 雲ブロックの作成
		m_blocks[i]->CreateModels(
			std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/MoveBlock.cmo")),
			m_blocks[i]->CLOWD
		);
		// 雲リセットブロックの作成
		m_blocks[i]->CreateModels(
			std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo")),
			m_blocks[i]->RECLOWD
		);

		// 初期化処理
		m_blocks[i]->Initialize(i);
	}
}
