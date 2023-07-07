/*
 *	@File	ParticleUtility.h
 *	@Brief	�p�[�e�B�N�����[�e�B���e�B�N���X�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PARTICLEUTILITY
#define PARTICLEUTILITY

class ParticleUtility
{
private:
	// ���W
	DirectX::SimpleMath::Vector3 m_position;

	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;

	// �����x
	DirectX::SimpleMath::Vector3 m_accelerate;

	// �T�C�Y
	DirectX::SimpleMath::Vector3 m_nowScale;
	DirectX::SimpleMath::Vector3 m_startScale;
	DirectX::SimpleMath::Vector3 m_endScale;

	// ��������
	float m_life;
	float m_startLife;

	// �F
	DirectX::SimpleMath::Color m_nowColor;
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_endColor;

public:
	// �R���X�g���N�^
	ParticleUtility(
		float life,
		DirectX::SimpleMath::Vector3 pos,
		DirectX::SimpleMath::Vector3 velocity,
		DirectX::SimpleMath::Vector3 accele,
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor);

	// �f�X�g���N�^
	~ParticleUtility();

	// �X�V
	bool Update(float elapsedTime);

// �A�N�Z�T�Q
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