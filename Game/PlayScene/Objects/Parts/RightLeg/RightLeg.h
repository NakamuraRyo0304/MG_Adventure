/*
 *	@File	RightLeg.h
 *	@Brief	プレイヤーの右足。
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef RIGHTLEG
#define RIGHTLEG

 // 基底クラス
#include "../IParts.h"

class RightLeg : public IParts
{
private:


public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>なし</returns>
	RightLeg(std::unique_ptr<DirectX::Model> model);
	~RightLeg();

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

#endif // RIGHTLEG