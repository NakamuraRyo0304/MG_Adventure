/*
 *	@File	PlayUI.cpp
 *	@Brief	プレイシーンのUI表示。
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemManager/SystemManager.h"
#include "Libraries/UserUtility.h"
#include "PlayUI.h"

// コンストラクタ
PlayUI::PlayUI()
	: m_system{}								// システムマネージャ
	, m_windowSize{}							// ウィンドウサイズ
	, m_gameTimer{0}							// ゲームのタイマー
    , m_oneSecPos{SimpleMath::Vector2::Zero}	// 1秒の座標
    , m_tenSecPos{SimpleMath::Vector2::Zero}	// 10秒の座標
	, m_countDownPos{SimpleMath::Vector2::Zero}	// ３カウントダウンの座標
	, m_countDownEnds{1.0f}						// カウントダウンが消える時間
	, m_underFontPos{SimpleMath::Vector2::Zero}	// 下の文字ロールの座標
	, m_moveTexPos{SimpleMath::Vector2::Zero}	// 画像移動用座標
	, m_arrowPos{SimpleMath::Vector2::Zero}		// 選択項目下線座標
	, m_helpPoses{}								// ヘルプの座標
	, m_targetArrow{}							// 選択に応じた座標
	, m_pageNum{1}								// ヘルプのページ番号
	, m_transNum{0}								// 遷移番号
	, m_sunPos{SimpleMath::Vector2::Zero}		// 太陽の画像の座標
	, is_transFlag{ false }						// 遷移画面フラグ
	, is_effectFlag{false}						// エフェクトの表示フラグ
	, is_helpFlag{false}						// ヘルプ画面のフラグ
{

}

// デストラクタ
PlayUI::~PlayUI()
{
	Finalize();
	m_system.reset();
}

// 作成関数
void PlayUI::Create(const std::shared_ptr<SystemManager>& system ,const SimpleMath::Vector2& windowSize)
{
	m_system = system;
	m_windowSize = windowSize;

	// 画像を登録
	m_system->GetDrawSprite()->AddTextureData(L"Number",			// 数字スプライト
		L"Resources/Textures/Number.dds");

	m_system->GetDrawSprite()->AddTextureData(L"GameStart",			// スタート合図
		L"Resources/Textures/PLAY_COMMON/GameStart.dds");

	m_system->GetDrawSprite()->AddTextureData(L"Death",				// 死亡エフェクト
		L"Resources/Textures/PLAY_COMMON/DeathEffect.dds");

	m_system->GetDrawSprite()->AddTextureData(L"Sun",				// 日没メーター
		L"Resources/Textures/PLAY_COMMON/Sun.dds");

	m_system->GetDrawSprite()->AddTextureData(L"HelpBack",			// ヘルプ時の背景暗転
		L"Resources/Textures/PLAY_HELP/HelpBack.dds");

	m_system->GetDrawSprite()->AddTextureData(L"OpenHelp",			// ヘルプを開く表示
		L"Resources/Textures/PLAY_HELP/OpenHelp.dds");

	m_system->GetDrawSprite()->AddTextureData(L"UnderFont",			// 下のテキスト
		L"Resources/Textures/PLAY_HELP/UnderFont.dds");

	m_system->GetDrawSprite()->AddTextureData(L"UnderBack",			// 下のテキストのライン
		L"Resources/Textures/PLAY_HELP/UnderBack.dds");

	m_system->GetDrawSprite()->AddTextureData(L"HowToPlay",			// ヘルプ表示
		L"Resources/Textures/PLAY_HELP/Help.dds");

	m_system->GetDrawSprite()->AddTextureData(L"BlockInfo",			// ブロック情報
		L"Resources/Textures/PLAY_HELP/BlockInfo.dds");

	m_system->GetDrawSprite()->AddTextureData(L"GoScenes",			// シーンセレクト
		L"Resources/Textures/PLAY_HELP/GoScenes.dds");

	m_system->GetDrawSprite()->AddTextureData(L"SelectArrow",		// 選択の矢印
		L"Resources/Textures/PLAY_HELP/SelectArrow.dds");

	// 比率を計算
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	// スプライトの位置を計算
	m_countDownPos = { (FULL_SCREEN_SIZE.x / 2 - NUM_SIZE / 2) * _scale.x , 80.0f * _scale.y };
	m_oneSecPos = { m_countDownPos.x + static_cast<float>(NUM_SIZE / 2) * _scale.x ,m_countDownPos.y * _scale.y};
	m_tenSecPos = { m_countDownPos.x - static_cast<float>(NUM_SIZE / 2) * _scale.x ,m_countDownPos.y * _scale.y};

	m_sunPos = { (SUN_SIZE.x * _scale.x) / 2, SUN_SIZE.y * _scale.y };

	// 矢印座標
	m_targetArrow[RETRY]  = 358.0f;
	m_targetArrow[SELECT] = 500.0f;
	m_targetArrow[TITLE]  = 642.0f;
	m_targetArrow[EXIT]   = 840.0f;

	// ヘルプ座標
	m_moveTexPos = SimpleMath::Vector2::Zero;
	m_pageNum = 1;
	m_helpPoses.emplace(L"HowToPlay", HelpPosition{ SimpleMath::Vector2::Zero ,
		SimpleMath::Vector2::Zero });
	m_helpPoses.emplace(L"BlockInfo", HelpPosition{ SimpleMath::Vector2{ FULL_SCREEN_SIZE.x, 0.0f},
		SimpleMath::Vector2{ FULL_SCREEN_SIZE.x, 0.0f} });
	m_helpPoses.emplace(L"GoScenes", HelpPosition{ SimpleMath::Vector2{ FULL_SCREEN_SIZE.x * 2, 0.0f},
		SimpleMath::Vector2{ FULL_SCREEN_SIZE.x * 2, 0.0f} });

	// 矢印座標
	m_arrowPos.initPos = m_arrowPos.nowPos =
	{ m_helpPoses[L"GoScenes"].initPos.x + (FULL_SCREEN_SIZE.x / 3), m_targetArrow[RETRY]};

	// 落下フラグを切る
	is_effectFlag = false;
	// ヘルプフラグを切る
	is_helpFlag = false;
	// 遷移画面ではない
	is_transFlag = false;
}

// 更新処理
void PlayUI::Update(const float& timelimit)
{
	m_gameTimer = static_cast<int>(timelimit);

	// 比率を計算
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	// 太陽の回転をセット
	m_system->GetDrawSprite()->CreateRotation(L"Sun", static_cast<float>(MAX_LIMITS - m_gameTimer) * ROT_SPEED);

	// 太陽の移動処理
	m_sunPos.x += m_gameTimer != 0 ? SUN_MOVE_SPEED * _scale.x : 0.0f;

	// フォントの移動処理
	UpdateUnderLine(_scale);

	// 変更をリセット
	if (!is_helpFlag)
	{
		m_pageNum = 1;
		m_moveTexPos = SimpleMath::Vector2::Zero;
		m_transNum = RETRY;
		for (auto& pos : m_helpPoses)
		{
			// 現在の座標、初期座標
			MovePositions(&pos.second.nowPos, pos.second.initPos);
		}
		MovePositions(&m_arrowPos.nowPos, m_arrowPos.initPos);
	}
}

// ページをめくる
void PlayUI::UpdatePage(const bool& leftArrow, const bool& rightArrow)
{
	// 移動量を計算
	if (leftArrow)
	{
		m_moveTexPos.x += FULL_SCREEN_SIZE.x;
		m_pageNum--;
	}
	if (rightArrow)
	{
		m_moveTexPos.x -= FULL_SCREEN_SIZE.x;
		m_pageNum++;
	}

	// クランプ処理
	m_moveTexPos.x = UserUtility::Clamp(m_moveTexPos.x, -FULL_SCREEN_SIZE.x * (MAX_PAGE - 1), 0.0f);
	m_pageNum = UserUtility::Clamp(m_pageNum, 1, MAX_PAGE);

	// 遷移画面ならTrueにする
	is_transFlag = static_cast<int>(m_pageNum) == 3 ? true : false;

	for (auto& pos : m_helpPoses)
	{
		// 現在の座標、初期座標＋移動量
		MovePositions(&pos.second.nowPos,pos.second.initPos + m_moveTexPos);
	}

	// X軸のみの移動
	MovePositions(&m_arrowPos.nowPos,
		SimpleMath::Vector2{ m_arrowPos.initPos.x, m_arrowPos.nowPos.y } + m_moveTexPos);
}

// 遷移先の選択
void PlayUI::UpdateTransition(const bool& upArrow, const bool& downArrow)
{
	if (upArrow)
	{
		m_transNum = m_transNum != RETRY ? m_transNum - 1 : EXIT;
	}
	if (downArrow)
	{
		m_transNum = m_transNum != EXIT ? m_transNum + 1 : RETRY;
	}

	// 指定座標に移動
	m_arrowPos.nowPos.y = UserUtility::Lerp(m_arrowPos.nowPos.y, m_targetArrow[m_transNum], 0.1f);
}

// 描画関数
void PlayUI::Render()
{
    // 比率を計算
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	// 落下エフェクト
	if (is_effectFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"Death",                          // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			_scale,                            // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);
	}

	// 太陽アイコンの描画
	RenderSunny(_scale);

	// タイマーの描画
	RenderTimer(_scale);

	//-------------------------------------------------------------------------------------//
	// ヘルプ中の表示
	if (is_helpFlag)
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"HelpBack",                       // 登録キー
			SimpleMath::Vector2::Zero,         // 座標
			{ 1.0f, 1.0f, 1.0f, 1.0f },        // 色
			_scale,                            // 拡大率
			SimpleMath::Vector2::Zero          // 中心位置
		);

		// ページの描画
		RenderHelpPage(_scale);

		// アロー
		if (is_transFlag)
		{
			m_system->GetDrawSprite()->DrawTexture(
				L"SelectArrow",
				m_arrowPos.nowPos * _scale,
				{ 1.0f, 1.0f, 1.0f, 1.0f },
				 _scale,
				ARROW_SIZE / 2,
				RECT_U(0L, 0L, static_cast<LONG>(ARROW_SIZE.x), static_cast<LONG>(ARROW_SIZE.y))
			);
		}
	}
	//-------------------------------------------------------------------------------------//
	// 通常の表示
	else
	{
		m_system->GetDrawSprite()->DrawTexture(
			L"OpenHelp",
			SimpleMath::Vector2{ m_windowSize.x - HELP_WIDTH * _scale.x, 0.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			SimpleMath::Vector2::Zero,
			{ 0L,0L,360L,120L }
		);

		// 画面下のフォント
		m_system->GetDrawSprite()->DrawTexture(
			L"UnderBack",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			SimpleMath::Vector2::Zero
		);
		m_system->GetDrawSprite()->DrawTexture(
			L"UnderFont",
			m_underFontPos,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			SimpleMath::Vector2::Zero,
			// 画像のサイズ
			{ 0L,0L,static_cast<LONG>(FULL_SCREEN_SIZE.x * 2),static_cast<LONG>(FULL_SCREEN_SIZE.y) }
		);
	}
}

// カウントダウン
void PlayUI::RenderCountDown(const float& countDown)
{
	// 比率を計算
	SimpleMath::Vector2 _scale = m_windowSize / FULL_SCREEN_SIZE;

	if (static_cast<int>(countDown / 60) == 0)
	{
		m_countDownEnds -= COUNT_END_SPEED;
		m_system->GetDrawSprite()->DrawTexture(
			L"GameStart",
			{ (m_windowSize / 2).x * _scale.x,
			((m_windowSize / 2).y + m_countDownEnds * COUNT_MOVE_SPEED) * _scale.y },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			_scale,
			m_windowSize / 2
		);
		return;
	}

	int _num = static_cast<int>(countDown) / 60;
	RECT_U _countRec = { _num * NUM_SIZE, 0, _num * NUM_SIZE + NUM_SIZE, NUM_SIZE };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_countDownPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		_scale,
		SimpleMath::Vector2::Zero,
		_countRec
	);
}

// 終了処理
void PlayUI::Finalize()
{
}

// 下のフォントの更新
void PlayUI::UpdateUnderLine(SimpleMath::Vector2 scale)
{
	m_underFontPos.x -= UNDER_SPEED * scale.x;

	if (m_underFontPos.x < -FULL_SCREEN_SIZE.x * 2 * scale.x)
	{
		m_underFontPos.x = FULL_SCREEN_SIZE.x;
	}

}

// タイマーの描画
void PlayUI::RenderTimer(SimpleMath::Vector2 scale)
{
	// 一桁に変換
	int _oneSec = m_gameTimer / 60 % 10;
	int _tenSec = m_gameTimer / 60 / 10 % 10;

	// 切り取り位置設定
	RECT_U _oneRec = { _oneSec * NUM_SIZE, 0, _oneSec * NUM_SIZE + NUM_SIZE, NUM_SIZE };
	RECT_U _tenRec = { _tenSec * NUM_SIZE, 0, _tenSec * NUM_SIZE + NUM_SIZE, NUM_SIZE };

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_tenSecPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		_tenRec
	);

	m_system->GetDrawSprite()->DrawTexture(
		L"Number",
		m_oneSecPos,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero,
		_oneRec
	);
}

// 太陽の描画
void PlayUI::RenderSunny(SimpleMath::Vector2 scale)
{
	m_system->GetDrawSprite()->DrawTexture(
		L"Sun",
		m_sunPos,
		{ 1.0f,1.0f,1.0f,1.0f },
		scale,
		SimpleMath::Vector2{ SUN_SIZE / 2 },
		{ 0L,0L,100L,100L }
	);
}

// ページの描画
void PlayUI::RenderHelpPage(SimpleMath::Vector2 scale)
{
	// Page1
	m_system->GetDrawSprite()->DrawTexture(
		L"HowToPlay",
		GetHelpPosition(L"HowToPlay") * scale,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero
	);

	// Page2
	m_system->GetDrawSprite()->DrawTexture(
		L"BlockInfo",
		GetHelpPosition(L"BlockInfo") * scale,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero
	);

	// Page3
	m_system->GetDrawSprite()->DrawTexture(
		L"GoScenes",
		GetHelpPosition(L"GoScenes") * scale,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		scale,
		SimpleMath::Vector2::Zero
	);
}

// 位置を移動する
void PlayUI::MovePositions(SimpleMath::Vector2* pos,const SimpleMath::Vector2& end)
{
	(*pos) = UserUtility::Lerp(*pos, end, SimpleMath::Vector2{ PAGE_SPEED });
}

// 座標を呼び出す
const SimpleMath::Vector2& PlayUI::GetHelpPosition(const wchar_t* key)
{
	std::map<const wchar_t*, HelpPosition>::const_iterator _it = m_helpPoses.find(key);

	return _it->second.nowPos;
}