/*
 *	@File	Player.cpp
 *	@Brief	プレイヤの処理をするクラス。
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/FactoryManager/FactoryManager.h"
#include "Parts/Head/Head.h"
#include "Parts/Body/Body.h"
#include "Parts/RightLeg/RightLeg.h"
#include "Parts/LeftLeg/LeftLeg.h"
#include "Player.h"

// コンストラクタ
Player::Player(std::unique_ptr<Model> head, std::unique_ptr<Model> body,
			   std::unique_ptr<Model> right, std::unique_ptr<Model> left)
	: m_coinNum{}					// 合計獲得コイン数
	, is_deathFlag{}				// 死亡フラグ
{
	m_factory = std::make_unique<FactoryManager>();
	m_parent = std::make_unique<Body>(std::move(body));
	m_head = std::make_unique<Head>(std::move(head));
	m_legR = std::make_unique<RightLeg>(std::move(right));
	m_legL = std::make_unique<LeftLeg>(std::move(left));
}

// デストラクタ
Player::~Player()
{
	Finalize();
}

// 初期化処理
void Player::Initialize()
{
	// ファクトリの作成
	m_factory->CreateFactory();

	// パラメータのリセット
	m_parent->ResetAll();

	// 死亡判定の初期化
	is_deathFlag = false;
}

// 更新処理
void Player::Update()
{
	// 親の更新
	m_parent->Update();

	// 死亡判定
	is_deathFlag = m_parent->GetPosition().y < DEATH_LINE;

	// 頭の更新処理
	m_head->Update();

	// 足の更新処理
	m_legR->Update();
	m_legL->Update();
}

// 描画処理
void Player::Render(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _world = m_parent->GetMatrix();

	// 右足の描画
	m_legR->SetParentMatrix(_world);
	m_legR->Draw(states, view, proj);

	// 左足の描画
	m_legL->SetParentMatrix(_world);
	m_legL->Draw(states, view, proj);

	// 頭部の描画
	m_head->SetParentMatrix(_world);
	m_head->Draw(states, view, proj);

	// 身体の描画
	m_parent->SetParentMatrix(SimpleMath::Matrix::Identity);
	m_parent->Draw(states, view, proj);
}

// 終了処理
void Player::Finalize()
{
	m_head.reset();
	m_legR.reset();
	m_legL.reset();
	m_parent.reset();
	m_factory.reset();
}

// 新規作成
void Player::NewCreate()
{
	// ファクトリマネージャ
	m_factory->BuildModelFactory();

	// ファクトリーからモデルをもらう
	auto _head = m_factory->VisitModelFactory()->GetCreateModel(L"Resources/Models/Head.cmo");
	auto _body = m_factory->VisitModelFactory()->GetCreateModel(L"Resources/Models/Body.cmo");
	auto _legR = m_factory->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegR.cmo");
	auto _legL = m_factory->VisitModelFactory()->GetCreateModel(L"Resources/Models/LegL.cmo");

	// 新規作成
	m_parent = std::make_unique<Body>(std::move(_body));
	m_head	 = std::make_unique<Head>(std::move(_head));
	m_legR   = std::make_unique<RightLeg>(std::move(_legR));
	m_legL   = std::make_unique<LeftLeg>(std::move(_legL));

	// ファクトリを破棄
	m_factory->LeaveModelFactory();
}

// リスポーン関数
void Player::Spawn(SimpleMath::Vector3 spawnPosition)
{
	// パラメータの初期化
	m_parent->ResetAll();

	// 座標の設定
	m_parent->SetPosition(spawnPosition);
}

// 座標の取得
const SimpleMath::Vector3& Player::GetPosition()
{
	if (not m_parent)
	{
		NewCreate();
	}
	return m_parent->GetPosition();
}

// 座標の設定
void Player::SetPosition(const SimpleMath::Vector3& position)
{
	if (not m_parent)
	{
		NewCreate();
	}
	m_parent->SetPosition(position);
}

// 重力のリセット
void Player::ResetGravity()
{
	m_parent->ZeroGravity();
}

// 重力を取得
const float& Player::GetGravity()
{
	if (not m_parent)
	{
		NewCreate();
	}
	return m_parent->GetGravity();
}

// 重力の設定
void Player::SetGravity(const float& gravity)
{
	m_parent->SetGravity(gravity);
}

// 回転の取得
const SimpleMath::Quaternion& Player::GetRotation()
{
	if (not m_parent)
	{
		NewCreate();
	}
	return m_parent->GetRotation();
}
