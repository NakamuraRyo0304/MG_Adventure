/*
 *	@File	MementoMap.cpp
 *	@Brief	マップの過去の情報を保存するクラス。
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "MementoMap.h"

// コンストラクタ
MementoMap::MementoMap(const std::vector<Object>& state)
	: m_state{ state }
{
}

// デストラクタ
MementoMap::~MementoMap()
{
	m_state.clear();
}

// ステートを取得
const std::vector<Object>& MementoMap::GetState()
{
	return m_state;
}

