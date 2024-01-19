/*
 *	@File	Body.h
 *	@Brief	プレイヤーの身体。
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef BODY
#define BODY

 // 基底クラス
#include "../IParts.h"

class Body : public IParts
{
private:

	struct Parameter
	{
		DirectX::SimpleMath::Vector3 position;		// 座標
		DirectX::SimpleMath::Vector3 velocity;		// 移動量
		DirectX::SimpleMath::Quaternion rotation;		// 向いている方向
		float gravity;								// 重力
		float accelerate;							// 加速度
		void reset()								// リセット関数
		{
			position = DirectX::SimpleMath::Vector3::Zero;
			velocity = DirectX::SimpleMath::Vector3::Zero;
			gravity = 0.0f;
			accelerate = 0.0f;
			rotation = DirectX::SimpleMath::Quaternion::Identity;
		}
	} m_param;

	// 移動速度
	const float MOVE_SPEED = 0.01f;

	// 回転速度
	const float ROT_SPEED = 0.05f;

	// 摩擦係数
	const float DECELERATION = 0.851f;

	// プレイヤのオフセット
	const float OFFSET_Y = 0.2f;
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>なし</returns>
	Body(std::unique_ptr<DirectX::Model> model);
	~Body();

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

public:

	// パラメータのリセット
	void ResetAll();

	// 座標の取得
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_param.position; }
	// 座標の設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_param.position = pos; }
	// 重力の取得
	const float& GetGravity() { return m_param.gravity; }
	// 重力を0にする
	void ZeroGravity() { m_param.gravity = 0.0f; }
	// 重力の設定
	void SetGravity(const float& gravity) { m_param.gravity = gravity; }
	// 回転の取得
	const DirectX::SimpleMath::Quaternion& GetRotation() { return m_param.rotation; }
};

#endif // BODY