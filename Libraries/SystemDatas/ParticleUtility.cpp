/*
 *	@File	ParticleUtility.cpp
 *	@Brief	�p�[�e�B�N�����[�e�B���e�B�N���X�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "StepTimer.h"
#include "DeviceResources.h"
#include <vector>

#include "ParticleUtility.h"

static const float ENDLESS = -100.0f;

/// <summary>
/// �R���X�g���N�^
/// �����͏����ݒ�p�̐��l
/// </summary>
/// <param name="life">��������</param>
/// <param name="pos">���W</param>
/// <param name="velocity">���x</param>
/// <param name="accele">�����x</param>
/// <param name="startScale">�����T�C�Y</param>
/// <param name="endScale">�ŏI�T�C�Y</param>
/// <param name="startColor">�����F</param>
/// <param name="endColor">�ŏI�F</param>
/// <returns>�Ȃ�</returns>
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

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <returns>�Ȃ�</returns>
ParticleUtility::~ParticleUtility()
{
}

/// <summary>
/// �X�V�֐�
/// </summary>
/// <param name="timer">����</param>
/// <returns>�������Ԃ����True</returns>
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


