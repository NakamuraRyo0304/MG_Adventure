/*
 *	@File	ThurdPersonCamera.cpp
 *	@Brief	三人称視点のカメラ。
 *	@Date	2023-08-05
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Libraries/SystemManager/SystemManager.h"

#include "ThirdPersonCamera.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="system">システムマネージャ</param>
/// <param name="context">コンテキストポインタ</param>
/// <param name="device">デバイスポインタ</param>
/// <returns>なし</returns>
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
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
ThirdPersonCamera::~ThirdPersonCamera()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="pos">基準点</param>
/// <param name="rotate">回転量</param>
/// <param name="distance">基準点からの距離</param>
/// <returns>なし</returns>
void ThirdPersonCamera::UpdateFollow(const SimpleMath::Vector3& pos, const SimpleMath::Quaternion& rotate,
	const SimpleMath::Vector3& distance)
{
	// カメラの目線の方向をプレイヤーの回転に合わせて計算
	SimpleMath::Vector3 forward =
		SimpleMath::Vector3::Transform(SimpleMath::Vector3::UnitZ, rotate);

	SimpleMath::Vector3 newPos = { pos.x,pos.y - 0.5f,pos.z };

	// カメラの位置と目線を計算
	SimpleMath::Vector3 eye =
		newPos + SimpleMath::Vector3::Transform(distance, rotate);
	SimpleMath::Vector3 target = eye - forward;

	// ビュー行列
	m_followView = SimpleMath::Matrix::CreateLookAt(eye, target, SimpleMath::Vector3::Up);

	//-------------------------------------------------------------------------------------//
	// 画像用変数更新
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
/// 雲の付着を表現
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ThirdPersonCamera::DrawAdhesion()
{
	// 画面サイズの比率
	SimpleMath::Vector2 scale = { m_system->GetDeviceResources()->GetOutputSize().right  / 1920.0f ,
								  m_system->GetDeviceResources()->GetOutputSize().bottom / 1080.0f };

	switch (m_switching)
	{
	case adhesionType::first:		// 雲の付着１
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion1",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	case adhesionType::second:		// 雲の付着２
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion2",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	case adhesionType::third:		// 雲の付着３
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion1",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	case adhesionType::fourth:		// 雲の付着４
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion1",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	case adhesionType::fifth:		// 雲の付着５
		m_system->GetDrawSprite()->DrawTexture(
			L"Adhesion1",
			SimpleMath::Vector2::Zero,
			{ 1.0f, 1.0f, 1.0f, 1.0f - (CHANGE_SPAN - m_adhesionTimer) / CHANGE_SPAN },
			scale,
			SimpleMath::Vector2::Zero
		);
		break;
	case adhesionType::sixth:		// 雲の付着６
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
