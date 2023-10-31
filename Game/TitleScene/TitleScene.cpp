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
TitleScene::TitleScene()
	: IScene()					// 基底クラスの初期化
	, m_titleLogoModel{}		// タイトルロゴのモデル
	, m_miniatureModel{}		// 中央ステージのモデル
	, m_cameraMoveY{0.0f}		// カメラ演出(スタート時)
	, m_logoMoveY{0.0f}			// ロゴの動き(移動)
	, m_logoMoveScale{}			// ロゴの動き(サイズ)
	, m_accelerate{ 0.0f }		// 選択変更時のステージの回転加速
	, is_startFlag{ false }		// 開始フラグ
	, is_menuFlag{ true }		// 選択フラグ
	, is_accelerateFlag{ false }// 回転加速しているかの判定フラグ
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

	// 変数の初期化
	SetSceneValues();

	// BGMを鳴らす
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_TITLESELECT, true);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleScene::Update()
{
	// インプットの更新
	auto _key = Keyboard::Get().GetState();
	GetSystemManager()->GetStateTrack()->Update(_key);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// サウンドの更新
	GetSystemManager()->GetSoundManager()->Update();

	// エスケープで終了
	if(GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::ENDGAME);}

	// 起動時のロゴの動き
	m_logoMoveY = UserUtility::Lerp(m_logoMoveY, END_MOVE_POS, 0.1f);

	// 選択項目を変更する
	if (!is_startFlag)
	{
		if (GetFadeValue() >= 0.7f) return;

		if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Left) ||
			GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Right) ||
			GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::A) ||
			GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::D))
		{
			is_menuFlag = !is_menuFlag;
			is_accelerateFlag = true;
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
		}

		// 変更時の動き
		is_accelerateFlag = m_accelerate >= MAX_ACCELERATE_TIME ? false : is_accelerateFlag;
		m_accelerate += is_accelerateFlag ? 0.1f : 0.0f;
		m_accelerate = !is_accelerateFlag ? 0.0f : m_accelerate;
	}

	// 遷移先を決定する(ゲーム開始 OR ゲーム終了)
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		// フェード中は処理しない
		if (GetFadeValue() >= 0.7f) return;
		is_startFlag = true;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}
	// セレクト
	if (FlyStartObjects())
	{
		ChangeScene(is_menuFlag ? SCENE::SELECT : SCENE::ENDGAME);
	}

	// UIの更新
	m_titleUI->Update(is_menuFlag);

}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleScene::Draw()
{
	// 描画関連
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& _states = *GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// カメラ用行列
	SimpleMath::Matrix _logoMat, _stageMat, _skyMat, _view, _projection;

	// 移動、回転行列
	SimpleMath::Matrix
		_logoTrans, _logoRot,								// ロゴ
		_stageTrans, _stageRotX, _stageRotY, _stageScale,	// ステージ
		_skyTrans, _skyRotX;								// スカイドーム

	// ワールド行列
	_logoMat = _stageMat = _skyMat = SimpleMath::Matrix::Identity;

	//-------------------------------------------------------------------------------------//

	// 回転行列
	_logoRot = SimpleMath::Matrix::CreateRotationX(sinf(_timer) * 0.5f);
	_stageRotX = SimpleMath::Matrix::CreateRotationX(0.3f);
	_stageRotY = SimpleMath::Matrix::CreateRotationY(_timer * 0.5f + m_accelerate);
	_skyRotX = SimpleMath::Matrix::CreateRotationX(_timer * 2.0f);

	//-------------------------------------------------------------------------------------//

	// 移動行列
	_logoTrans = SimpleMath::Matrix::CreateTranslation(0.0f, m_logoMoveY, cosf(_timer) * 0.5f);
	_stageTrans = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -10.0f);
	_skyTrans = SimpleMath::Matrix::CreateTranslation(0.0f, m_cameraMoveY, 0.0f);

	//-------------------------------------------------------------------------------------//

	// スケール行列
	_stageScale = SimpleMath::Matrix::CreateScale(1.2f);

	//-------------------------------------------------------------------------------------//

	// ロゴ
	_logoMat *= _logoRot * _logoTrans;
	// ステージ
	_stageMat *= _stageScale * _stageRotY * _stageRotX * _stageTrans;
	// スカイドーム
	_skyMat *= _skyRotX;

	//-------------------------------------------------------------------------------------//

	// スタート演出の処理はこの中を処理する
	if (is_startFlag)
	{
		// スカイドームの回転と移動
		_skyMat *= _skyRotX;
		_skyMat *= _skyTrans;

		// ロゴも一緒に動く
		m_logoMoveScale *= LOGO_CHANGE_SCALE;
		_logoMat *= SimpleMath::Matrix::CreateScale(m_logoMoveScale);
	}

	// ビュー行列
	SimpleMath::Vector3 eye(0.0f, 0.1f + m_cameraMoveY, 8.0f);
	_view = SimpleMath::Matrix::CreateLookAt(eye, SimpleMath::Vector3::Zero, SimpleMath::Vector3::Up);

	// プロジェクション行列
	_projection = GetSystemManager()->GetCamera()->GetProjection();

	// ライトの設定
	SimpleMath::Vector3	 _lightDir(-1.0f, 1.0f, 1.0f);
	SimpleMath::Color	 _lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	// アマチュアモデル設定
	m_miniatureModel->UpdateEffects([&](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				for (int i = 0; i < 3; ++i)
				{
					// ライトの使用を設定
					_lights->SetLightEnabled(i, true);

					// ライトの方向を設定
					_lights->SetLightDirection(i, _lightDir);

					// ライトの色を設定
					_lights->SetLightDiffuseColor(i, _lightColor);
				}
			}
		});
	m_miniatureModel->Draw(_context, _states, _stageMat, _view, _projection);	// ステージ
	m_titleLogoModel->Draw(_context, _states, _logoMat, _view, _projection);	// ロゴ
	m_skyDomeModel->Draw(_context, _states, _skyMat, _view, _projection);  		// スカイドーム

	// UIの描画
	m_titleUI->Render(GetFadeValue(), static_cast<int>(m_logoMoveY) == static_cast<int>(END_MOVE_POS));
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
	auto _device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique();

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// UIの初期化
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch(_context);
	m_titleUI = std::make_unique<TitleUI>();
	m_titleUI->Create(GetSystemManager(), _device, GetScreenSize());

	// モデルの作成---------------------------------------------------------------------------------

	// タイトルロゴ
	m_titleLogoModel = ModelFactory::GetCreateModel(_device, L"Resources/Models/TitleLogoVer2.cmo");
	m_titleLogoModel->UpdateEffects([](IEffect* effect)
		{
			// ライティング
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				// ライトの指定
				_lights->SetLightEnabled(0, true);
				_lights->SetLightEnabled(1, true);
				_lights->SetLightEnabled(2, false);

				// ライトの方向を設定
				SimpleMath::Vector3 _lightDir = -SimpleMath::Vector3::UnitZ;
				_lights->SetLightDirection(0, _lightDir);
				_lights->SetLightDirection(1, _lightDir);
			}
		}
	);

	// ステージモデル
	m_miniatureModel = ModelFactory::GetCreateModel(_device, L"Resources/Models/TitleStage.cmo");

	// スカイドーム
	m_skyDomeModel = ModelFactory::GetCreateModel(_device, L"Resources/Models/ShineSky.cmo");
	m_skyDomeModel->UpdateEffects([](IEffect* effect)
		{
			// ライティング
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				_lights->SetLightEnabled(0, false);
				_lights->SetLightEnabled(1, true);
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
}

/// <summary>
/// シーン内の変数初期化関数
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void TitleScene::SetSceneValues()
{
	// ロゴの大きさ
	m_logoMoveY = 10.0f;
	m_logoMoveScale = 1.0f;

	// ゲームを開始/ゲームを終了
	is_menuFlag = true;

	// 加速度を初期化
	m_accelerate = 0.0f;
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