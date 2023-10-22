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
	: m_system{ system }
	, m_adhesionTimer{ CHANGE_SPAN }
	, m_followView{}
	, is_changeFlag{false}
{
	// �h���[�X�v���C�g
	auto& _sp = m_system->GetDrawSprite();

	_sp->MakeSpriteBatch(context);

	_sp->AddTextureData(L"Adhesion", L"Resources/Textures/ADHESION/Adhesion.dds", device);
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
/// <param name="timer">�^�C�}�[</param>
/// <returns>�Ȃ�</returns>
void ThirdPersonCamera::UpdateFollow(const SimpleMath::Vector3& pos, const SimpleMath::Quaternion& rotate,
	const SimpleMath::Vector3& distance, const float& timer)
{
	// �J�����̖ڐ��̕������v���C���[�̉�]�ɍ��킹�Čv�Z
	SimpleMath::Vector3 _forward =
		SimpleMath::Vector3::Transform(SimpleMath::Vector3::UnitZ, rotate);

	SimpleMath::Vector3 _newPos = { pos.x,pos.y - 0.5f,pos.z };

	// �J�����̈ʒu�Ɩڐ����v�Z
	SimpleMath::Vector3 _eye =
		_newPos + SimpleMath::Vector3::Transform(distance, rotate);
	SimpleMath::Vector3 _target = _eye - _forward;

	// �r���[�s��
	m_followView = SimpleMath::Matrix::CreateLookAt(_eye, _target, SimpleMath::Vector3::Up);

	//-------------------------------------------------------------------------------------//
	// �摜�p�ϐ��X�V
	m_adhesionTimer = timer;
}

/// <summary>
/// �_�̕t����\��
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void ThirdPersonCamera::DrawAdhesion()
{
	// �f�o�C�X���\�[�X
	auto _pDR = m_system->GetDeviceResources();

	// ��ʃT�C�Y�̔䗦
	SimpleMath::Vector2 _scale = { _pDR->GetOutputSize().right  / FULL_SCREEN_SIZE.x ,
								   _pDR->GetOutputSize().bottom / FULL_SCREEN_SIZE.y };

	m_system->GetDrawSprite()->DrawTexture(
		L"Adhesion",
		SimpleMath::Vector2::Zero,
		{ 1.0f, 1.0f, 1.0f, 0.5f },
		_scale,
		SimpleMath::Vector2::Zero
	);

}
