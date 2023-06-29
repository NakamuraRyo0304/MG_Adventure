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

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
GameMain::GameMain():
	m_nextScene{ SCENE::TITLE },
	m_nowScene{ nullptr },
	m_num{1},
	m_screenWidth{},
	m_screenHeight{},
	is_fadeOutFlag{}
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

	// フェードイン
	m_fade->SetFadeIn();
	is_fadeOutFlag = false;

	// ステージ番号
	m_num = 1;
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
void GameMain::Update(const DX::StepTimer& timer)
{
	// フェードの更新
	m_fade->Update(FADE_SPEED);

	// 時間を更新する
	float time = static_cast<float>(timer.GetTotalSeconds());

	// キー入力情報を取得する
	auto keyState = Keyboard::Get().GetState();

	m_keyboardStateTracker->Update(keyState);

	// マウス入力情報を取得する
	auto mouseState = Mouse::Get().GetState();

	m_mouseStateTracker->Update(mouseState);

	// エスケープでゲーム終了
	if (keyState.Escape)
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
		m_nowScene->Update(time, keyState, mouseState);

		// NONE以外が入ったら処理
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

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
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
	if (m_nowScene != nullptr)
	{
		return;
	}	
	
	// TODO: シーン３：シーンはここから追加
	switch (m_nextScene)
	{
		case SCENE::TITLE:		// タイトルシーン
		{
			m_nowScene = std::make_unique<TitleScene>();
			break;
		}
		case SCENE::SELECT:		// ステージセレクトシーン
		{
			m_nowScene = std::make_unique<SelectScene>();
			break;
		}
		case SCENE::PLAY:		// ゲームシーン
		{
			m_nowScene = std::make_unique<PlayScene>();
			m_nowScene->SetStageNum(m_num);
			break;
		}
		case SCENE::RESULT:		// リザルトシーン
		{
			m_nowScene = std::make_unique<ResultScene>();
			break;
		}
		case SCENE::EDIT:		// ステージエディットシーン
		{
			m_nowScene = std::make_unique<EditScene>();
			break;
		}
		default:
			break;	
	}

	// 作成したシーンを初期化
	m_nowScene->Initialize();

	// フェードイン
	m_fade->Reset();
	m_fade->SetFadeIn();

	// 次へのシーン情報を初期化
	m_nextScene = SCENE::NONE;
}

//--------------------------------------------------------//
//シーン削除                                              //
//--------------------------------------------------------//
void GameMain::DeleteScene()
{
	// シーンが作成されていなければ処理しない
	if (!m_nowScene) return;

	// 現在がセレクトシーンなら保持
	if (m_nextScene == SCENE::PLAY)
	{
		m_num = m_nowScene->GetStageNum();
	}

	// 現シーンの終了処理
	if (m_fade->GetFadeNum() >= m_fade->GetMaxValue())
	{
		// 現シーンの完全削除
		m_nowScene->Finalize();
		m_nowScene.reset();
	}
	else // フェードアウト
	{		
		m_fade->Reset();
		m_fade->SetFadeOut();
	}
}

//--------------------------------------------------------//
//画面サイズ依存の初期化                                  //
//--------------------------------------------------------//
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
	m_fade = std::make_unique<Fade>();
	m_fade->Initialize(context, device);
}