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
	bool is_cloudCheck;
	bool is_startCheck;

	// プレイヤーの数をチェックする
	int m_playerNum;
	DirectX::SimpleMath::Vector3 m_playerPos;

	// コインの数
	int m_coinNum;
	// コインの距離を測る
	float m_coinLength;
	bool is_clearPossibility;

	// 雲の数
	int m_cloudNum;

private:

	// 範囲
	const float JIDGE = 0.9f * 1.5f;

	// コインのチェックボーダー
	const int CHECK_COIN_NUM = 20;
	const float CHECK_COIN_LENGTH = 12.5f;

public:

	ClearChecker();
	~ClearChecker() = default;

	/// <summary>
	/// マップのセットとその他設定
	/// </summary>
	/// <param name="map">チェック対象のマップ</param>
	/// <returns>なし</returns>
	void SetMap(const std::vector<Object>& map);


	/// <summary>
	 /// クリアチェックを実行
	 /// </summary>
	 /// <param name="引数無し"></param>
	 /// <returns>成功でTrue：失敗でFalse</returns>
	bool RunCheck();

	// プレイヤーの数を取得
	const int& GetPlayerNum() { return m_playerNum; }

	// コインの数の数を取得
	const int& GetCoinNum() { return m_coinNum; }

	// 雲の位置が乗れる位置か判定
	const bool& GetCloudFlag() { return is_cloudCheck; }

	// プレイヤーが正常にスタートできるか判定
	const bool& GetCanStart() { return is_startCheck; }

	// クリアできる可能性が高ければTrueを返す
	const bool& GetClearPossibility() { return is_clearPossibility; }

private:

	// 雲とプレイヤーの座標の比較
	bool CheckSameHeight();

	// 正常スタート可能かチェック
	bool CheckNormalStart();
};

#endif // CLEARCHECKER