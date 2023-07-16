/*
 *	@File	GameMain.cpp
 *	@Brief	ゲームループを扱うクラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "GameMain.h"

// フェードオブジェクト
#include "../Libraries/SystemDatas/Fade.h"

// TODO: シーン２：シーンのインクルード
#include "Game/TitleScene/TitleScene.h"
#include "Game/SelectScene/SelectScene.h"
#include "Game/EditScene/EditScene.h"
#include "Game/PlayScene/PlayScene.h"
#include "Game/ResultScene/ResultScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
GameMain::GameMain():
	m_nextScene{ SCENE::SELECT },
	m_prevScene{ SCENE::NONE },
	m_nowScene{ nullptr },
	m_num{1},
	m_clearTime{0.0f},
	m_screenWidth{},
	m_screenHeight{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
GameMain::~GameMain()
{
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::Initialize()
{
	// シーン作成
	CreateScene();

	// フェードイン
	m_fade->SetFadeIn();

	// ステージ番号
	m_num = 1;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">時間を更新するタイマー</param>
/// <returns>なし</returns>
void GameMain::Update(const DX::StepTimer& timer)
{
	// フェードの更新
	m_fade->Update();

	// 時間を更新する
	float time = static_cast<float>(timer.GetTotalSeconds());

	// キー入力情報を取得する
	auto kb = Keyboard::Get().GetState();
	m_keyboardStateTracker->Update(kb);

	// マウス入力情報を取得する
	auto ms = Mouse::Get().GetState();
	m_mouseStateTracker->Update(ms);

	// エスケープでゲーム終了
	if (kb.Escape)
	{
		m_nowScene->ExitApp();
	}

	// 次のシーンが設定されていたらシーン切り替え
	if (m_nextScene != SCENE::NONE && m_fade->GetEndFlag())
	{
		// シーン削除
		DeleteScene();

		// シーン作成
		CreateScene();
	}

	// 実体があれば更新
	if (m_nowScene != nullptr)
	{
		// シーンの更新処理
		m_nowScene->Update(time, kb, ms);

			// フェードが終わっていたらシーンを切り替える
		if (m_fade->GetEndFlag())
		{
			m_nextScene = m_nowScene->GetNextScene();
		}
		else
		{
			m_nowScene->StopNextScene();
		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::Draw()
{
	// 実態があれば描画
	if (m_nowScene != nullptr)
	{
		m_nowScene->Draw();
	}

	// フェードの描画
	m_fade->Draw();
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::Finalize()
{
	// シーン削除
	DeleteScene();
}

/// <summary>
/// シーンの新規作成
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::CreateScene()
{
	// シーンが作成されているときは処理しない
	if (m_nowScene != nullptr) return;
	
	// TODO: シーン３：シーンはここから追加
	switch (m_nextScene)
	{
		case SCENE::TITLE:		// タイトルシーン
		{
			m_nowScene = std::make_unique<TitleScene>();

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::SELECT:		// ステージセレクトシーン
		{
			m_nowScene = std::make_unique<SelectScene>();

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::PLAY:		// ゲームシーン
		{
			m_nowScene = std::make_unique<PlayScene>();

			// 選択したステージを渡す
			CastSceneType<PlayScene>(m_nowScene)->SetStageNum(m_num);
			m_fade->SetFadeSpeed(PLAY_FADE_SPEED);
			break;
		}
		case SCENE::RESULT:		// リザルトシーン
		{
			m_nowScene = std::make_unique<ResultScene>();

			// クリアタイムを渡す
			CastSceneType<ResultScene>(m_nowScene)->SetStageNum(m_num);
			CastSceneType<ResultScene>(m_nowScene)->SetClearTime(m_clearTime);
			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::EDIT:		// ステージエディットシーン
		{
			m_nowScene = std::make_unique<EditScene>();

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		default:
			break;	
	}

	// 作成したシーンを初期化
	m_nowScene->Initialize();

	// フェードイン
	m_fade->SetFadeIn();

	// 次へのシーン情報を初期化
	m_nextScene = SCENE::NONE;
}
/// <summary>
/// シーンの削除
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::DeleteScene()
{
	// シーンが作成されていなければ処理しない
	if (!m_nowScene) return;

	// 現在がセレクトシーンならステージ番号を保持
	if (m_nextScene == SCENE::PLAY)
	{
		// 再読み込みでなければ処理する
		if (m_nextScene != m_prevScene && m_prevScene != SCENE::RESULT)
		{
			m_num = CastSceneType<SelectScene>(m_nowScene)->GetStageNum();
		}
	}

	// 現在がプレイシーンならタイムリミットを保持
	if (m_nextScene == SCENE::RESULT)
	{
		if (CastSceneType<ResultScene>(m_nowScene) == nullptr)
		{
			m_clearTime = CastSceneType<PlayScene>(m_nowScene)->GetTimeLimit();
		}
	}

	// 現シーンの終了処理
	if (m_fade->GetFadeNum() >= m_fade->GetMaxValue())
	{
		// 現シーンの完全削除
		m_nowScene->Finalize();
		m_nowScene.reset();

		// 前のシーンを保存
		m_prevScene = m_nextScene;
	}
	else // フェードアウト
	{
		m_fade->SetFadeOut();
	}
}

/// <summary>
/// 画面依存、デバイス依存の初期化
/// </summary>
/// <param name="screenWidth">画面横幅</param>
/// <param name="screenHeight">画面縦幅</param>
/// <returns>なし</returns>
void GameMain::CreateWindowDependentResources(const int& screenWidth, const int& screenHeight)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	auto device = pDR->GetD3DDevice();
	auto context = pDR->GetD3DDeviceContext();

	// スクリーンサイズの設定
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// キーボード関連
	m_keyboard = std::make_unique<Keyboard>();
	m_keyboardStateTracker = std::make_unique<Keyboard::KeyboardStateTracker>();

	// マウス関連
	m_mouse = std::make_unique<Mouse>();
	m_mouseStateTracker = std::make_unique<Mouse::ButtonStateTracker>();
	m_mouse->SetWindow(pDR->GetHwnd());

	// フェードオブジェクトの初期化
	m_fade = std::make_unique<Fade>(DEFAULT_FADE_SPEED);
	m_fade->Initialize(context, device);
}