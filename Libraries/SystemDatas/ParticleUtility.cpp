/*
 *	@File	ParticleUtility.cpp
 *	@Brief	パーティクルユーティリティクラス。// namespaceは利便性考慮で削除
 *	@Date	2023-07-07
 *  @Author T.Ito
 *  @Editor NakamuraRyo
 */

#include "pch.h"

#include <vector>

#include "ParticleUtility.h"

static const float ENDLESS = -100.0f;

// コンストラクタ
ParticleUtility::ParticleUtility(
	float life,
	SimpleMath::Vector3 pos,
	SimpleMath::Vector3 velocity,
	SimpleMath::Vector3 accelerate,
	SimpleMath::Vector3 startScale, SimpleMath::Vector3 endScale,
	SimpleMath::Color startColor, SimpleMath::Color endColor)
{
	// エフェクト継続時間
	m_startLife = life;
	m_life = life;

	// 座標、移動量、加速度
	m_position = pos;
	m_velocity = velocity;
	m_accelerate = accelerate;

	// サイズ
	m_startScale = startScale;
	m_nowScale = startScale;
	m_endScale = endScale;

	// 色
	m_startColor = startColor;
	m_nowColor = startColor;
	m_endColor = endColor;
}

// デストラクタ
ParticleUtility::~ParticleUtility()
{
}

// 更新処理
bool ParticleUtility::Update(float elapsedTime)
{
	// スケール
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);

	// 色
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// 速度の計算
	m_velocity += m_accelerate * elapsedTime;

	// 座標の計算
	m_position += m_velocity * elapsedTime;

	// 生存時間を削る
	m_life -= elapsedTime;

	// ライフが0で自身を削除
	if (m_life < 0.0f)
	{
		return false;
	}

	return true;
}


