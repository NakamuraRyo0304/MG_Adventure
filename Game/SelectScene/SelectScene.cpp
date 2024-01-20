/*
 *	@File	SelectScene.cpp
 *	@Brief  セレクトシーン。
 *	@Date	2023-05-03
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/MapLoad.h"
#include "Objects/SelectUI.h"
#include "Objects/FontObject.h"
#include "Objects/SelectSky.h"
#include "System/SelectCamera.h"
#include "../CommonObjects/Blocks.h"
#include "SelectScene.h"

// コンストラクタ
SelectScene::SelectScene(const int& stageNum, const int& safeNum, const int& coins)
	: IScene()					// 基底クラスの初期化
	, m_stageNum{ stageNum }	// ステージ番号
	, m_safeNum{ safeNum }		// 未開放ステージ番号
	, m_allCoins{ coins }		// 合計コイン数
	, m_useCoins{}				// 使用コイン数
	, m_initCoins{}				// 開始コイン数
	, m_targetY{}				// カメラのターゲットのY座標
	, is_selectEdit{false}		// ステージエディットを選択
{
}

// デストラクタ
SelectScene::~SelectScene()
{
	m_blocks->reset();
	m_selectUI.reset();
	m_selectSky.reset();
	m_fontObject.reset();
	m_camera.reset();
}

// 初期化処理
void SelectScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// 変数の初期化
	SetSceneValues();
}

// 更新処理
void SelectScene::Update()
{
	// インプットの更新
	auto& _input = Input::GetInstance();
	auto _key = Keyboard::Get().GetState();

	// セレクトカメラの更新
	m_camera->Update();

	// サウンドの更新
	GetSystemManager()->GetSoundManager()->Update();

	// ステージの変更
	ChangeStageNumber();

	// UIの更新
	m_selectUI->Update((_key.D || _key.Right), (_key.A || _key.Left));

	// エスケープでタイトルに戻る
	if(_input.GetKeyTrack()->IsKeyReleased(Keyboard::Escape)) { ChangeScene(SCENE::TITLE); }

	// コインの数を保存
	m_allCoins = m_initCoins - static_cast<int>(m_useCoins);

	// Spaceキーでシーン切り替え
	if (_input.GetKeyTrack()->IsKeyReleased(Keyboard::Space))
	{
		// フェード中は処理しない
		if (GetFadeValue() >= FADE_START_VALUE) return;

		// シーン切り替え
		GoNextScene();
	}

	// コイン演出
	if (is_selectEdit)
	{
		// コイン数をセットする
		m_selectUI->SetAllCoins(m_initCoins - static_cast<int>(m_useCoins));
		m_selectUI->MoveCoins(m_useCoins);

		// 使用料を支払ったら遷移する
		if (static_cast<int>(m_useCoins) == STAGE_CREATE_PRICE) { ChangeScene(SCENE::EDIT);	}
		else { m_useCoins += COUNT_SPEED; }
	}
}

// 描画処理
void SelectScene::Draw()
{
	// 描画関連
	auto& _states = GetSystemManager()->GetCommonStates();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// カメラ用行列
	SimpleMath::Matrix _view, _proj;

	// ビュー行列
	_view = m_camera->GetView();

	// プロジェクション行列
	_proj = m_camera->GetProjection();

	// マップの描画
	if (m_blocks[m_stageNum] != nullptr)
	{
		m_blocks[m_stageNum]->Render(*_states, _view, _proj, _timer, LIGHT_DIRECTION);
	}

	// スカイドームの描画
	m_selectSky->Draw(*_states, _view, _proj, _timer);

	// 文字の描画
	m_fontObject->Render(*_states, m_stageNum, _timer * FLASH_COUNT, _view, _proj);

	// UIの描画
	m_selectUI->Render(GetFadeValue(), m_stageNum, MAX_STAGE_NUM - 1);
}

// 終了処理
void SelectScene::Finalize()
{
	// モデル削除
	for (size_t i = 0; i < MAX_STAGE_NUM; ++i)
	{
		m_blocks[i]->Finalize();
	}
	m_selectUI->Finalize();
}

// 画面、デバイス依存の初期化
void SelectScene::CreateWindowDependentResources()
{
	// システムの作成
	GetSystemManager()->CreateSystem();
	GetFactoryManager()->CreateFactory();

	// カメラの設定
	m_camera = std::make_unique<SelectCamera>(GetScreenSize());

	// UIの作成
	GetSystemManager()->GetDrawSprite()->MakeSpriteBatch();
	m_selectUI = std::make_unique<SelectUI>();
	m_selectUI->Create(GetSystemManager(),GetScreenSize());

	// フォントオブジェクトの作成
	m_fontObject = std::make_unique<FontObject>(GetFactoryManager(),m_safeNum, MAX_STAGE_NUM);

	// スカイドームモデルを作成する
	m_selectSky = std::make_unique<SelectSky>(GetFactoryManager(), L"Resources/Models/ShineSky.cmo");

	// 先に描画対象を作成し、他を裏で処理
	CreateFirstStage();
	m_loadTask = std::async(std::launch::async, [&]() { CreateStages(); });
}

// シーン変数初期化関数
void SelectScene::SetSceneValues()
{
	// BGMを鳴らす
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_BGM_TITLESELECT, true);

	// コイン数をセット
	m_selectUI->SetAllCoins(m_allCoins);

	// 使用コイン数をリセット(floatなのはコインが減る演出用)
	m_useCoins = 0.0f;

	// 開始コイン数を保存
	m_initCoins = m_allCoins;

	// ステージエディットフラグを初期化
	is_selectEdit = false;
}

// シーン切り替え
void SelectScene::GoNextScene()
{
	// ステージ番号が0ならエディタに、それ以外はプレイへ
	if (m_stageNum == 0)
	{
		is_selectEdit = m_allCoins >= STAGE_CREATE_PRICE;

		// 不足の場合、メッセージ
		if (m_allCoins < STAGE_CREATE_PRICE)
		{
			MessageBox(NULL,
				TEXT("コインを10枚以上取得していないため、この機能は使用できません。"),
				TEXT("コイン不足"), MB_OK);
		}
	}
	else
	{
		ChangeScene(SCENE::PLAY);
	}

	// 決定音を鳴らす
	GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_DECISION, false);
}

// ステージのロード
void SelectScene::CreateStages()
{
	// ファクトリマネージャ
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	for (int i = 0; i < MAX_STAGE_NUM; ++i)
	{
		// ファクトリーからモデルをもらう
		auto _grass   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineBox.cmo");
		auto _coin    = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineCoin.cmo");
		auto _cloud   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineCl.cmo");
		auto _gravity = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

		// 作成されていない場合は作成する
		if (not m_blocks[i])
		{
			// ブロックの作成
			m_blocks[i] = std::make_unique<Blocks>(GetFactoryManager());
			m_blocks[i]->CreateShader();

			// モデルの受け渡し
			m_blocks[i]->CreateModels(std::move(_grass),   m_blocks[i]->GRASS);
			m_blocks[i]->CreateModels(std::move(_coin),    m_blocks[i]->COIN);
			m_blocks[i]->CreateModels(std::move(_cloud),   m_blocks[i]->CLOWD);
			m_blocks[i]->CreateModels(std::move(_gravity), m_blocks[i]->GRAVITY);

			// 初期化処理
			m_blocks[i]->Initialize(i);
		}
	}

	_fm->LeaveModelFactory();
}
void SelectScene::CreateFirstStage()
{
	// ブロックの作成
	m_blocks[m_stageNum] = std::make_unique<Blocks>(GetFactoryManager());
	m_blocks[m_stageNum]->CreateShader();

	// ファクトリマネージャ
	auto& _fm = GetFactoryManager();
	_fm->BuildModelFactory();

	auto _grass   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineBox.cmo");
	auto _coin    = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineCoin.cmo");
	auto _cloud   = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/lineCl.cmo");
	auto _gravity = _fm->VisitModelFactory()->GetCreateModel(L"Resources/Models/ResetPt.cmo");

	_fm->LeaveModelFactory();

	// モデルの受け渡し
	m_blocks[m_stageNum]->CreateModels(std::move(_grass),   m_blocks[m_stageNum]->GRASS);
	m_blocks[m_stageNum]->CreateModels(std::move(_coin),    m_blocks[m_stageNum]->COIN);
	m_blocks[m_stageNum]->CreateModels(std::move(_cloud),   m_blocks[m_stageNum]->CLOWD);
	m_blocks[m_stageNum]->CreateModels(std::move(_gravity), m_blocks[m_stageNum]->GRAVITY);

	// 初期化処理
	m_blocks[m_stageNum]->Initialize(m_stageNum);
}

// ステージ選択
void SelectScene::ChangeStageNumber()
{
	// 切り替え可能なタイミングはここで変更
	if (m_camera->IsCanChange()) return;

	// インプットの更新
	auto _input = Keyboard::Get().GetState();

	if (_input.Right || _input.D)
	{
		// ステージ番号が最大なら処理しない
		if (m_stageNum == MAX_STAGE_NUM - 1 - m_safeNum) return;

		// ターゲットの更新
		m_camera->MoveTarget();

		// 選択音を鳴らす
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);

		m_stageNum++;
	}
	if (_input.Left || _input.A)
	{
		// ステージ番号が0なら処理しない
		if (m_stageNum == 0) return;

		// ターゲットの更新
		m_camera->MoveTarget();

		// 選択音を鳴らす
		GetSystemManager()->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_SELECT, false);

		m_stageNum--;
	}
}