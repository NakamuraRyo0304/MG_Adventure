/*
 *	@File	ResultScene.cpp
 *	@Brief  リザルトシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ユーザーユーティリティ
#include "Libraries/UserUtility.h"

 // CSV読み込み
#include "../../Libraries/SystemDatas/MapLoad.h"

// ブロック
#include "../PlayScene/Objects/Blocks.h"

#include "ResultScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
ResultScene::ResultScene():
	IScene(),
	m_timer{0.0f},
	m_clearTime{0.0f},
	m_selectNum{RETRY},
	m_stageNum{1},
	m_retryPos{},
	m_retryAlpha{},
	m_retryScale{},
	m_selectPos{},
	m_selectAlpha{},
	m_selectScale{},
	m_titlePos{},
	m_titleAlpha{},
	m_titleScale{},
	m_windowSize{}
{
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

	// マップ読み込み
	m_blocks->Initialize(m_stageNum);
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

	// メニューセレクト
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Down))
	{
		m_selectNum++;
		if (m_selectNum == 3) // ループ処理
		{
			m_selectNum = RETRY;
		}
	}
	else if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Up))
	{
		m_selectNum--;
		if (m_selectNum < 0) // ループ処理
		{
			m_selectNum = TITLE;
		}
	}

	// アルファ値の変更
	switch (m_selectNum)
	{
	case RETRY:
		m_retryAlpha = 1.0f;
		m_selectAlpha = 0.5f;
		m_titleAlpha = 0.5f;
		m_retryScale = 1.1f;
		m_selectScale = 1.0f;
		m_titleScale = 1.0f;
		break;
	case SELECT:
		m_retryAlpha = 0.5f;
		m_selectAlpha = 1.0f;
		m_titleAlpha = 0.5f;
		m_retryScale = 1.0f;
		m_selectScale = 1.1f;
		m_titleScale = 1.0f;
		break;
	case TITLE:
		m_retryAlpha = 0.5f;
		m_selectAlpha = 0.5f;
		m_titleAlpha = 1.0f;
		m_retryScale = 1.0f;
		m_selectScale = 1.0f;
		m_titleScale = 1.1f;
		break;
	default:
		break;
	}

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		switch (m_selectNum)
		{
		case RETRY:
			GoNextScene(SCENE::PLAY);
			break;
		case SELECT:
			GoNextScene(SCENE::SELECT);
			break;
		case TITLE:
			GoNextScene(SCENE::TITLE);
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::Draw()
{
	// デバッグフォント
	GetSystemManager()->GetString()->ChangeFontColor(Colors::Black);
	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(), 
		{ 0,0 }, 
		L"ResultScene"
	);

	wchar_t sel[10];

	if (m_selectNum == RETRY)
	{
		swprintf_s(sel, 10, L"Retry");
	}
	else if (m_selectNum == SELECT)
	{
		swprintf_s(sel, 10, L"Select");
	}
	else if (m_selectNum == TITLE)
	{
		swprintf_s(sel, 10, L"Title");
	}

	// 描画関連
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	SimpleMath::Matrix  view, proj;

	// ビュー行列
	SimpleMath::Vector3    eye(cosf(m_timer), 20.0f + sinf(m_timer) * 2.0f, 10.0f);
	SimpleMath::Vector3     up(0.0f, 5.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, -10.0f, -5.0f);
		
	view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

	// プロジェクション行列
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// マップの描画
	m_blocks->Render(context, states, view, proj, m_timer);

	//-------------------------------------------------------------------------------------// 

	// 画像の拡大率をウィンドウをもとに計算
	float imageScale = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// 画面を薄暗くする
	GetSystemManager()->GetDrawSprite()->DrawTexture(
		L"BLIND",											// 登録キー
		SimpleMath::Vector2::Zero,							// 座標
		{ 1.0f,1.0f,1.0f,0.5f },							// 色
		1.0f,												// 拡大率
		SimpleMath::Vector2::Zero							// 中心位置
	);

	// リトライ文字
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"RETRY",										// 登録キー
			m_retryPos,										// 座標
			{ 1.0f,1.0f,1.0f,m_retryAlpha },				// 色
			IMAGE_RATE * imageScale * m_retryScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }					// 中心位置
		);

	// セレクト文字
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"SELECT",										// 登録キー
			m_selectPos,									// 座標
			{ 1.0f,1.0f,1.0f,m_selectAlpha },				// 色
			IMAGE_RATE * imageScale * m_selectScale,		// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }					// 中心位置
		);

	// タイトル文字
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"TITLE",										// 登録キー
			m_titlePos,										// 座標
			{ 1.0f,1.0f,1.0f,m_titleAlpha },				// 色
			IMAGE_RATE * imageScale * m_titleScale,			// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }					// 中心位置
		);
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
}

/// <summary>
/// 画面依存、デバイス依存の初期化
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ResultScene::CreateWindowDependentResources()
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

	// ウィンドウサイズを取得
	m_windowSize = SimpleMath::Vector2{ width,height };

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, 45.0f);

	// ブロックの作成
	m_blocks = std::make_unique<Blocks>();

	// 草ブロックの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/GrassBlock.cmo")),
		m_blocks->GRASS
	);
	// コインの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/Coin.cmo")),
		m_blocks->COIN
	);
	// 雲ブロックの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/MoveBlock.cmo")),
		m_blocks->CLOWD
	);
	// 雲リセットブロックの作成
	m_blocks->CreateModels(
		std::move(ModelFactory::GetCreateModel(device, L"Resources/Models/ResetPt.cmo")),
		m_blocks->RECLOWD
	);

	// 画像の設定
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch(context);
	// 画像を登録
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"RETRY", L"Resources/Textures/FONT/RETRY.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"SELECT", L"Resources/Textures/FONT/SELECT.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"TITLE", L"Resources/Textures/FONT/TITLE.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"BLIND", L"Resources/Textures/ResultBack.dds", device);

	// 比率を計算
	float span = static_cast<float>(width) / FULL_SCREEN_SIZE.x;

	// 座標情報
	m_retryPos  = { 960.0f * span, 500.0f * span };
	m_selectPos = { 960.0f * span, 600.0f * span };
	m_titlePos  = { 960.0f * span, 700.0f * span };
}