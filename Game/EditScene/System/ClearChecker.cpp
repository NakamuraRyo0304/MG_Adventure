/*
 *	@File	ClearChecker.cpp
 *	@Brief	クリアチェックをするクラス。
 *	@Date	2023-10-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ユーザユーティリティ
#include "../../../Libraries/UserUtility.h"

#include "ClearChecker.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
ClearChecker::ClearChecker()
	: m_checkMap{}				// マップデータ
	, m_playerNum{0}			// プレイヤーの数
	, m_coinNum{0}				// コインの数
	, m_coinLength{0.0f}		// コインの距離
	, is_coinPossibility{false}	// クリアできる想定範囲か
	, is_playerCheck{false}		// プレイヤーフラグ
	, is_coinCheck{false}		// コインフラグ
{
}

/// <summary>
 /// クリアチェックを実行
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>成功でTrue：失敗でFalse</returns>
bool ClearChecker::RunCheck()
{
	m_playerNum = 0;
	m_coinNum = 0;

	is_playerCheck = false;
	is_coinCheck = false;
	is_coinPossibility = true;

	// プレイヤーの数を計算
	for (int i = 0; i < m_checkMap.size(); i++)
	{
		if (m_checkMap[i].id == MAPSTATE::PLAYER)
		{
			m_playerNum++;
		}
		if (m_checkMap[i].id == MAPSTATE::COIN)
		{
			m_coinNum++;
		}
	}

	// コインの数が規定量を越えていないか計測
	is_coinPossibility = m_coinNum < CHECK_COIN_NUM ? true : false;

	// コインとコインの距離を計算する
	m_coinLength = 0.0f;

	for (auto& i : m_checkMap)
	{
		for (auto& j : m_checkMap)
		{
			if (i.id != MAPSTATE::COIN || j.id != MAPSTATE::COIN) continue;

			float check = 0.0f;

			// 距離を計算して最大を越えたら入れ替える
			check = SimpleMath::Vector3::Distance(i.position, j.position);

			if (check > m_coinLength)
			{
				m_coinLength = check;
			}
		}
	}

	// コイン間の距離が規定オーバーか計測
	is_coinPossibility = m_coinLength < CHECK_COIN_LENGTH ? true : false;

	// プレイヤーが１体しかいないことを確認
	is_playerCheck = m_playerNum < 2 ? true : false;

	// コインが置いてあることを確認
	is_coinCheck = m_coinNum > 0 ? true : false;

	return is_playerCheck && is_coinCheck;
}

/// <summary>
/// クリアできる可能性が高ければTrueを返す
/// </summary>
/// <param name="引数無し"></param>
/// <returns>Summaryと同じ</returns>
const bool& ClearChecker::GetCoinCheck()
{
	return is_coinPossibility;
}
