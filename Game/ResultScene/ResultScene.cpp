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

// ランダム(演出用なのでランダムデバイスは使わない)
#include <random>
#include <time.h>

#include "ResultScene.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
ResultScene::ResultScene():
	IScene(),
	m_timer{0.0f},				// 時計
	m_windowSize{},				// ウィンドウサイズ
	m_clearTime{0.0f},			// クリアタイムを格納
	m_saveTime{0.0f},			// クリアタイムを保存する変数
	m_directionTime{0.0f},		// 演出する時間
	m_selectingScene{RETRY},	// 次のシーン選択
	m_stageNum{1},				// 背景のステージ番号(初期化で1)
	//-------------------------------------------------------------------------------------//
	m_retryPos{},				// リトライテキストの位置
	m_retryAlpha{},				// 透明度
	m_retryScale{},				// 大きさ
	//-------------------------------------------------------------------------------------//
	m_selectPos{},				// セレクトテキストの位置
	m_selectAlpha{},			// 透明度
	m_selectScale{},			// 大きさ
	//-------------------------------------------------------------------------------------//
	m_titlePos{},				// タイトルテキストの位置
	m_titleAlpha{},				// 透明度
	m_titleScale{},				// 大きさ
	//-------------------------------------------------------------------------------------//
	m_coinsPos{},				// コインテキストの位置
	m_coinNum{0},				// 枚数
	m_oneCoiPos{},				// 1の位の座標
	m_tenCoiPos{},				// 10の位の座標
	//-------------------------------------------------------------------------------------//
	m_clearPos{}				// クリアテキストの位置
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

	// マップ読み込み
	m_blocks->Initialize(m_stageNum);

	// タイマーの保存
	m_saveTime = m_clearTime;

	// 演出時間 最初のフェードも考慮して多めに取る
	m_directionTime = 120.0f;

	// BGMを鳴らす
	GetSystemManager()->GetSoundManager()->SetSound(XACT_WAVEBANK_SKBX_BGM_RESULT, true);
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

	// 演出時間をカウント
	m_directionTime--;

	// 演出をする
	if (m_directionTime < 0.0f)
	{
		m_directionTime = 0.0f;
		m_clearTime = 60 - m_saveTime;
	}
	else
	{
		// ランダムな値を入れる
		m_clearTime = static_cast<float>(rand() % 60 + 1);
	}

	// キー入力情報を取得する
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// サウンドの更新
	GetSystemManager()->GetSoundManager()->Update();

	// メニューセレクト
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Down))
	{
		m_selectingScene++;
		m_selectingScene = m_selectingScene ==  3 ? RETRY : m_selectingScene;
	}
	else if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Up))
	{
		m_selectingScene--;
		m_selectingScene = m_selectingScene == -1 ? TITLE : m_selectingScene;
	}

	// アルファ値とスケールの変更
	switch (m_selectingScene)
	{
	case RETRY:
		// 透明度
		m_retryAlpha  = SELECT_FONT_ALPHA;
		m_selectAlpha = DEFAULT_FONT_ALPHA;
		m_titleAlpha  = DEFAULT_FONT_ALPHA;
		// サイズ
		m_retryScale  = SELECT_FONT_SCALE;
		m_selectScale = DEFAULT_FONT_SCALE;
		m_titleScale  = DEFAULT_FONT_SCALE;
		break;
	case SELECT:
		// 透明度
		m_retryAlpha  = DEFAULT_FONT_ALPHA;
		m_selectAlpha = SELECT_FONT_ALPHA;
		m_titleAlpha  = DEFAULT_FONT_ALPHA;
		// サイズ
		m_retryScale  = DEFAULT_FONT_SCALE;
		m_selectScale = SELECT_FONT_SCALE;
		m_titleScale  = DEFAULT_FONT_SCALE;
		break;
	case TITLE:
		// 透明度
		m_retryAlpha  = DEFAULT_FONT_ALPHA;
		m_selectAlpha = DEFAULT_FONT_ALPHA;
		m_titleAlpha  = SELECT_FONT_ALPHA;
		// サイズ
		m_retryScale  = DEFAULT_FONT_SCALE;
		m_selectScale = DEFAULT_FONT_SCALE;
		m_titleScale  = SELECT_FONT_SCALE;
		break;
	default:
		break;
	}

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
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
	}

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
void ResultScene::Draw()
{
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
	SimpleMath::Vector2 imageScale = m_windowSize / FULL_SCREEN_SIZE;

	// 画面を薄暗くする
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"BLIND",										// 登録キー
			SimpleMath::Vector2::Zero,						// 座標
			{ 1.0f,1.0f,1.0f,0.3f },						// 色
			1.0f * imageScale,								// 拡大率
			SimpleMath::Vector2::Zero						// 中心位置
		);

	// 文字を描画
	DrawTextFonts(imageScale);

	//-------------------------------------------------------------------------------------//
	// 秒数を計算
	int sec = static_cast<int>(m_clearTime);

	// 一桁目の数字を表示
	RenderDigit(sec % 10,		 m_oneSecPos, imageScale, static_cast<int>(SPRITE_SIZE), static_cast<int>(SPRITE_SIZE));

	// 十の桁の数字を表示
	RenderDigit((sec / 10) % 10, m_tenSecPos, imageScale, static_cast<int>(SPRITE_SIZE), static_cast<int>(SPRITE_SIZE));

	//-------------------------------------------------------------------------------------//
	// 獲得コイン数を表示

	// 一桁目の数字を表示
	RenderDigit(m_coinNum % 10,		   m_oneCoiPos, imageScale, static_cast<int>(SPRITE_SIZE), static_cast<int>(SPRITE_SIZE));

	// 十の桁の数字を表示
	RenderDigit((m_coinNum / 10) % 10, m_tenCoiPos, imageScale, static_cast<int>(SPRITE_SIZE), static_cast<int>(SPRITE_SIZE));
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
	float width  = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

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
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"RETRY",     L"Resources/Textures/FONT/RETRY.dds",	  device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"SELECT",    L"Resources/Textures/FONT/SELECT.dds",  device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"TITLE",     L"Resources/Textures/FONT/TITLE.dds",	  device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Clear",     L"Resources/Textures/FONT/CLEAR.dds",	  device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"TotalCoin", L"Resources/Textures/FONT/GETCOINS.dds",device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"BLIND",     L"Resources/Textures/ResultBack.dds",	  device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Number",    L"Resources/Textures/Number.dds",		  device);

	// 比率を計算
	SimpleMath::Vector2 scale = m_windowSize / FULL_SCREEN_SIZE;

	// 共通部分を定義
	float spCenterX   = SPRITE_SIZE / 2 * scale.x;
	float numCenterX  = FULL_SCREEN_SIZE.x / 2 * scale.x - spCenterX;

	// 各座標を決定
	m_oneSecPos = { numCenterX + spCenterX ,200.0f * scale.y };
	m_tenSecPos = { numCenterX - spCenterX ,200.0f * scale.y };
	m_oneCoiPos = { numCenterX + spCenterX ,540.0f * scale.y };
	m_tenCoiPos = { numCenterX - spCenterX ,540.0f * scale.y };

	m_coinsPos  = { 50.0f, m_oneCoiPos.y - 40.0f };
	m_clearPos  = { 50.0f, m_oneSecPos.y - 40.0f };
	m_retryPos  = { FULL_SCREEN_SIZE.x - FONT_WIDTH, 700.0f };
	m_selectPos = { FULL_SCREEN_SIZE.x - FONT_WIDTH, 800.0f };
	m_titlePos  = { FULL_SCREEN_SIZE.x - FONT_WIDTH, 900.0f };
}

/// <summary>
/// 数字を描画する
/// </summary>
/// <param name="digit">描画する数字</param>
/// <param name="position">座標</param>
/// <param name="scale">拡大率</param>
/// <param name="digitWidth">数字の幅</param>
/// <param name="digitHeight">数字の高さ</param>
/// <returns>なし</returns>
void ResultScene::RenderDigit(int digit, const SimpleMath::Vector2& position, SimpleMath::Vector2 scale, int digitWidth, int digitHeight)
{
	// スプライトの位置を計算
	float spritePosX = position.x * scale.x;
	float spritePosY = position.y * scale.y;

	// スプライトの中心位置を計算
	SimpleMath::Vector2 center = { spritePosX  * scale.x / 2.0f, spritePosY  * scale.y / 2.0f };

	// 切り取り位置の設定
	RECT_U rect = RECT_U();

	// 切り取り開始位置を設定(横)
	rect.left = digit * digitWidth;

	// 切り取り終了位置を設定(横)
	rect.right = rect.left + digitWidth;

	// 画像縦幅を設定
	rect.bottom = digitHeight;

	// 数字表示
	GetSystemManager()->GetDrawSprite()->DrawTexture(
		L"Number",                         // 登録キー
		position + center,                 // 座標
		{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
		scale,                             // 拡大率
		center,                            // 中心位置
		rect                               // 切り取り位置
	);
}

/// <summary>
/// フォントを描画する
/// </summary>
/// <param name="imageScale">画像比率</param>
/// <returns>なし</returns>
void ResultScene::DrawTextFonts(SimpleMath::Vector2 imageScale)
{
	// コイン文字
	GetSystemManager()->GetDrawSprite()->DrawTexture(
		L"TotalCoin",
		m_coinsPos * imageScale,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		IMAGE_RATE * imageScale,
		SimpleMath::Vector2::Zero
	);
	// クリアタイム文字
	GetSystemManager()->GetDrawSprite()->DrawTexture(
		L"Clear",
		m_clearPos * imageScale,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		IMAGE_RATE * imageScale,
		SimpleMath::Vector2::Zero
	);

	//-------------------------------------------------------------------------------------//

	// シーン選択文字(リトライ、セレクト、タイトル)
	GetSystemManager()->GetDrawSprite()->DrawTexture(
		L"RETRY",
		m_retryPos * imageScale,
		{ 1.0f, 1.0f, 1.0f, m_retryAlpha },
		IMAGE_RATE * imageScale * m_retryScale,
		SimpleMath::Vector2::Zero
	);
	GetSystemManager()->GetDrawSprite()->DrawTexture(
		L"SELECT",
		m_selectPos * imageScale,
		{ 1.0f, 1.0f, 1.0f, m_selectAlpha },
		IMAGE_RATE * imageScale * m_selectScale,
		SimpleMath::Vector2::Zero
	);
	GetSystemManager()->GetDrawSprite()->DrawTexture(
		L"TITLE",
		m_titlePos * imageScale,
		{ 1.0f, 1.0f, 1.0f, m_titleAlpha },
		IMAGE_RATE * imageScale * m_titleScale,
		SimpleMath::Vector2::Zero
	);
}
