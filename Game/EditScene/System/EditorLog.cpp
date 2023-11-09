/*
 *	@File	EditorLog.cpp
 *	@Brief	マップのログの保存先。
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "MementoMap.h"
#include "EditorLog.h"

// コンストラクタ
EditorLog::EditorLog()
	: m_currentIndex{0}
	, m_histories{}
{
	m_histories.clear();
	m_currentIndex = 0;
}

// デストラクタ
EditorLog::~EditorLog()
{
	m_histories.clear();
}

// 履歴を蓄積する
void EditorLog::AddHistory(const MementoMap& history)
{
	// 変更点をまとめた情報を保存
	m_histories.push_back(history);
	m_currentIndex = m_histories.size() - 1;
}

// Undo機能
const MementoMap& EditorLog::GetUndo()
{
	if (m_currentIndex > 0)
	{
		m_currentIndex--;
	}
	return m_histories[m_currentIndex];
}

// Redo機能
const MementoMap& EditorLog::GetRedo()
{
	if (m_currentIndex < m_histories.size() - 1)
	{
		m_currentIndex++;
	}
	return m_histories[m_currentIndex];
}

