/*
 *	@File	LeftLeg.h
 *	@Brief	プレイヤーの左足。
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef LEFTLEG
#define LEFTLEG

 // 基底クラス
#include "../IParts.h"

class LeftLeg : public IParts
{
private:


public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>なし</returns>
	LeftLeg(std::unique_ptr<DirectX::Model> model);
	~LeftLeg();

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

#endif // LEFTLEG