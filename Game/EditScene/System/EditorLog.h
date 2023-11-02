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

class MementoMap;
class EditorLog
{
private:

	// 保存した配列
	std::vector<MementoMap> m_histories;

	// 現在のインデックス
	size_t m_currentIndex;

public:

	EditorLog();
	~EditorLog();

	/// <summary>
	/// 状態を蓄積していく関数
	/// </summary>
	/// <param name="history">変更点</param>
	/// <returns>なし</returns>
	void AddHistory(const MementoMap& history);

	/// <summary>
	/// ひとつ前の状態に戻す
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>ひとつ前の状態</returns>
	const MementoMap& GetUndo();

	/// <summary>
	/// Undoを取り消す
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>前回のUndoの前の状態</returns>
	const MementoMap& GetRedo();
};

#endif // EDITORLOG