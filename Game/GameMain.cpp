/*
 *	@File	GameMain.cpp
 *	@Brief	ゲームループを扱うクラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "GameMain.h"

// Jsonファイルの入出力に使用
#include "Libraries/Nlohmann/json.hpp"
//------------------------------------------------------------------------//
// Jsonファイルのダウンロード先						                      //
//																		  //
// nlohmann/jsonを使用													  //
// https://github.com/nlohmann/json										  //
//------------------------------------------------------------------------//

// ファイル入出力に使用
#include <fstream>

// フェードオブジェクト
#include "Libraries/SystemDatas/Fade.h"

// ユーザーユーティリティ
#include "Libraries/UserUtility.h"

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
GameMain::GameMain()
	: m_nextScene{ SCENE::TITLE }		// 最初のシーン
	, m_prevScene{ SCENE::NONE }		// 前回のシーン
	, m_nowScene{ nullptr }				// 今のシーンポインタ
	, m_num{1}							// ステージ番号
	, m_clearTime{0.0f}					// クリアタイム
	, m_coinNum{0}						// 獲得コイン数
	, m_screenWidth{}					// スクリーンサイズ(横)
	, m_screenHeight{}					// スクリーンサイズ(縦)
	//以下はセーブデータ対応------------------------------------------------------------------------//
	, is_saveOnce{false}				// セーブ済みか確認するフラグ
	, m_safeStages{}						// 未開放ステージ
	, m_allCoins{}						// 累計コイン枚数
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

	// セーブデータを読み込む
	LoadSaveData();
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::Update()
{
	// フェードの更新
	m_fade->Update();

	// キー入力情報を取得する
	m_keyboardStateTracker->Update(Keyboard::Get().GetState());

	// マウス入力情報を取得する
	m_mouseStateTracker->Update(Mouse::Get().GetState());

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
		m_nowScene->Update();

		// フェードの値をセット
		m_nowScene->SetFadeValue(m_fade->GetFadeNum());

		// フェードが終わっていたらシーンを切り替える
		if (m_fade->GetEndFlag())
		{
			m_nextScene = m_nowScene->GetNextScene();
		}
		else // フェードが終わるまで引き留める
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
	m_nowScene.reset();
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

			//-------------------------------------------------------------------------------------//
			// 合計コイン数を999で上限止め
			m_allCoins = UserUtility::Clamp(m_allCoins, 0, 999);

			// ステージ番号、未開放ステージ番号、合計コイン数を渡す
			CastSceneType<SelectScene>(m_nowScene)->SetStageNum(m_num);
			CastSceneType<SelectScene>(m_nowScene)->SetSafeStages(m_safeStages);
			CastSceneType<SelectScene>(m_nowScene)->SetAllCoins(m_allCoins);
			//-------------------------------------------------------------------------------------//

			m_fade->SetFadeSpeed(DEFAULT_FADE_SPEED);
			break;
		}
		case SCENE::PLAY:		// ゲームシーン
		{
			m_nowScene = std::make_unique<PlayScene>();

			//-------------------------------------------------------------------------------------//
			// 選択したステージを渡す
			CastSceneType<PlayScene>(m_nowScene)->SetAllCoinNum(m_allCoins);
			CastSceneType<PlayScene>(m_nowScene)->SetStageNum(m_num);
			//-------------------------------------------------------------------------------------//

			m_fade->SetFadeSpeed(PLAY_FADE_SPEED);
			break;
		}
		case SCENE::RESULT:		// リザルトシーン
		{
			m_nowScene = std::make_unique<ResultScene>();

			//-------------------------------------------------------------------------------------//
			// ステージ番号、クリアタイム、獲得コイン数を渡す
			CastSceneType<ResultScene>(m_nowScene)->SetStageNum(m_num);
			CastSceneType<ResultScene>(m_nowScene)->SetClearTime(m_clearTime);
			CastSceneType<ResultScene>(m_nowScene)->SetCoinNum(m_coinNum);
			//-------------------------------------------------------------------------------------//

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
	m_nowScene->SetScreenSize({ static_cast<float>(m_screenWidth) , static_cast<float>(m_screenHeight) });
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
	case SCENE::EDIT:
		// 制作費用を引いたコインを保持
		m_allCoins = CastSceneType<SelectScene>(m_nowScene)->GetAllCoins();
		break;
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
	DX::DeviceResources* _pDR = DX::DeviceResources::GetInstance();

	// スクリーンサイズの設定
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// キーボード関連
	m_keyboard = std::make_unique<Keyboard>();
	m_keyboardStateTracker = std::make_unique<Keyboard::KeyboardStateTracker>();

	// マウス関連
	m_mouse = std::make_unique<Mouse>();
	m_mouseStateTracker = std::make_unique<Mouse::ButtonStateTracker>();
	m_mouse->SetWindow(_pDR->GetHwnd());

	// フェードオブジェクトの初期化
	m_fade = std::make_unique<Fade>(DEFAULT_FADE_SPEED);
	m_fade->Initialize();
}

/// <summary>
/// セーブデータを読み込む
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::LoadSaveData()
{
	using Json = nlohmann::json;

	// JSONデータをファイルに読み込む
	std::ifstream _file("Datas/Progress.json", std::ios::in);

	if (_file.is_open())
	{
		Json _input = {};
		_file >> _input;

		// データを格納する
		m_allCoins   = _input["CoinNum"];	// 所有コイン数
		m_safeStages = _input["SafeStage"];	//未開放ステージ数

		_file.close();
	}
}

/// <summary>
/// セーブデータを書き出す
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void GameMain::WriteSaveData()
{
	using Json = nlohmann::json;
	const int _JSON_INDENT = 4;	// インデント

	// JSONデータをファイルに書き込む
	std::ofstream _file("Datas/Progress.json", std::ios::out);

	if (_file.is_open())
	{
		Json _output = {};

		// データをセット
		_output =
		{
			 {"CoinNum", m_allCoins},		// 所有コイン数
			 {"SafeStage",m_safeStages}		//未開放ステージ数
		};

		// インデントを揃えて書き出し
		_file << _output.dump(_JSON_INDENT);

		_file.close();
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
	if (CastSceneType<PlayScene>(m_nowScene)->GetCoinNum() == CastSceneType<PlayScene>(m_nowScene)->GetMaxCoinCount())
	{
		// 全て解放していたら処理しない
		m_safeStages -= m_safeStages > 0 ? 1 : 0;
	}

	// コイン数を加算
	m_allCoins += m_coinNum;

	// 最新状態を保存
	WriteSaveData();

	// 二回保存されないようにする
	is_saveOnce = true;
}