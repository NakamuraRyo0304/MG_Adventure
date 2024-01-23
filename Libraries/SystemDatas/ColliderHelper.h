/*
 *	@File	ColliderHelper.h
 *	@Brief	当たり判定のヘルパークラス。
 *	@Date	2023-01-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef COLLIDERHELPER
#define COLLIDERHELPER

// コライダー
#include "Libraries/SystemDatas/Collider.h"
#include "Game/CommonObjects/Object.h"

class Player;
class Blocks;
class SystemManager;
class ColliderHelper
{
private:

	// 判定を行う半径
	float m_radius;

	// オブジェクトの判定
	std::vector<Object> m_hitObj;

	// 前回のインデックス
	std::deque<UINT> m_lastIdx;

	// 判定
	std::unique_ptr<Collider::BoxCollider> m_collider;

	// コインのサウンド用
	std::shared_ptr<SystemManager> m_system;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="radius">判定の半径</param>
	/// <param name="system">システムマネージャポインタ</param>
	/// <returns>なし</returns>
	ColliderHelper(const float& radius, std::shared_ptr<SystemManager> system);
	~ColliderHelper();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="player">プレイヤポインタ</param>
	/// <param name="blocks">ブロックポインタ</param>
	/// <returns>なし</returns>
	void Update(Player* player, Blocks* blocks);


private:

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="player">プレイヤポインタ</param>
	/// <param name="blocks">ブロックポインタ</param>
	/// <returns>なし</returns>
	void Judgement(Player* player, Blocks* blocks);

	/// <summary>
	/// 押し戻し処理
	/// </summary>
	/// <param name="player">プレイヤポインタ</param>
	/// <param name="blocks">ブロックポインタ</param>
	/// <param name="obj">当たったオブジェクト</param>
	/// <returns>なし</returns>
	void ApplyPushBack(Player* player, Blocks* blocks, Object& obj);

};

#endif // COLLIDERHELPER
