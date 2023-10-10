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
	: m_checkMap{}			// マップデータ
	, m_playerNum{0}		// プレイヤーの数
	, is_checkFlag{false}	// チェックフラグ
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
	bool checkPlayer = false;
	bool checkCoin = false;

	// 必須要素が含まれているか
	for (auto& idx : m_checkMap)
	{
		for (auto& jud : m_checkMap)
		{
			if (idx.id == MAPSTATE::PLAYER)
			{
				if (checkPlayer)
				{
					m_playerNum++;
				}
				else
				{
					// プレイヤーがいたらTrue
					checkPlayer = true;
				}
			}
			if (idx.id == MAPSTATE::COIN)
			{
				// コインがあればTrue
				checkCoin = true;
			}
		}
	}

	return checkPlayer && checkCoin;
}
