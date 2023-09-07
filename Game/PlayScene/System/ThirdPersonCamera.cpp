/*
 *	@File	ThurdPersonCamera.cpp
 *	@Brief	�O�l�̎��_�̃J�����B
 *	@Date	2023-08-05
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Libraries/SystemManager/SystemManager.h"

#include "ThirdPersonCamera.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="system">�V�X�e���}�l�[�W��</param>
/// <param name="context">�R���e�L�X�g�|�C���^</param>
/// <param name="device">�f�o�C�X�|�C���^</param>
/// <returns>�Ȃ�</returns>
ThirdPersonCamera::ThirdPersonCamera(std::shared_ptr<SystemManager> system,
	ID3D11DeviceContext1* context, ID3D11Device1* device)
	: m_adhesionTimer{ 0.0f }
	, m_switching{adhesionType::first}
	, is_changeFlag{false}
{
	m_system = system;

	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	m_system->GetDrawSprite()->AddTextureData(L"Adhesion1", L"Resources/Textures/ADHESION/Adhesion1.dds", device);
	m_system->GetDrawSprite()->AddTextureData(L"Adhesion2", L"Resources/Textures/ADHESION/Adhesion2.dds", device);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
ThirdPersonCamera::~ThirdPersonCamera()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="pos">��_</param>
/// <param name="rotate">��]��</param>
/// <param name="distance">��_����̋���</param>
/// <returns>�Ȃ�</returns>
void ThirdPersonCamera::UpdateFollow(const SimpleMath::Vector3& pos, const SimpleMath::Quaternion& rotate,
	const SimpleMath::Vector3& distance)
{
	// �J�����̖ڐ��̕������v���C���[�̉�]�ɍ��킹�Čv�Z
	SimpleMath::Vector3 forward =
		SimpleMath::Vector3::Transform(SimpleMath::Vector3::UnitZ, rotate);

	SimpleMath::Vector3 newPos = { pos.x,pos.y - 0.5f,pos.z };

	// �J�����̈ʒu�Ɩڐ����v�Z
	SimpleMath::Vector3 eye =
		newPos + SimpleMath::Vector3::Transform(distance, rotate);
	SimpleMath::Vector3 target = eye - forward;

	// �r���[�s��
	m_followView = SimpleMath::Matrix::CreateLookAt(eye, target, SimpleMath::Vector3::Up);

	//-------------------------------------------------------------------------------------//
	// �摜�p�ϐ��X�V
	m_adhesionTimer += is_changeFlag ? 1 : -1;

	if (m_adhesionTimer > CHANGE_SPAN || m_adhesionTimer < 0)
	{
		switch (m_switching)
		{
		case adhesionType::first:
			m_switching = second;
			break;
		case adhesionType::second:
			m_switching = third;
			break;
		case adhesionType::third:
			m_switching = fourth;
			break;
		case adhesionType::fourth:
			m_switching = fifth;
			break;
		case adhesionType::fifth:
			m_switching = sixth;
			break;
		case adhesionType::sixth:
			m_switching = first;
			break;
		default:
			break;
		}
		is_changeFlag = !is_changeFlag;
	}
}

/// <summary>
/// �_�̕t����\��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ThirdPersonCamera::DrawAdhesion()
{
	// ��ʃT�C�Y�̔䗦
	SimpleMath::Vector2 scale = { m_system->GetDeviceResources()->GetOutputSize().right  / 1920.0f ,
								  m_system->GetDeviceResources()->GetOutputSize().bottom / 1080.0f };

	switch (m_switching)
	{
	case adhesionType::first:		// �_�̕t���P
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion1",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	case adhesionType::second:		// �_�̕t���Q
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion2",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	case adhesionType::third:		// �_�̕t���R
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion1",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	case adhesionType::fourth:		// �_�̕t���S
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion1",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	case adhesionType::fifth:		// �_�̕t���T
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion1",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	case adhesionType::sixth:		// �_�̕t���U
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion1",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	default:
		break;
	}
}
