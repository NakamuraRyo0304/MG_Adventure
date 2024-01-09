/*
 *	@File	ColliderHelper.cpp
 *	@Brief	当たり判定のヘルパークラス。
 *	@Date	2023-01-09
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/UserUtility.h"
#include "Libraries/SystemManager/SystemManager.h"
#include "Libraries/SystemDatas/MapLoad.h"
#include "Game/PlayScene/Objects/Player.h"
#include "Game/CommonObjects/Blocks.h"
#include "ColliderHelper.h"

 // エイリアス宣言
using BOXHIT = Collider::BoxCollider::HIT_FACE;

// コンストラクタ
ColliderHelper::ColliderHelper(const float& radius, std::shared_ptr<SystemManager> system)
	: m_radius{ radius }
	, m_system{ system }
{
	// 当たり判定の作成
	m_collider = std::make_unique<Collider::BoxCollider>();

	// 接触オブジェクトの初期化
	m_hitObj.clear();
}

// デストラクタ
ColliderHelper::~ColliderHelper()
{
	m_hitObj.clear();
	m_collider.reset();
}

// 更新処理
void ColliderHelper::Update(Player* player, Blocks* blocks)
{
	// 当たり判定の更新
	Judgement(player, blocks);

	// 衝突したオブジェクトごとに押し戻し処理を行う
	for (auto& i : m_hitObj)
	{
		ApplyPushBack(player, blocks, i);
	}
}


// 判定処理
void ColliderHelper::Judgement(Player* player, Blocks* blocks)
{
	// 衝突したオブジェクトリストを初期化
	m_hitObj.clear();

	// 当たり判定を取る
	for (auto& block : blocks->GetMapData())
	{
		// ブロックがなければ処理しない
		if (block.id == MAPSTATE::NONE) continue;

		// プレイヤの判定範囲外は処理しない
		if (not UserUtility::CheckPointInSphere(
			player->GetPosition(), m_radius, block.position)) continue;

		// 判定を取る
		m_collider->Judgement(
			player->GetPosition(), block.position,				// プレイヤ、オブジェクトの座標
			SimpleMath::Vector3{ player->GetSize() },			// プレイヤサイズ
			SimpleMath::Vector3{ blocks->GetObjSize(block.id) }	// ブロックサイズ
		);

		// 当たっていたらコリジョンリストに追加
		if (m_collider->IsHitBoxFlag())
		{
			m_hitObj.push_back(block);
		}
	}
}

// 押し戻し処理
void ColliderHelper::ApplyPushBack(Player* player, Blocks* blocks, Object& obj)
{
	// 当っているオブジェがなしの場合は処理しない
	if (obj.id == MAPSTATE::NONE)
	{
		// 要素を消して終了
		m_hitObj.pop_back();
		return;
	}

	// デフォルトで判定をつける
	m_collider->SetPushMode(true);

	// コインの獲得処理
	if (obj.id == MAPSTATE::COIN)
	{
		// 押し戻ししない
		m_collider->SetPushMode(false);

		// コインカウントアップ
		blocks->CountUpCoin(obj.index);

		// コイン獲得音を鳴らす
		m_system->GetSoundManager()->PlaySound(XACT_WAVEBANK_SKBX_SE_COINGETTER, false);
	}

	// 雲の浮上処理
	if (obj.id == MAPSTATE::CLOUD)
	{
		// プレイヤーが下にいたら押し戻ししない
		if (player->GetPosition().y < obj.position.y + blocks->GetObjSize(MAPSTATE::CLOUD))
		{
			// 要素を消して終了
			m_hitObj.pop_back();
			return;
		}

		// 判定を有効化
		m_collider->SetPushMode(true);

		// インデックス番号を格納
		m_lastIdx.push_back(obj.index);

		// 空なら処理しない
		if (m_lastIdx.empty()) return;

		// 当たっている判定を出す
		blocks->SetCloudHitFlag(m_lastIdx.front(), true);
		m_lastIdx.pop_front();
	}

	// 重力処理
	if (obj.id == MAPSTATE::GRAVITY)
	{
		m_collider->SetPushMode(false);
		blocks->CallGravity();
	}

	// プレイヤーの押し戻し
	SimpleMath::Vector3 _playerPos = player->GetPosition();
	m_collider->Judgement(
		&_playerPos, obj.position,
		SimpleMath::Vector3{ player->GetSize() },
		SimpleMath::Vector3{ blocks->GetObjSize(obj.id) });
	player->SetPosition(_playerPos);

	// ブロックの上に乗っていたら着地判定
	if (m_collider->GetHitFace() == BOXHIT::UP)
	{
		player->ResetGravity();
	}

	// 要素を消して終了
	m_hitObj.pop_back();
}