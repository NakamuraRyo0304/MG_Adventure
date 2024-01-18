/*
 *	@File	Head.h
 *	@Brief	プレイヤーの頭。
 *	@Date	2023-01-18
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef HEAD
#define HEAD

// 基底クラス
#include "../IParts.h"

class Head : public IParts
{
private:

	// 前後移動量
	float m_moveFB;

	// 首の回転
	DirectX::SimpleMath::Vector2 m_rotate;
	DirectX::SimpleMath::Quaternion m_normalRotate;

	// 頭の前後速度
	const float FB_SPEED = 0.25f;

	// 首の回転速度
	const float NECK_ROT_SPEED = 0.30f;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>なし</returns>
	Head(std::unique_ptr<DirectX::Model> model);
	~Head();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="states">コモンステートポインタ</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">プロジェクション行列</param>
	/// <returns>なし</returns>
	void Draw(DirectX::CommonStates& states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;

};

#endif // HEAD