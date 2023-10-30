/*
 *	@File	ResultScene.cpp
 *	@Brief  リザルトシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

 // CSV読み込み
#include "../../Libraries/SystemDatas/MapLoad.h"

// ブロック
#include "../PlayScene/Objects/Blocks.h"

// UI
#include "Objects/ResultUI.h"

// ランダム(演出用なのでランダムデバイスは使わない)
#include <random>
#include <time.h>

#include "ResultScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
ResultScene::ResultScene()
	: IScene()					// 基底クラスの初期化
	, m_timer{0.0f}				// 時計
	, m_windowSize{}			// ウィンドウサイズ
	, m_coinNum{}				// コインの数
	, m_clearTime{0.0f}			// クリアタイムを格納
	, m_saveTime{0.0f}			// クリアタイムを保存する変数
	, m_directionTime{0.0f}		// 演出する時間
	, m_stageNum{1}				// 背景のステージ番号(初期化で1)
	, m_selectingScene{ 0 }		// 現在選択中のシーン
{
	// ランダムの生成
	srand(unsigned int(time(0)));
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
ResultScene::~ResultScene()
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// カメラ視点移動
	GetSystemManager()->GetCamera()->SetEagleMode(false);

	// 変数の初期化
	SetSceneValues();

	// BGMを鳴らす
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_RESULT, true);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">時間/fps</param>
/// <param name="keyState">キーボードポインタ</param>
/// <param name="mouseState">マウスポインタ</param>
/// <returns>なし</returns>
void ResultScene::Update(const float& elapsedTime, Keyboard::State& keyState,
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

	// 数字のアニメーション
	AnimationValue();

	// メニューセレクト
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Right) ||
	    GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::D))
	{
		// フェード中は処理しない
		if (GetFadeValue() >= 0.7f) return;

		m_selectingScene++;
		m_selectingScene = m_selectingScene ==  3 ? RETRY : m_selectingScene;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
	}
	else if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Left) ||
			 GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::A))
	{
		// フェード中は処理しない
		if (static_cast<int>(GetFadeValue()) != 0) return;

		m_selectingScene--;
		m_selectingScene = m_selectingScene == -1 ? TITLE : m_selectingScene;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
	}

	// UIの更新
	m_userInterface->Update(elapsedTime, static_cast<int>(m_clearTime));
	// 現在選択中のシーンをセット
	m_userInterface->SetSelecting(m_selectingScene);
	// 獲得コイン数をセット
	m_userInterface->SetCoins(m_coinNum);

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		// フェード中は処理しない
		if (static_cast<int>(GetFadeValue()) != 0) return;

		switch (m_selectingScene)
		{
		case RETRY:
			ChangeScene(SCENE::PLAY);
			break;
		case SELECT:
			ChangeScene(SCENE::SELECT);
			break;
		case TITLE:
			ChangeScene(SCENE::TITLE);
			break;
		default:
			break;
		}

		// 決定音を鳴らす
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}
	// エスケープで終了
	GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Escape) ? ChangeScene(SCENE::ENDGAME) : void();
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::Draw()
{
	// 描画関連
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& _states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	SimpleMath::Matrix  _view, _projection;

	// ビュー行列
	SimpleMath::Vector3    _eye(cosf(m_timer), 20.0f + sinf(m_timer) * 2.0f, 10.0f);
	SimpleMath::Vector3     _up(0.0f, 5.0f, 0.0f);
	SimpleMath::Vector3 _target(0.0f, -10.0f, -5.0f);

	_view = SimpleMath::Matrix::CreateLookAt(_eye, _target, _up);

	// プロジェクション行列
	_projection = GetSystemManager()->GetCamera()->GetProjection();

	// マップの描画
	m_blocks->Render(_context, _states, _view, _projection, m_timer, SimpleMath::Vector3{ 1.0f,-1.0f,-1.0f });

	// UIの表示
	m_userInterface->Render(GetFadeValue());
}


/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::Finalize()
{
	// マップの後処理
	m_blocks->Finalize();

	// UIの後処理
	m_userInterface->Finalize();
}

/// <summary>
/// 画面依存、デバイス依存の初期化
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::CreateWindowDependentResources()
{
	// デバイスとデバイスコンテキストの取得
	auto _device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto _context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique(_device, _context);

	// 画面サイズの格納
	float _width  = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float _height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// ウィンドウサイズを取得
	m_windowSize = SimpleMath::Vector2{ _width,_height };

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(_width, _height, 45.0f);

	// ブロックの作成
	m_blocks = std::make_unique<Blocks>();

	// 草ブロックの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(_device, L"Resources/Models/GrassBlock.cmo")),
		m_blocks->GRASS
	);
	// コインの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(_device, L"Resources/Models/Coin.cmo")),
		m_blocks->COIN
	);
	// 雲ブロックの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(_device, L"Resources/Models/Cloud.cmo")),
		m_blocks->CLOWD
	);
	// 重力ブロックの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(_device, L"Resources/Models/ResetPt.cmo")),
		m_blocks->GRAVITY
	);

	// UIの作成
	m_userInterface = std::make_unique<ResultUI>(GetSystemManager(), _context, _device);
	m_userInterface->Initialize();

}

/// <summary>
/// シーン内の変数初期化関数
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::SetSceneValues()
{
	// タイマーの保存
	m_saveTime = m_clearTime;

	// 演出時間 最初のフェードも考慮して多めに取る
	m_directionTime = 120.0f;

	// マップ読み込み
	m_blocks->Initialize(m_stageNum);
}

/// <summary>
/// 数字のアニメーション
/// </summary>
/// <param name="引数無し"></param>
/// <returns>終わったらTrue</returns>
bool ResultScene::AnimationValue()
{
	// 演出時間をカウント
	m_directionTime--;

	// 演出をする
	if (m_directionTime < 0.0f)
	{
		m_directionTime = 0.0f;
		m_clearTime = MAX_TIME - m_saveTime;

		return true;
	}
	else
	{
		// ランダムな値を入れる
		m_clearTime = static_cast<float>(rand() % 60 + 1);
	}

	return false;
}

/// <summary>
/// コインのセッター
/// </summary>
/// <param name="coinNum">コイン数</param>
/// <returns>なし</returns>
void ResultScene::SetCoinNum(const int& coinNum)
{
	m_coinNum = coinNum;
}
