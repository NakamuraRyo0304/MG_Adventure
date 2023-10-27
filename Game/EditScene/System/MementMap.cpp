/*
 *	@File	MementMap.cpp
 *	@Brief	マップの過去の情報を保存するクラス。
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "MementMap.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="state">変更があったオブジェクト</param>
 /// <returns>なし</returns>
MementMap::MementMap(const std::vector<Object>& state)
	: m_state{ state }
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
MementMap::~MementMap()
{
	m_state.clear();
}

/// <summary>
/// ステートゲッター
/// </summary>
/// <param name="引数無し"></param>
/// <returns>前回のステート</returns>
const std::vector<Object>& MementMap::GetState()
{
	return m_state;
}

