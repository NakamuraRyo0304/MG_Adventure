/*
 *	@File	GameMain.cpp
 *	@Brief	ゲームループを扱うクラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "GameMain.h"

 // ファイルの保存に使用
#include<sstream>
#include<fstream>
#include<regex>

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
	m_nextScene{ SCENE::TITLE },		// 最初のシーン
	m_prevScene{ SCENE::NONE },			// 前回のシーン
	m_nowScene{ nullptr },				// 今のシーンポインタ
	m_num{1},							// ステージ番号
	m_clearTime{0.0f},					// クリアタイム
	m_coinNum{0},						// 獲得コイン数
	m_screenWidth{},					// スクリーンサイズ(横)
	m_screenHeight{},					// スクリーンサイズ(縦)
	is_saveOnce{false},
	//以下はセーブデータ対応------------------------------------------------------------------------//
	m_closeNum{},						// 未開放ステージ
	m_totalCoinNum{},					// 累計コイン枚数
	m_clearJudge{0}						// クリア判定
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

	is_saveOnce = false;

	for (int i = 0; i < 10; ++i)
	{
		m_clearJudge[i] = 0;
	}

	// セーブデータを読み込む
	LoadSaveData();
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
			CastSceneType<SelectScene>(m_nowScene)->SetNoStageNum(m_closeNum);
			CastSceneType<SelectScene>(m_nowScene)->SetTotalCoins(m_totalCoinNum);

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
			CastSceneType<ResultScene>(m_nowScene)->SetCoinNum(m_coinNum);
			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::EDIT:		// ステージエディットシーン
		{
			m_nowScene = std::make_unique<EditScene>();

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::ENDGAME:	// ゲーム終了
		{
			WriteSaveData(); // データを書き出し
			m_nowScene = std::make_unique<TitleScene>();
			m_nowScene->ExitApp();
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

	is_saveOnce = false;
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

	switch (m_nextScene)
	{
	case SCENE::PLAY:
		// 再読み込みでなければ処理する
		if (m_nextScene != m_prevScene && m_prevScene != SCENE::RESULT)
		{
			// ステージ番号を保持
			m_num = CastSceneType<SelectScene>(m_nowScene)->GetStageNum();
		}
		break;
	case SCENE::RESULT:
		if (CastSceneType<ResultScene>(m_nowScene) == nullptr)
		{
			// クリアタイムと獲得コイン数を保持
			m_clearTime = CastSceneType<PlayScene>(m_nowScene)->GetClearTime();
			m_coinNum = CastSceneType<PlayScene>(m_nowScene)->GetCoinNum();
			OpenNewStage();
		}
		break;
	default:
		break;
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

/// <summary>
/// セーブデータを読み込む
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::LoadSaveData()
{
	//-------------------------------------------------------------------------------------//
	// 未開放ステージ数と獲得コイン数の取得
	std::ifstream coinIfs(L"Resources/SaveData.txt");

	std::string line;

	// データがなくなるまで格納
	for (; std::getline(coinIfs, line); )
	{
		if (!coinIfs) return;

		// カンマを空白に変更
		std::string tmp = std::regex_replace(line, std::regex(","), " ");

		// 空白で分割する
		std::istringstream iss(tmp);

		if (!(iss >> m_closeNum))
		{
			coinIfs.close();
			return;
		}

		if (!(iss >> m_totalCoinNum))
		{
			coinIfs.close();
			return;
		}
	}
	coinIfs.close();

	//-------------------------------------------------------------------------------------//
	// ステージクリア情報の取得
	std::ifstream clearIfs(L"Resources/Restriction.txt");

	// データがなくなるまで格納
	for (int i = 0; std::getline(clearIfs, line); ++i)
	{
		if (!clearIfs) return;

		// カンマを空白に変更
		std::string tmp = std::regex_replace(line, std::regex(","), " ");

		// 空白で分割する
		std::istringstream clearIss(tmp);

		// 1がクリア,0が未クリア
		if (!(clearIss >> m_clearJudge[i]))
		{
			clearIfs.close();
			return;
		}
	}
	clearIfs.close();
}

/// <summary>
/// セーブデータを書き出す
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::WriteSaveData()
{
	//-------------------------------------------------------------------------------------//
	// 未開放ステージ数と獲得コイン数の書き出し
	std::ofstream coinOfs(L"Resources/SaveData.txt");

	// ファイルがなければ処理しない
	if (coinOfs)
	{
		// ファイルを出力する
		std::ostringstream coinOss;

		coinOss << m_closeNum << "," << m_totalCoinNum << ",";
		coinOfs << coinOss.str();

		coinOfs.close();
	}

	//-------------------------------------------------------------------------------------//
	// クリア状況を書き出し
	std::ofstream clearOfs(L"Resources/Restriction.txt");

	// ファイルがなければ処理しない
	if (clearOfs)
	{
		// ファイルを出力する
		std::ostringstream clearOss;

		for (int i = 0; i < 10; ++i)
		{
			clearOss << m_clearJudge[i] << ",";
		}
		clearOfs << clearOss.str();

		clearOfs.close();
	}
}

/// <summary>
/// ステージ解放処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::OpenNewStage()
{
	if (m_nextScene != SCENE::RESULT || is_saveOnce) return;

	// プレイ中のステージのコイン数をすべて獲得したら開く
	if (CastSceneType<PlayScene>(m_nowScene)->GetCoinNum() == CastSceneType<PlayScene>(m_nowScene)->GetMaxCoinCount() &&
		m_clearJudge[CastSceneType<PlayScene>(m_nowScene)->GetStageNum()] == 0)
	{
		// 全て解放していたら処理しない
		m_closeNum -= m_closeNum > 0 ? 1 : 0;

		m_clearJudge[CastSceneType<PlayScene>(m_nowScene)->GetStageNum()] = 1;
	}

	m_totalCoinNum += m_coinNum;


	// 最新状態を保存
	WriteSaveData();

	// 二回保存されないようにする
	is_saveOnce = true;
}
