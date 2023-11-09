/*
 *	@File	ThurdPersonCamera.cpp
 *	@Brief	三人称視点のカメラ。
 *	@Date	2023-08-05
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemManager/SystemManager.h"
#include "ThirdPersonCamera.h"

// コンストラクタ
ThirdPersonCamera::ThirdPersonCamera(std::shared_ptr<SystemManager> system)
	: m_system{ system }
	, m_followView{}
{
	// ドロースプライト
	auto& _sp = m_system->GetDrawSprite();

	_sp->MakeSpriteBatch();

	_sp->AddTextureData(L"Adhesion", L"Resources/Textures/PLAY_COMMON/Adhesion.dds");
}

// デストラクタ
ThirdPersonCamera::~ThirdPersonCamera()
{
}

// 追従処理
void ThirdPersonCamera::UpdateFollow(const SimpleMath::Vector3& pos, const SimpleMath::Quaternion& rotate,
	const SimpleMath::Vector3& distance)
{
	// カメラの目線の方向をプレイヤーの回転に合わせて計算
	SimpleMath::Vector3 _forward =
		SimpleMath::Vector3::Transform(SimpleMath::Vector3::UnitZ, rotate);

	SimpleMath::Vector3 _newPos = { pos.x,pos.y - 0.5f,pos.z };

	// カメラの位置と目線を計算
	SimpleMath::Vector3 _eye =
		_newPos + SimpleMath::Vector3::Transform(distance, rotate);
	SimpleMath::Vector3 _target = _eye - _forward;

	// ビュー行列
	m_followView = SimpleMath::Matrix::CreateLookAt(_eye, _target, SimpleMath::Vector3::Up);
}

// 外界を制限
void ThirdPersonCamera::DrawAdhesion()
{
	// デバイスリソース
	auto _pDR = m_system->GetDeviceResources();

	// 画面サイズの比率
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
