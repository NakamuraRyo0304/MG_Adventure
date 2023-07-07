/*
 *	@File	ParticleUtility.h
 *	@Brief	パーティクルユーティリティクラス。
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
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
	DirectX::SimpleMath::Vector3 m_nowScale;
	DirectX::SimpleMath::Vector3 m_startScale;
	DirectX::SimpleMath::Vector3 m_endScale;

	// 生存時間
	float m_life;
	float m_startLife;

	// 色
	DirectX::SimpleMath::Color m_nowColor;
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_endColor;

public:
	// コンストラクタ
	ParticleUtility(
		float life,
		DirectX::SimpleMath::Vector3 pos,
		DirectX::SimpleMath::Vector3 velocity,
		DirectX::SimpleMath::Vector3 accele,
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor);

	// デストラクタ
	~ParticleUtility();

	// 更新
	bool Update(float elapsedTime);

// アクセサ群
	const DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
		
	const DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }

	const DirectX::SimpleMath::Vector3 GetAccele() { return m_accelerate; }
		
	const DirectX::SimpleMath::Vector3 GetNowScale() { return m_nowScale; }
	const DirectX::SimpleMath::Vector3 GetStartScale() { return m_startScale; }
	const DirectX::SimpleMath::Vector3 GetEndScale() { return m_endScale; }

	const float GetLife() { return m_life; }
	const float GetStartLife() { return m_startLife; }
	const DirectX::SimpleMath::Color GetNowColor() { return m_nowColor; }
	const DirectX::SimpleMath::Color GetStartColor() { return m_startColor; }
	const DirectX::SimpleMath::Color GetEndColor() { return m_endColor; }
};

# endif // PARTICLEUTILITY