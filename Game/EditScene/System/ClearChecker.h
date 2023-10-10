/*
 *	@File	ClearChecker.h
 *	@Brief	クリアチェックをするクラス。
 *	@Date	2023-10-07
 *  @Author NakamuraRyo
 */

#ifndef CLEARCHECKER
#define CLEARCHECKER

 // マップデータ読み込み
#include "../../../Libraries/SystemDatas/MapLoad.h"

// エイリアスを宣言
using MAPSTATE = MapLoad::BOXSTATE;

class ClearChecker
{
private:

	// チェック配列
	std::vector<Object> m_checkMap;

	// チェックフラグ
	bool is_checkFlag;

	// プレイヤーの数をチェックする
	int m_playerNum;

	// 範囲
	const float JIDGE = 0.9f * 1.5f;
public:

	ClearChecker();
	~ClearChecker() = default;

	// マップセッター
	void SetMap(const std::vector<Object>& map) { m_checkMap = map; }

	// クリアチェックを実行
	bool RunCheck();

	// プレイヤーの数
	const int& GetPlayerNum() { return m_playerNum; }
};

#endif // CLEARCHECKER