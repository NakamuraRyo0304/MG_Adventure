/*
 *	@File	Logo.h
 *	@Brief	タイトルロゴクラス。
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef LOGO
#define LOGO

#include "ITitleObject.h"

class Logo : public ITitleObject
{
private:

	// モデル
	std::unique_ptr<DirectX::Model> m_model;

	// 開始フラグ
	bool is_startFlag;

	// ロゴが目的地に着いたか調べるフラグ
	bool is_endFlag;

	// ロゴの開始時の演出
	float m_logoMoveScale;
	float m_logoMoveY;
	const float END_MOVE_POS = 1.0f;
	const float LOGO_CHANGE_SCALE = 1.05f;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="path">タイトルロゴモデルのパス</param>
	/// <returns>なし</returns>
	Logo(const wchar_t* path);
	~Logo();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="states">コモンステート</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">射影行列</param>
	/// <returns>なし</returns>
	void Render(DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

public:

	// スタートフラグを設定する
	void SetStartFlag(const bool& flag) { is_startFlag = flag; }

	// ロゴの移動が終了地点に達したか判定する
	const bool& IsLogoEndFlag() { return is_endFlag; }
};

#endif // LOGO