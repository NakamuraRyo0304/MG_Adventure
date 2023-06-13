/*
 *	@File	GameMain.cpp
 *	@Brief	ゲームループを扱うクラス
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "GameMain.h"

// TODO: シーン２：シーンのインクルード
#include "Game/TitleScene/TitleScene.h"
#include "Game/SelectScene/SelectScene.h"
#include "Game/EditScene/EditScene.h"
#include "Game/PlayScene/PlayScene.h"
#include "Game/ResultScene/ResultScene.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
GameMain::GameMain():
	m_nextScene(SCENE::TITLE),
	m_pNowScene(nullptr),
	m_num(1),
	m_screenWidth(),
	m_screenHeight()
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
GameMain::~GameMain()
{
	Finalize();
}

//--------------------------------------------------------//
//初期化                                                  //
//--------------------------------------------------------//
void GameMain::Initialize()
{
	// シーン作成
	CreateScene();

	// ステージ番号
	m_num = 1;
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
void GameMain::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	auto keyState = DirectX::Keyboard::Get().GetState();
	
	m_keyboardStateTracker->Update(keyState);

	// マウス入力情報を取得する
	auto mouseState = DirectX::Mouse::Get().GetState();

	m_mouseStateTracker->Update(mouseState);

	// 次のシーンが設定されていたらシーン切り替え
	if (m_nextScene != SCENE::NONE)
	{
		// シーン削除
		DeleteScene();
		
		// シーン作成
		CreateScene();
	}

	// 実態があれば更新
	if (m_pNowScene != nullptr)
	{
		// タイマーとキーボードの受け渡し
		m_pNowScene->Update(static_cast<float>(timer.GetElapsedSeconds()), keyState,mouseState);
				
		// NONE以外が入ったら処理
		m_nextScene = m_pNowScene->GetNextScene();
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void GameMain::Draw()
{
	// 実態があれば描画
	if (m_pNowScene != nullptr)
	{
		m_pNowScene->Draw();
	}
}

//--------------------------------------------------------//
//終了処理                                                //
//--------------------------------------------------------//
void GameMain::Finalize()
{
	// シーン削除
	DeleteScene();
}

//--------------------------------------------------------//
//シーン作成                                              //
//--------------------------------------------------------//
void GameMain::CreateScene()
{
	// シーンが作成されているときは処理しない
	if (m_pNowScene != nullptr)
	{
		return;
	}	

	// TODO: シーン３：シーンはここから追加
	switch (m_nextScene)
	{
		case SCENE::TITLE:		// タイトルシーン
		{
			m_pNowScene = std::make_unique<TitleScene>();
			break;
		}
		case SCENE::SELECT:		// ステージセレクトシーン
		{
			m_pNowScene = std::make_unique<SelectScene>();
			break;
		}
		case SCENE::PLAY:		// ゲームシーン
		{
			m_pNowScene = std::make_unique<PlayScene>();
			break;
		}
		case SCENE::RESULT:		// リザルトシーン
		{
			m_pNowScene = std::make_unique<ResultScene>();
			break;
		}
		case SCENE::EDIT:		// ステージエディットシーン
		{
			m_pNowScene = std::make_unique<EditScene>();
			break;
		}
		default:
			break;
	}

	// プレイシーンになったら受け渡し
	if (m_nextScene == SCENE::PLAY)
	{
		m_pNowScene->SetStageNum(m_num);
	}

	// 作成したシーンを初期化
	m_pNowScene->Initialize();

	// 次へのシーン情報を初期化
	m_nextScene = SCENE::NONE;
}

//--------------------------------------------------------//
//シーン削除                                              //
//--------------------------------------------------------//
void GameMain::DeleteScene()
{
	// シーンが作成されていなければ処理しない
	if (m_pNowScene == nullptr)
	{
		return;
	}

	// 現在がセレクトシーンなら保持
	if (m_nextScene == SCENE::PLAY)
	{
		m_num = m_pNowScene->GetStageNum();
	}

	// 現シーンの終了処理
	m_pNowScene->Finalize();

	// 現シーンの完全削除
	m_pNowScene.reset();
}

//--------------------------------------------------------//
//画面サイズ依存の初期化                                  //
//--------------------------------------------------------//
void GameMain::CreateWindowDependentResources(const int& screenWidth, const int& screenHeight)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// スクリーンサイズの設定
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// キーボード関連
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_keyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	// マウス関連
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouseStateTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
	m_mouse->SetWindow(pDR->GetHwnd());
}
