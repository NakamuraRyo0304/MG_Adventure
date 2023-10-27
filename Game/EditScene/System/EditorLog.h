/*
 *	@File	EditorLog.h
 *	@Brief	マップのログの保存先。
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef EDITORLOG
#define EDITORLOG

 // 構造体取得
#include "Libraries/SystemDatas/MapLoad.h"

class MementMap;
class EditorLog
{
private:

	// 保存した配列
	std::vector<MementMap> m_histories;

	// 現在のインデックス
	size_t m_currentIndex;

public:

	EditorLog();
	~EditorLog();

	// 状態を蓄積していく関数
	void AddHistory(const MementMap& history);

	const MementMap& GetUndo();
	const MementMap& GetRedo();
};

#endif // EDITORLOG