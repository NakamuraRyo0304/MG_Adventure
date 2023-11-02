/*
 *	@File	TitleScene.cpp
 *	@Brief	タイトルシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

// タイトルロゴ
#include "Objects/Logo.h"

// ステージ
#include "Objects/Stage.h"

// スカイドーム
#include "Objects/TitleSky.h"

// UI
#include "Objects/TitleUI.h"

#include "TitleScene.h"

// コンストラクタ
TitleScene::TitleScene()
	: IScene()					// 基底クラスの初期化
	, m_cameraMoveY{0.0f}		// カメラ演出(スタート時)
	, is_startFlag{ false }		// 開始フラグ
	, is_menuFlag{ true }		// 選択フラグ
{
}

// デストラクタ
TitleScene::~TitleScene()
{
	Finalize();
}

// 初期化処理
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

// 更新処理
void TitleScene::Update()
{
	// インプットの更新
	auto& _input = Input::GetInstance();

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// サウンドの更新
	GetSystemManager()->GetSoundManager()->Update();

	// エスケープで終了
	if(_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)){ ChangeScene(SCENE::ENDGAME);}

	// ロゴの更新
	m_logo->Update();

	// 選択項目を変更する
	if (!is_startFlag)
	{
		if (GetFadeValue() >= 0.7f) return;

		if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Left) ||
			_input.GetKeyTrack()->IsKeyReleased(Keyboard::Right) ||
			_input.GetKeyTrack()->IsKeyReleased(Keyboard::A) ||
			_input.GetKeyTrack()->IsKeyReleased(Keyboard::D))
		{
			is_menuFlag = !is_menuFlag;
			m_stage->AddAccelerate();
			GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
		}
	}

	// 遷移先を決定する(ゲーム開始 OR ゲーム終了)
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Space))
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

	// ステージの更新
	m_stage->Update();

	// UIの更新
	m_titleUI->Update(is_menuFlag);

}

// 描画処理
void TitleScene::Draw()
{
	// 描画関連
	auto& _states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	SimpleMath::Matrix _view, _projection;

	// スタート演出の処理はこの中を処理する
	if (is_startFlag)
	{
		m_titleSky->SetStartFlag(true);
		m_logo->SetStartFlag(true);
	}

	// ビュー行列
	SimpleMath::Vector3 eye(0.0f, 0.1f + m_cameraMoveY, 8.0f);
	m_titleSky->SetPositionY(eye.y);
	_view = SimpleMath::Matrix::CreateLookAt(eye, SimpleMath::Vector3::Zero, SimpleMath::Vector3::Up);

	// プロジェクション行列
	_projection = GetSystemManager()->GetCamera()->GetProjection();

	// ステージを描画
	m_stage->Render(_states, _view, _projection);

	// ロゴを描画
	m_logo->Render(_states, _view, _projection);

	// スカイドームを描画
	m_titleSky->Render(_states, _view, _projection);

	// UIの描画
	m_titleUI->Render(GetFadeValue(), m_logo->IsLogoEndFlag());
}

// 終了処理
void TitleScene::Finalize()
{
	// UIの終了処理
	m_titleUI->Finalize();
}

// 画面、デバイス依存の初期化
void TitleScene::CreateWindowDependentResources()
{
	// メイクユニーク
	GetSystemManager()->CreateUnique();

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(GetScreenSize().x, GetScreenSize().y, CAMERA_ANGLE);

	// UIの初期化
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_titleUI = std::make_unique<TitleUI>();
	m_titleUI->Create(GetSystemManager(), GetScreenSize());

	// タイトルロゴの設定
	m_logo = std::make_unique<Logo>(L"Resources/Models/TitleLogoVer2.cmo");

	// ステージの設定
	m_stage = std::make_unique<Stage>(L"Resources/Models/TitleStage.cmo");

	// スカイドームの設定
	m_titleSky = std::make_unique<TitleSky>(L"Resources/Models/ShineSky.cmo");
}

// シーン変数初期化関数
void TitleScene::SetSceneValues()
{
	// ゲームを開始/ゲームを終了
	is_menuFlag = true;
}

// スタート演出
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