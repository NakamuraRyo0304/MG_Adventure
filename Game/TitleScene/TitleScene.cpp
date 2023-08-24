/*
 *	@File	TitleScene.cpp
 *	@Brief	タイトルシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Objects/TitleUI.h"

#include "TitleScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
TitleScene::TitleScene():
	IScene(),
	m_timer{0.0f},
	m_titleLogoModel{},
	m_miniatureModel{},
	m_moveY{0.0f},
	is_startFlag{false},
	m_logoMoveScale{},
	is_gameFlag{true}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
TitleScene::~TitleScene()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// カメラ視点移動
	GetSystemManager()->GetCamera()->SetEagleMode(false);
	// カメラ座標設定
	GetSystemManager()->GetCamera()->SetEyePosition(SimpleMath::Vector3(0.0f, -20.0f, -20.0f));

	// ロゴの大きさ
	m_logoMoveScale = 1.0f;

	// ゲームを開始/ゲームを終了
	is_gameFlag = true;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">時間/fps</param>
/// <param name="keyState">キーボードポインタ</param>
/// <param name="mouseState">マウスポインタ</param>
/// <returns>なし</returns>
void TitleScene::Update(const float& elapsedTime,Keyboard::State& keyState,
	Mouse::State& mouseState)
{
	m_timer = elapsedTime;

	// キー入力情報を取得する
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// 決定していなければ選択を切り替える
	if (!is_startFlag)
	{
		if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Left))
		{
			is_gameFlag = true;
		}
		if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Right))
		{
			is_gameFlag = false;
		}
	}

	// 遷移先を決定する(ゲーム開始 OR ゲーム終了)
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		is_startFlag = true;
	}
	if (is_startFlag)
	{
		m_moveY++;

		// 演出が終わったら遷移
		if (m_moveY > MAX_HEIGHT)
		{
			if (is_gameFlag)	// Startを選択したらセレクトシーンに行く
			{
				ChangeScene(SCENE::SELECT);
			}
			else				// Exitを選択したらゲームを終了する
			{
				ChangeScene(SCENE::ENDGAME);
			}
		}
	}

	// UIの更新
	m_titleUI->Update(is_gameFlag);

	// エスケープで終了
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape))
	{
		ExitApp();
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleScene::Draw()
{
	// 描画関連
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	SimpleMath::Matrix logoMat, stageMat, skyMat, view, proj;

	// 移動、回転行列
	SimpleMath::Matrix logoTrans, logoRot;
	SimpleMath::Matrix stageTrans, stageRotX, stageRotY, stageScale;
	SimpleMath::Matrix skyTrans,skyRotX;

	// ワールド行列
	logoMat = SimpleMath::Matrix::Identity;
	stageMat = SimpleMath::Matrix::Identity;
	skyMat = SimpleMath::Matrix::Identity;

	// 回転行列
	logoRot = SimpleMath::Matrix::CreateRotationX(sinf(m_timer) * 0.5f);
	stageRotX = SimpleMath::Matrix::CreateRotationX(0.3f);
	stageRotY = SimpleMath::Matrix::CreateRotationY(m_timer * 0.5f);
	skyRotX = SimpleMath::Matrix::CreateRotationX(m_timer * 2.0f);

	// 移動行列
	logoTrans = SimpleMath::Matrix::CreateTranslation(0.0f, 2.0f, cosf(m_timer) * 0.5f);
	stageTrans = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -10.0f);
	skyTrans = SimpleMath::Matrix::CreateTranslation(0.0f, m_moveY, 0.0f);

	// スケール行列
	stageScale = SimpleMath::Matrix::CreateScale(1.2f);

	// ロゴ
	logoMat *= logoRot * logoTrans;
	// ステージ
	stageMat *= stageScale * stageRotY * stageRotX * stageTrans;
	// スカイドーム
	skyMat *= skyRotX;

	// スタート演出の処理はこの中
	if (is_startFlag)
	{
		// スカイドームの回転と移動
		skyMat *= skyRotX;
		skyMat *= skyTrans;

		// ロゴも一緒に動く
		m_logoMoveScale *= 1.05f;
		logoMat *= SimpleMath::Matrix::CreateScale(m_logoMoveScale);
	}

	// ビュー行列
	SimpleMath::Vector3    eye(0.0f, 0.1f + m_moveY, 8.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, SimpleMath::Vector3::Zero, SimpleMath::Vector3::Up);

	// プロジェクション行列
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// ライトの設定
	SimpleMath::Vector3 lightDirection(-1.0f, 1.0f, 1.0f);
	SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	// アマチュアモデル設定
	m_miniatureModel->UpdateEffects([&](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				// ライトオン
				lights->SetLightEnabled(0, true);
				lights->SetLightEnabled(1, true);
				lights->SetLightEnabled(2, true);

				// ライトの方向を設定
				lights->SetLightDirection(0, lightDirection);
				lights->SetLightDirection(1, lightDirection);
				lights->SetLightDirection(2, lightDirection);

				// ライトの色を設定
				lights->SetLightDiffuseColor(0, lightColor);
				lights->SetLightDiffuseColor(1, lightColor);
				lights->SetLightDiffuseColor(2, lightColor);
			}
		});
	m_miniatureModel->Draw(context, states, stageMat, view, proj);	// ステージ
	m_titleLogoModel->Draw(context, states, logoMat, view, proj);	// ロゴ
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);  	// スカイドーム

	// UIの描画
	m_titleUI->Render();
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleScene::Finalize()
{
	// モデルの解放
	ModelFactory::DeleteModel(m_titleLogoModel);
	ModelFactory::DeleteModel(m_miniatureModel);
	ModelFactory::DeleteModel(m_skyDomeModel);

	// UIの終了処理
	m_titleUI->Finalize();
}

/// <summary>
/// 画面依存、デバイス依存の初期化
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleScene::CreateWindowDependentResources()
{
	// デバイスとデバイスコンテキストの取得
	auto device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique(device, context);
	GetSystemManager()->GetString()->CreateString(device, context);

	// 画面サイズの格納
	float width  =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height =
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, 45.0f);

	// モデルの作成---------------------------------------------------------------------------------

	// タイトルロゴ
	m_titleLogoModel = ModelFactory::GetCreateModel(device, L"Resources/Models/TitleLogo.cmo");

	// ステージモデル
	m_miniatureModel = ModelFactory::GetCreateModel(device, L"Resources/Models/TitleStage.cmo");

	// スカイドーム
	m_skyDomeModel = ModelFactory::GetCreateModel(device, L"Resources/Models/ShineSky.cmo");
	m_skyDomeModel->UpdateEffects([](IEffect* effect)
		{
			// ライティング
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, true);
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


	// UIの初期化
	m_titleUI = std::make_unique<TitleUI>(SimpleMath::Vector2{ width, height });
	m_titleUI->Create(GetSystemManager(), context, device);
}