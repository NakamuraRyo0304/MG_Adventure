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

	GetSystemManager()->GetCamera()->SetEagleMode(false);	// カメラ視点移動

	// マップ読み込み
	m_blocks->Initialize(m_stageNum);
	
	// 見上げ距離
	m_changeMapMove = UP_VALUE;
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
	if (m_changeMapMove > 0.0f)
	{
		m_changeMapMove--;
	}
	else
	{
		m_changeMapMove = 0.0f;
	}


	// ステージ番号変更
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Right))
	{
		m_changeMapMove = UP_VALUE;
		m_stageNum++;

		// TODO: [ステージ番号]マップ数はMAX_STAGE_NUMを変更！
		m_stageNum = UserUtility::Clamp(m_stageNum, 0, MAX_STAGE_NUM);

		m_blocks->Initialize(m_stageNum);
	}
	if (GetSystemManager()->GetStateTrack()->IsKeyPressed(Keyboard::Left))
	{
		m_changeMapMove = UP_VALUE;
		m_stageNum--;

		// TODO: [ステージ番号]マップ数はMAX_STAGE_NUMを変更！
		m_stageNum = UserUtility::Clamp(m_stageNum, 0, MAX_STAGE_NUM);

		m_blocks->Initialize(m_stageNum);
	}
	

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		if (m_stageNum == 0)
		{
			// 0番目を選択したらステージエディタのシーンへ飛ぶ
			GoNextScene(SCENE::EDIT);
		}
		else
		{
			GoNextScene(SCENE::PLAY);
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
	SimpleMath::Matrix world, view, proj;

	// ワールド行列
	world = SimpleMath::Matrix::Identity;

	// ビュー行列
	SimpleMath::Vector3    eye(CAMERA_RADIUS * sin(m_timer), 30.0f, CAMERA_RADIUS * cos(m_timer) * 2.0f);
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, m_changeMapMove, 0.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

	// プロジェクション行列
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// マップの描画
	m_blocks->Render(context, states, view, proj, m_timer);

	// スカイドームの描画
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(m_timer * SKY_ROT_SPEED);
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);


	//-------------------------------------------------------------------------------------// 

	// テキストの移動アニメーション
	SimpleMath::Matrix stageMat = SimpleMath::Matrix::Identity;
	stageMat *= SimpleMath::Matrix::CreateRotationY(m_timer);
	stageMat *= SimpleMath::Matrix::CreateScale(10.0f);
	stageMat *= SimpleMath::Matrix::CreateTranslation(0.0f, 10.0f, cosf(m_timer) * 2.0f);

	// ステージ番号表示
	switch (m_stageNum)
	{
	case 0:
		m_editModel->Draw(context, states, stageMat, view, proj);
		return;
	case 1:
		m_stage1Model->Draw(context, states, stageMat, view, proj);
		return;
	case 2:
		m_stage2Model->Draw(context, states, stageMat, view, proj);
		return;
	case 3:
		m_stage3Model->Draw(context, states, stageMat, view, proj);
		return;
	case 4:
		m_stage4Model->Draw(context, states, stageMat, view, proj);
		return;
	case 5:
		m_stage5Model->Draw(context, states, stageMat, view, proj);
		return;
	default:
		break;
	}

}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void SelectScene::Finalize()
{
	// マップの後処理
	m_blocks->Finalize();

	// モデル削除
	ModelFactory::DeleteModel(m_stage1Model);
	ModelFactory::DeleteModel(m_stage2Model);
	ModelFactory::DeleteModel(m_stage3Model);
	ModelFactory::DeleteModel(m_stage4Model);
	ModelFactory::DeleteModel(m_stage5Model);
	ModelFactory::DeleteModel(m_editModel);
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

	// ステージ番号のモデル
	m_editModel   = ModelFactory::GetCreateModel(device, L"Resources/Models/StageEdit.cmo");
	m_stage1Model = ModelFactory::GetCreateModel(device, L"Resources/Models/Stage1.cmo");
	m_stage2Model = ModelFactory::GetCreateModel(device, L"Resources/Models/Stage2.cmo");
	m_stage3Model = ModelFactory::GetCreateModel(device, L"Resources/Models/Stage3.cmo");
	m_stage4Model = ModelFactory::GetCreateModel(device, L"Resources/Models/Stage4.cmo");
	m_stage5Model = ModelFactory::GetCreateModel(device, L"Resources/Models/Stage5.cmo");
}