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
	, m_playerPos{}				// プレイヤーの座標
	, m_coinNum{0}				// コインの数
	, m_coinLength{0.0f}		// コインの距離
	, m_cloudNum{0}				// 雲の数
	, is_coinPossibility{false}	// クリアできる想定範囲か
	, is_playerCheck{false}		// プレイヤーフラグ
	, is_coinCheck{false}		// コインフラグ
	, is_cloudCheck{false}		// 雲フラグ
	, is_startCheck{false}		// スタートフラグ
{
}

/// <summary>
/// マップのセットとその他設定
/// </summary>
/// <param name="map">チェック対象のマップ</param>
/// <returns>なし</returns>
void ClearChecker::SetMap(const std::vector<Object>& map)
{
	// 捜査するマップを格納
	m_checkMap = map;

	// 数のリセット
	m_playerNum = 0;
	m_coinNum = 0;
	m_cloudNum = 0;

	// 座標
	m_playerPos = SimpleMath::Vector3::Zero;

	// チェックフラグ
	is_playerCheck = false;
	is_coinCheck = false;
	is_cloudCheck = false;
	is_startCheck = false;
	is_coinPossibility = false;

	// 距離
	m_coinLength = 0.0f;
}

/// <summary>
 /// クリアチェックを実行
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>成功でTrue：失敗でFalse</returns>
bool ClearChecker::RunCheck()
{
	// オブジェクトの数を計算
	for (int i = 0; i < m_checkMap.size(); i++)
	{
		if (m_checkMap[i].id == MAPSTATE::PLAYER)
		{
			m_playerNum++;
			m_playerPos = m_checkMap[i].position;
		}
		if (m_checkMap[i].id == MAPSTATE::COIN)
		{
			m_coinNum++;
		}
		if (m_checkMap[i].id == MAPSTATE::CLOUD)
		{
			m_cloudNum++;
		}
	}

	// コインの数が規定量を越えていないか計測
	is_coinPossibility = m_coinNum < CHECK_COIN_NUM ? true : false;

	// コインとコインの距離を計算する
	for (auto& i : m_checkMap)
	{
		for (auto& j : m_checkMap)
		{
			// コインじゃなければ処理を飛ばす
			if (i.id != MAPSTATE::COIN || j.id != MAPSTATE::COIN) continue;

			// 距離を計算して最大を越えたら入れ替える
			 float check = SimpleMath::Vector3::Distance(i.position, j.position);

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

	// 座標比較
	is_cloudCheck = CheckSameHeight();

	// 正常スタートが可能かチェック
	is_startCheck = CheckNormalStart();

	return is_playerCheck && is_coinCheck && is_cloudCheck && is_startCheck;
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

/// <summary>
/// プレイヤーとコインの座標比較
/// </summary>
/// <param name="引数無し"></param>
/// <returns>雲が乗れる位置にあればTrue</returns>
bool ClearChecker::CheckSameHeight()
{
	// 雲がなければ比較せず抜ける
	if (m_cloudNum == 0) return true;

	for (auto& i : m_checkMap)
	{
		if (i.id == MAPSTATE::CLOUD)
		{
			// 乗れればTrue、乗れなければFalseを入れる
			if (static_cast<int>(m_playerPos.y) > static_cast<int>(i.position.y))
			{
				return true;
			}
		}
	}

	return false;
}

/// <summary>
/// 正常にスタート可能かチェック
/// </summary>
/// <param name="引数無し"></param>
/// <returns>可能ならTrue</returns>
bool ClearChecker::CheckNormalStart()
{
	for (auto& i : m_checkMap)
	{
		// ブロックがあればスキップ
		if (i.id != MAPSTATE::NONE) continue;

		// プレイヤーの下にブロックがなければfalseを返す
		if (static_cast<int>(i.position.x) == static_cast<int>(m_playerPos.x) &&
			static_cast<int>(i.position.z) == static_cast<int>(m_playerPos.z) &&
			static_cast<int>(i.position.y) <  static_cast<int>(m_playerPos.y))
		{
			return false;
		}
	}

	// チェックを潜り抜けたら正常スタート可能
	return true;
}
