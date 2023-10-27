/*
 *	@File	MementoMap.h
 *	@Brief	マップの過去の情報を保存するクラス。
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MEMENTMAP
#define MEMENTMAP

 // 構造体取得
#include "Libraries/SystemDatas/MapLoad.h"

class MementoMap
{
private:
	// 保存
	std::vector<Object> m_state;

public:

	MementoMap(const std::vector<Object>& state);
	~MementoMap();

	// ステートゲッター
	const std::vector<Object>& GetState();
};

#endif // MEMENTMAP