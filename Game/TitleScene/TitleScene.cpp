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
	m_timer{0.0f},				// タイマー
	m_titleLogoModel{},			// タイトルロゴのモデル
	m_miniatureModel{},			// 中央ステージのモデル
	m_cameraMoveY{0.0f},		// カメラ演出(スタート時)
	m_logoMoveY{0.0f},			// ロゴの動き(移動)
	m_logoMoveScale{},			// ロゴの動き(サイズ)
	is_startFlag{false},		// 開始フラグ
	is_menuFlag{true},			// 選択フラグ
	m_accelerate{0.0f},			// 選択変更時のステージの回転加速
	is_accelerateFlag{false}	// 回転加速しているかの判定フラグ
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
	m_logoMoveY = 10.0f;
	m_logoMoveScale = 1.0f;

	// ゲームを開始/ゲームを終了
	is_menuFlag = true;

	// 加速度を初期化
	m_accelerate = 0.0f;

	// BGMを鳴らす
	GetSystemManager()->GetSoundManager()->SetSound(XACT_WAVEBANK_SKBX_BGM_TITLESELECT, true);
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

	// サウンドの更新
	GetSystemManager()->GetSoundManager()->Update();

	// 起動時のロゴの動き
	m_logoMoveY = UserUtility::Lerp(m_logoMoveY, END_MOVE_POS, 0.1f);

	// 決定していなければ選択を切り替える
	if (!is_startFlag)
	{
		// 選択項目を変更する
		if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Left) ||
			GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Right))
		{
			is_menuFlag = !is_menuFlag;
			is_accelerateFlag = true;
		}

		// 最大まで回転したら回転をやめる
		is_accelerateFlag = m_accelerate >= MAX_ACCELERATE_TIME ? false : is_accelerateFlag;
		// 回転フラグが立っていたら加算
		m_accelerate += is_accelerateFlag ? 0.1f : 0.0f;
		// 回転フラグが立っていなければ初期化する
		m_accelerate = !is_accelerateFlag ? 0.0f : m_accelerate;
	}

	// 遷移先を決定する(ゲーム開始 OR ゲーム終了)
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		is_startFlag = true;
	}
	// 演出が終わったかどうか判定する
	if (FlyStartObjects())
	{
		// Startを選んだらセレクトへ移行、Exitを選んだらゲームを終了
		ChangeScene(is_menuFlag ? SCENE::SELECT : SCENE::ENDGAME);
	}

	// UIの更新
	m_titleUI->Update(is_menuFlag);

	// エスケープで終了
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape))
	{
		ChangeScene(SCENE::ENDGAME);
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

	//-------------------------------------------------------------------------------------//

	// 移動、回転行列
	SimpleMath::Matrix logoTrans, logoRot,				// ロゴ
		stageTrans, stageRotX, stageRotY, stageScale,	// ステージ
		skyTrans, skyRotX;								// スカイドーム

	// ワールド行列
	logoMat = stageMat = skyMat = SimpleMath::Matrix::Identity;

	//-------------------------------------------------------------------------------------//

	// 回転行列
	logoRot = SimpleMath::Matrix::CreateRotationX(sinf(m_timer) * 0.5f);
	stageRotX = SimpleMath::Matrix::CreateRotationX(0.3f);
	stageRotY = SimpleMath::Matrix::CreateRotationY(m_timer * 0.5f + m_accelerate);
	skyRotX = SimpleMath::Matrix::CreateRotationX(m_timer * 2.0f);

	//-------------------------------------------------------------------------------------//

	// 移動行列
	logoTrans = SimpleMath::Matrix::CreateTranslation(0.0f, m_logoMoveY, cosf(m_timer) * 0.5f);
	stageTrans = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -10.0f);
	skyTrans = SimpleMath::Matrix::CreateTranslation(0.0f, m_cameraMoveY, 0.0f);

	//-------------------------------------------------------------------------------------//

	// スケール行列
	stageScale = SimpleMath::Matrix::CreateScale(1.2f);

	//-------------------------------------------------------------------------------------//

	// ロゴ
	logoMat *= logoRot * logoTrans;
	// ステージ
	stageMat *= stageScale * stageRotY * stageRotX * stageTrans;
	// スカイドーム
	skyMat *= skyRotX;

	//-------------------------------------------------------------------------------------//

	// スタート演出の処理はこの中を処理する
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
	SimpleMath::Vector3 eye(0.0f, 0.1f + m_cameraMoveY, 8.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, SimpleMath::Vector3::Zero, SimpleMath::Vector3::Up);

	// プロジェクション行列
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// ライトの設定
	SimpleMath::Vector3	 lightDirection(-1.0f, 1.0f, 1.0f);
	SimpleMath::Color	 lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	// アマチュアモデル設定
	m_miniatureModel->UpdateEffects([&](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				for (int i = 0; i < 3; ++i)
				{
					// ライトの使用を設定
					lights->SetLightEnabled(i, true);

					// ライトの方向を設定
					lights->SetLightDirection(i, lightDirection);

					// ライトの色を設定
					lights->SetLightDiffuseColor(i, lightColor);
				}
			}
		});
	m_miniatureModel->Draw(context, states, stageMat, view, proj);	// ステージ
	m_titleLogoModel->Draw(context, states, logoMat, view, proj);	// ロゴ
	m_skyDomeModel->Draw(context, states, skyMat, view, proj);  	// スカイドーム

	// UIの描画
	if (static_cast<int>(m_logoMoveY) == static_cast<int>(END_MOVE_POS))
	{
		m_titleUI->Render();
	}
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
	m_titleLogoModel = ModelFactory::GetCreateModel(device, L"Resources/Models/TitleLogoVer2.cmo");
	m_titleLogoModel->UpdateEffects([](IEffect* effect)
		{
			// ライティング
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				// ライトの指定
				lights->SetLightEnabled(0, true);
				lights->SetLightEnabled(1, true);
				lights->SetLightEnabled(2, false);

				// ライトの方向を設定
				SimpleMath::Vector3 lightDirection = -SimpleMath::Vector3::UnitZ;
				lights->SetLightDirection(0, lightDirection);
				lights->SetLightDirection(1, lightDirection);
			}
		}
	);

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

/// <summary>
/// スタートを押したときの演出
/// </summary>
/// <param name="引数無し"></param>
/// <returns>演出完了でTrueを返す</returns>
bool TitleScene::FlyStartObjects()
{
	if (is_startFlag)
	{
		m_cameraMoveY++;

		// 演出が終わったら遷移
		if (m_cameraMoveY > MAX_HEIGHT)
		{
			return true;
		}
	}

	return false;
}
