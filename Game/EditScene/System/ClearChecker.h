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
	bool is_playerCheck;
	bool is_coinCheck;

	// プレイヤーの数をチェックする
	int m_playerNum;

	// コインの数
	int m_coinNum;
	// コインの距離を測る
	float m_coinLength;
	bool is_coinPossibility;

private:
	// 範囲
	const float JIDGE = 0.9f * 1.5f;

	// コインのチェックボーダー
	const int CHECK_COIN_NUM = 20;

	const float CHECK_COIN_LENGTH = 12.5f;

public:

	ClearChecker();
	~ClearChecker() = default;

	// マップセッター
	void SetMap(const std::vector<Object>& map) { m_checkMap = map; }

	// クリアチェックを実行
	bool RunCheck();

	// プレイヤーの数
	const int& GetPlayerNum() { return m_playerNum; }

	// クリアできる可能性が高ければTrueを返す
	const bool& GetCoinCheck();
};

#endif // CLEARCHECKER