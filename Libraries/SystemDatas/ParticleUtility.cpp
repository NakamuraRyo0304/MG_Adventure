/*
 *	@File	ParticleUtility.cpp
 *	@Brief	�p�[�e�B�N�����[�e�B���e�B�N���X�B// namespace�͗��֐��l���ō폜
 *	@Date	2023-07-07
 *  @Author T.Ito
 *  @Editor NakamuraRyo
 */

#include "pch.h"

#include <vector>

#include "ParticleUtility.h"

static const float ENDLESS = -100.0f;

// �R���X�g���N�^
ParticleUtility::ParticleUtility(
	float life,
	SimpleMath::Vector3 pos,
	SimpleMath::Vector3 velocity,
	SimpleMath::Vector3 accelerate,
	SimpleMath::Vector3 startScale, SimpleMath::Vector3 endScale,
	SimpleMath::Color startColor, SimpleMath::Color endColor)
{
	// �G�t�F�N�g�p������
	m_startLife = life;
	m_life = life;

	// ���W�A�ړ��ʁA�����x
	m_position = pos;
	m_velocity = velocity;
	m_accelerate = accelerate;

	// �T�C�Y
	m_startScale = startScale;
	m_nowScale = startScale;
	m_endScale = endScale;

	// �F
	m_startColor = startColor;
	m_nowColor = startColor;
	m_endColor = endColor;
}

// �f�X�g���N�^
ParticleUtility::~ParticleUtility()
{
}

// �X�V����
bool ParticleUtility::Update(float elapsedTime)
{
	// �X�P�[��
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);

	// �F
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// ���x�̌v�Z
	m_velocity += m_accelerate * elapsedTime;

	// ���W�̌v�Z
	m_position += m_velocity * elapsedTime;

	// �������Ԃ����
	m_life -= elapsedTime;

	// ���C�t��0�Ŏ��g���폜
	if (m_life < 0.0f)
	{
		return false;
	}

	return true;
}


