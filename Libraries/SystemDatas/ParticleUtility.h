/*
 *	@File	ParticleUtility.h
 *	@Brief	�p�[�e�B�N�����[�e�B���e�B�N���X�B// namespace�͗��֐��l���ō폜
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
	// ���W
	DirectX::SimpleMath::Vector3 m_position;

	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;

	// �����x
	DirectX::SimpleMath::Vector3 m_accelerate;

	// �T�C�Y
	DirectX::SimpleMath::Vector3 m_startScale, m_nowScale, m_endScale;

	// ��������
	float m_startLife, m_life;

	// �F
	DirectX::SimpleMath::Color m_startColor, m_nowColor, m_endColor;

public:

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

	// �f�X�g���N�^
	~ParticleUtility();

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	/// <param name="timer">����</param>
	/// <returns>�������Ԃ����True</returns>
	bool Update(float elapsedTime);

// �A�N�Z�T�Q
public:
	// ���W���擾����
	const DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	// �ړ��ʂ��擾����
	const DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }
	// �����x���擾����
	const DirectX::SimpleMath::Vector3 GetAccele() { return m_accelerate; }
	// ���݂̑傫�����擾����
	const DirectX::SimpleMath::Vector3 GetNowScale() { return m_nowScale; }
	// �ŏ��̑傫�����擾����
	const DirectX::SimpleMath::Vector3 GetStartScale() { return m_startScale; }
	// �ŏI�̑傫�����擾����
	const DirectX::SimpleMath::Vector3 GetEndScale() { return m_endScale; }
	// ���݂̐������Ԃ��擾����
	const float GetLife() { return m_life; }
	// �ŏ��̐������Ԃ��擾����
	const float GetStartLife() { return m_startLife; }
	// ���݂̐F���擾����
	const DirectX::SimpleMath::Color GetNowColor() { return m_nowColor; }
	// �ŏ��̐F���擾����
	const DirectX::SimpleMath::Color GetStartColor() { return m_startColor; }
	// �Ō�̐F���擾����
	const DirectX::SimpleMath::Color GetEndColor() { return m_endColor; }
};

# endif // PARTICLEUTILITY