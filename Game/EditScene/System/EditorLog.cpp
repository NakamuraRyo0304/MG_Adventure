/*
 *	@File	EditorLog.cpp
 *	@Brief	マップのログの保存先。
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#include "pch.h"

 // メメント
#include "MementMap.h"

#include "EditorLog.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
EditorLog::EditorLog()
	: m_currentIndex{0}
	, m_histories{}
{
	m_histories.clear();
	m_currentIndex = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
EditorLog::~EditorLog()
{
	m_histories.clear();
}

/// <summary>
/// 状態を蓄積していく関数
/// </summary>
/// <param name="history">変更点</param>
/// <returns>なし</returns>
void EditorLog::AddHistory(const MementMap& history)
{
	// 変更点をまとめた情報を保存
	m_histories.push_back(history);
	m_currentIndex = m_histories.size() - 1;
}

/// <summary>
/// ひとつ前の状態に戻す
/// </summary>
/// <param name="引数無し"></param>
/// <returns>ひとつ前の状態</returns>
const MementMap& EditorLog::GetUndo()
{
	if (m_currentIndex > 0)
	{
		m_currentIndex--;
	}
	return m_histories[m_currentIndex];
}

/// <summary>
/// Undoを取り消す
/// </summary>
/// <param name="引数無し"></param>
/// <returns>前回のUndoの前の状態</returns>
const MementMap& EditorLog::GetRedo()
{
	if (m_currentIndex < m_histories.size() - 1)
	{
		m_currentIndex++;
	}
	return m_histories[m_currentIndex];
}

