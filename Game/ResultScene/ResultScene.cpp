/*
 *	@File	ResultScene.cpp
 *	@Brief  リザルトシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/MapLoad.h"
#include "Objects/ResultUI.h"
#include "System/ResultCamera.h"
#include "../CommonObjects/Blocks.h"
#include "ResultScene.h"

// コンストラクタ
ResultScene::ResultScene(const int& stageNum, const int& coins, const float& clearTime)
	: IScene()					// 基底クラスの初期化
	, m_stageNum{ stageNum }	// 背景のステージ番号
	, m_coinNum{ coins }		// コインの数
	, m_clearTime{ clearTime }	// クリアタイムを格納
	, m_saveTime{0.0f}			// クリアタイムを保存する変数
	, m_directionalTime{0.0f}	// 演出する時間
	, m_selectingScene{ 0 }		// 現在選択中のシーン
	, is_animEnd{false}			// アニメーションの終了フラグ
{

}

// デストラクタ
ResultScene::~ResultScene()
{
	m_blocks.reset();
	m_resultUI.reset();
	m_camera.reset();
}

// 初期化処理
void ResultScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// 変数の初期化
	SetSceneValues();
}

// 更新処理
void ResultScene::Update()
{
	// インプットの更新
	auto& _input = Input::GetInstance();

	// リザルトカメラの更新
	m_camera->Update();

	// サウンドの更新
	GetSystemManager()->GetSoundManager()->Update();

	// 数字のアニメーション
	is_animEnd = AnimationValue();

	// エスケープで終了
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::ENDGAME); }

	// メニューセレクト
	SelectMenu();

	// UIの更新
	m_resultUI->Update(static_cast<int>(m_clearTime));

	// Spaceキーでシーン切り替え
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Space))
	{
		// フェード中は処理しない
		if (static_cast<int>(GetFadeValue()) != 0) return;

		// シーン分岐
		NextScene();

		// 決定音を鳴らす
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
	}
}

// 描画関数
void ResultScene::Draw()
{
	// 描画関連
	auto& _states = GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// カメラ用行列
	SimpleMath::Matrix  _view, _projection;

	// ビュー行列
	_view = m_camera->GetView();

	// プロジェクション行列
	_projection = m_camera->GetProjection();

	// マップの描画
	m_blocks->Render(*_states, _view, _projection, _timer, LIGHT_DIRECTION);

	// UIの表示
	m_resultUI->Render(is_animEnd);
}

// 終了処理
void ResultScene::Finalize()
{
	// マップの後処理
	m_blocks->Finalize();

	// UIの後処理
	m_resultUI->Finalize();
}

// 画面、デバイス依存の初期化
void ResultScene::CreateWindowDependentResources()
{
	// システムの作成
	GetSystemManager()->CreateSystem();
	GetFactoryManager()->CreateFactory();

	// カメラの設定
	m_camera = std::make_unique<ResultCamera>(GetScreenSize());

	// UIの作成
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_resultUI = std::make_unique<ResultUI>();
	m_resultUI->Create(GetSystemManager(), GetScreenSize());

	// ブロックの作成
	m_blocks = std::make_unique<Blocks>(GetFactoryManager());

	// モデルの作成
	GetFactoryManager()->BuildModelFactory();
	CreateModels(GetFactoryManager());
	GetFactoryManager()->LeaveModelFactory();
}

// シーン変数初期化関数
void ResultScene::SetSceneValues()
{
	// カメラ視点移動
	GetSystemManager()->GetCamera()->SetEagleMode(false);

	// BGMを鳴らす
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_RESULT, true);

	// タイマーの保存
	m_saveTime = m_clearTime;

	// 演出時間 最初のフェードも考慮して多めに取る
	m_directionalTime = 120.0f;

	// マップ読み込み
	m_blocks->Initialize(m_stageNum);

	// アニメーションのフラグ
	is_animEnd = false;

	// コインを設定
	m_resultUI->SetCoins(m_coinNum);
}

// モデルの作成
void ResultScene::CreateModels(std::shared_ptr<FactoryManager> fm)
{
	auto _grass = fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/GrassBlock.cmo");
	auto _coin = fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Coin.cmo");
	auto _cloud = fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/Cloud.cmo");
	auto _gravity = fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

	// モデルの受け渡し
	m_blocks->CreateModels(std::move(_grass), m_blocks->GRASS);
	m_blocks->CreateModels(std::move(_coin), m_blocks->COIN);
	m_blocks->CreateModels(std::move(_cloud), m_blocks->CLOWD);
	m_blocks->CreateModels(std::move(_gravity), m_blocks->GRAVITY);
}

// 数字のアニメーション
bool ResultScene::AnimationValue()
{
	// 演出時間をカウント
	m_directionalTime--;

	// ランダムの値を生成
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, MAX_COUNT);

	// ランダムな整数を生成
	int randomValue = dist(gen);

	// 演出をする
	if (m_directionalTime < 0.0f)
	{
		m_directionalTime = 0.0f;
		m_clearTime = static_cast<float>(MAX_TIME) - m_saveTime;

		return true;
	}
	else
	{
		// ランダムな値を入れる
		m_clearTime = static_cast<float>(randomValue);
	}

	return false;
}

// シーン切り替え
void ResultScene::NextScene()
{
	switch (m_selectingScene)
	{
	case SELECTION::NEXT:	// 最大ステージなら処理をしない
		m_stageNum < 5 ? m_stageNum++ : NULL;
		ChangeScene(SCENE::PLAY);
		break;
	case SELECTION::RETRY:
		ChangeScene(SCENE::PLAY);
		break;
	case SELECTION::SELECT:
		ChangeScene(SCENE::SELECT);
		break;
	default:
		break;
	}
}

// メニュー選択
void ResultScene::SelectMenu()
{
	// インプット取得
	auto& _input = Input::GetInstance();
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Right) ||
		_input.GetKeyTrack()->IsKeyReleased(Keyboard::D))
	{
		// フェード中は処理しない
		if (static_cast<int>(GetFadeValue()) != 0) return;

		m_selectingScene++;
		m_selectingScene = m_selectingScene == 3 ? SELECTION::NEXT : m_selectingScene;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
	}
	else if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Left) ||
		_input.GetKeyTrack()->IsKeyReleased(Keyboard::A))
	{
		// フェード中は処理しない
		if (static_cast<int>(GetFadeValue()) != 0) return;

		m_selectingScene--;
		m_selectingScene = m_selectingScene == -1 ? SELECTION::SELECT : m_selectingScene;
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);
	}

	// シーンを設定
	m_resultUI->SetSelecting(m_selectingScene);
}
