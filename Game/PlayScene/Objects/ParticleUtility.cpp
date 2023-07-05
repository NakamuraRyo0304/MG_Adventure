//--------------------------------------------------------------------------------------
// File: ParticleUtil.cpp
//
// �p�[�e�B�N�����[�e�B���e�B�N���X
// position�⑬�x�Ȃǃp�[�e�B�N���P�ɕK�v�ȗv�f���i�[
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Particleutility.h"

#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

using namespace DirectX;

const static float ENDLESS = -100.0f;

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
tito::ParticleUtility::ParticleUtility(
	float life,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accele,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor)
{

	m_startLife =
		m_life = life;

	m_position = pos;
	m_velocity = velocity;
	m_accele = accele;

	m_startScale =
		m_nowScale = startScale;
	m_endScale = endScale;


	m_startColor =
		m_nowColor = startColor;
	m_endColor = endColor;
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
tito::ParticleUtility::~ParticleUtility()
{
}
/// <summary>
/// �X�V�֐�
/// </summary>
/// <param name="timer">Game������StepTimer���󂯎��</param>
/// <returns>��������(life)������Ԃ�true</returns>
bool tito::ParticleUtility::Update(float elapsedTime)
{
	// �X�P�[��
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	// �F
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);
	// ���x�̌v�Z
	m_velocity += m_accele * elapsedTime;
	// ���W�̌v�Z
	m_position += m_velocity * elapsedTime;
	m_life -= elapsedTime;
	// ���C�t���O�Ŏ��g�������Ă��炤
	if (m_life < 0.0f)
	{
		return false;
	}

	return true;
}


