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
	m_clearTime{0.0f},			// クリアタイムを格納
	m_saveTime{0.0f},			// クリアタイムを保存する変数
	m_directionTime{0.0f},		// 演出する時間
	m_selectNum{RETRY},			// 次のシーン選択
	m_coinNum{0},				// コインの枚数
	m_stageNum{1},				// 背景のステージ番号(初期化で1)
	m_retryPos{},				// リトライテキストの位置
	m_retryAlpha{},				// リトライテキストの透明度
	m_retryScale{},				// リトライテキストの大きさ
	m_selectPos{},				// セレクトテキストの位置
	m_selectAlpha{},			// セレクトテキストの透明度
	m_selectScale{},			// セレクトテキストの大きさ
	m_titlePos{},				// タイトルテキストの位置
	m_titleAlpha{},				// タイトルテキストの透明度
	m_titleScale{},				// タイトルテキストの大きさ
	m_coinsPos{},				// コインテキストの位置
	m_oneCoiPos{},				// コインの1の位の座標
	m_tenCoiPos{},				// コインの10の位の座標
	m_windowSize{},				// ウィンドウサイズ
	m_clockPos{}				// タイマーの座標
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

	// アルファ値とスケールの変更
	switch (m_selectNum)
	{
	case RETRY:
		m_retryAlpha  = 1.0f;
		m_selectAlpha = 0.5f;
		m_titleAlpha  = 0.5f;
		m_retryScale  = 1.1f;
		m_selectScale = 1.0f;
		m_titleScale  = 1.0f;
		break;
	case SELECT:
		m_retryAlpha  = 0.5f;
		m_selectAlpha = 1.0f;
		m_titleAlpha  = 0.5f;
		m_retryScale  = 1.0f;
		m_selectScale = 1.1f;
		m_titleScale  = 1.0f;
		break;
	case TITLE:
		m_retryAlpha  = 0.5f;
		m_selectAlpha = 0.5f;
		m_titleAlpha  = 1.0f;
		m_retryScale  = 1.0f;
		m_selectScale = 1.0f;
		m_titleScale  = 1.1f;
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
	float imageScale = static_cast<float>(m_windowSize.x) / FULL_SCREEN_SIZE.x;

	// 画面を薄暗くする
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"BLIND",										// 登録キー
			SimpleMath::Vector2::Zero,						// 座標
			{ 1.0f,1.0f,1.0f,0.5f },						// 色
			1.0f,											// 拡大率
			SimpleMath::Vector2::Zero						// 中心位置
		);

	// コイン文字
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"COINS",										// 登録キー
			m_coinsPos,										// 座標
			{ 1.0f,1.0f,1.0f,1.0f },						// 色
			IMAGE_RATE * imageScale * 1.0f,					// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }					// 中心位置
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


	// 時計画像
	GetSystemManager()->GetDrawSprite()->DrawTexture(
			L"Clock",										// 登録キー
			m_clockPos,										// 座標
			{ 1.0f,1.0f,1.0f,1.0f },						// 色
			DEFAULT_RATE * imageScale,						// 拡大率
			{ IMAGE_CENTER,IMAGE_CENTER }					// 中心位置
		);


	//-------------------------------------------------------------------------------------// 
	// 秒数を計算
	int sec = static_cast<int>(m_clearTime);

	// 一桁目の数字を表示
	RenderDigit(sec % 10, m_oneSecPos, imageScale, SPRITE_SIZE, SPRITE_SIZE);

	// 十の桁の数字を表示
	RenderDigit((sec / 10) % 10, m_tenSecPos, imageScale, SPRITE_SIZE, SPRITE_SIZE);

	//-------------------------------------------------------------------------------------// 

	// 一桁目の数字を表示
	RenderDigit(m_coinNum % 10, m_oneCoiPos, imageScale, SPRITE_SIZE, SPRITE_SIZE);

	// 十の桁の数字を表示
	RenderDigit((m_coinNum / 10) % 10, m_tenCoiPos, imageScale, SPRITE_SIZE, SPRITE_SIZE);
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
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"COINS", L"Resources/Textures/FONT/COINS.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"BLIND", L"Resources/Textures/ResultBack.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Number", L"Resources/Textures/Number.dds", device);
	GetSystemManager()->GetDrawSprite()->AddTextureData(L"Clock", L"Resources/Textures/Clock.dds", device);

	// 比率を計算
	float span = width / FULL_SCREEN_SIZE.x;

	// スプライトの位置を計算
	m_oneSecPos = { (NUM_OFFSET + static_cast<float>(SPRITE_SIZE / 2)) * span, 100.0f * span };
	m_tenSecPos = { (NUM_OFFSET - static_cast<float>(SPRITE_SIZE / 2)) * span, 100.0f * span };
	m_oneCoiPos = { (NUM_OFFSET + static_cast<float>(SPRITE_SIZE / 2)) * span, 440.0f * span };
	m_tenCoiPos = { (NUM_OFFSET - static_cast<float>(SPRITE_SIZE / 2)) * span, 440.0f * span };

	// 座標情報
	m_coinsPos  = { TEXT_OFFSET * span, 400.0f * span };
	m_retryPos  = { TEXT_OFFSET * span, 700.0f * span };
	m_selectPos = { TEXT_OFFSET * span, 800.0f * span };
	m_titlePos  = { TEXT_OFFSET * span, 900.0f * span };
	m_clockPos  = { (SPRITE_SIZE + width * span) / 2,150.0f * span };

	// 座標補正
	CorrectionOffset(width, span);
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
void ResultScene::RenderDigit(int digit, const DirectX::SimpleMath::Vector2& position, float scale, int digitWidth, int digitHeight)
{
	// スプライトの位置を計算
	float spritePosX = position.x * scale;
	float spritePosY = position.y * scale;

	// スプライトの中心位置を計算
	SimpleMath::Vector2 center = { spritePosX  * scale / 2.0f, spritePosY  * scale / 2.0f };

	// 切り取り位置の設定
	RECT_U rect;

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
/// オフセット値を変更する関数
/// </summary>
/// <param name="width">画面横幅</param>
/// <param name="span">最大サイズとの比率</param>
/// <returns>なし</returns>
void ResultScene::CorrectionOffset(float width ,float span)
{
	// 座標補正
	if (static_cast<int>(width) != 1280) return;

	m_oneSecPos.x -= static_cast<float>(SPRITE_SIZE / 1.5f * span);
	m_tenSecPos.x -= static_cast<float>(SPRITE_SIZE / 1.5f * span);
	m_oneCoiPos.x -= static_cast<float>(SPRITE_SIZE / 1.5f * span);
	m_tenCoiPos.x -= static_cast<float>(SPRITE_SIZE / 1.5f * span);
	m_clockPos.x  += static_cast<float>(SPRITE_SIZE * 2.95f * span);
}
