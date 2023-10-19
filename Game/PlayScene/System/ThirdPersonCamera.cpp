/*
 *	@File	ThurdPersonCamera.cpp
 *	@Brief	三人称視点のカメラ。
 *	@Date	2023-08-05
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Libraries/SystemManager/SystemManager.h"

// 靄
#include "../Objects/Haze.h"

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
	: m_adhesionTimer{ CHANGE_SPAN }
	, is_changeFlag{false}
{
	m_system = system;

	m_system->GetDrawSprite()->MakeSpriteBatch(context);

	m_system->GetDrawSprite()->AddTextureData(L"Adhesion", L"Resources/Textures/ADHESION/Adhesion.dds", device);

	// 靄の作成
	m_haze = std::make_unique<Haze>();
	m_haze->CreateShader(m_system->GetDeviceResources()->GetInstance());

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
/// <param name="timer">タイマー</param>
/// <returns>なし</returns>
void ThirdPersonCamera::UpdateFollow(const SimpleMath::Vector3& pos, const SimpleMath::Quaternion& rotate,
	const SimpleMath::Vector3& distance, const float& timer)
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
	m_adhesionTimer = timer;
}

/// <summary>
/// 雲の付着を表現
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void ThirdPersonCamera::DrawAdhesion()
{
	// 画面サイズの比率
	SimpleMath::Vector2 scale = { m_system->GetDeviceResources()->GetOutputSize().right  / FULL_SCREEN_SIZE.x ,
								  m_system->GetDeviceResources()->GetOutputSize().bottom / FULL_SCREEN_SIZE.y };

	m_system->GetDrawSprite()->DrawTexture(
		L"Adhesion",
		SimpleMath::Vector2::Zero,
		{ 1.0f, 1.0f, 1.0f, 0.5f },
		scale,
		SimpleMath::Vector2::Zero
	);

}
