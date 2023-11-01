/*
 *	@File	ParticleUtility.h
 *	@Brief	パーティクルユーティリティクラス。// namespaceは利便性考慮で削除
 *	@Date	2023-07-07
 *  @Author T.Ito
 *  @Editor NakamuraRyo
 */

#pragma once
#ifndef PARTICLEUTILITY
#define PARTICLEUTILITY

class ParticleUtility
{
private:
	// 座標
	DirectX::SimpleMath::Vector3 m_position;

	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;

	// 加速度
	DirectX::SimpleMath::Vector3 m_accelerate;

	// サイズ
	DirectX::SimpleMath::Vector3 m_startScale, m_nowScale, m_endScale;

	// 生存時間
	float m_startLife, m_life;

	// 色
	DirectX::SimpleMath::Color m_startColor, m_nowColor, m_endColor;

public:

	/// <summary>
	/// コンストラクタ
	/// 引数は初期設定用の数値
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="pos">座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accele">加速度</param>
	/// <param name="startScale">初期サイズ</param>
	/// <param name="endScale">最終サイズ</param>
	/// <param name="startColor">初期色</param>
	/// <param name="endColor">最終色</param>
	/// <returns>なし</returns>
	ParticleUtility(
		float life,
		DirectX::SimpleMath::Vector3 pos,
		DirectX::SimpleMath::Vector3 velocity,
		DirectX::SimpleMath::Vector3 accele,
		DirectX::SimpleMath::Vector3 startScale,
		DirectX::SimpleMath::Vector3 endScale,
		DirectX::SimpleMath::Color startColor,
		DirectX::SimpleMath::Color endColor
	);

	// デストラクタ
	~ParticleUtility();

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="timer">時間</param>
	/// <returns>生存時間ありでTrue</returns>
	bool Update(float elapsedTime);

// アクセサ群
public:
	// 座標を取得する
	const DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	// 移動量を取得する
	const DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }
	// 加速度を取得する
	const DirectX::SimpleMath::Vector3 GetAccele() { return m_accelerate; }
	// 現在の大きさを取得する
	const DirectX::SimpleMath::Vector3 GetNowScale() { return m_nowScale; }
	// 最初の大きさを取得する
	const DirectX::SimpleMath::Vector3 GetStartScale() { return m_startScale; }
	// 最終の大きさを取得する
	const DirectX::SimpleMath::Vector3 GetEndScale() { return m_endScale; }
	// 現在の生存時間を取得する
	const float GetLife() { return m_life; }
	// 最初の生存時間を取得する
	const float GetStartLife() { return m_startLife; }
	// 現在の色を取得する
	const DirectX::SimpleMath::Color GetNowColor() { return m_nowColor; }
	// 最初の色を取得する
	const DirectX::SimpleMath::Color GetStartColor() { return m_startColor; }
	// 最後の色を取得する
	const DirectX::SimpleMath::Color GetEndColor() { return m_endColor; }
};

# endif // PARTICLEUTILITY