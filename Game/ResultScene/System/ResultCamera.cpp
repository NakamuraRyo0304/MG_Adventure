/*
 *	@File	ResultCamera.cpp
 *	@Brief	リザルトシーンのカメラクラス。
 *	@Date	2023-11-12
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "ResultCamera.h"

// コンストラクタ
ResultCamera::ResultCamera(const SimpleMath::Vector2& screenSize)
	: IGameCamera(screenSize)
	, m_timer{}
{
	SetPosition(SimpleMath::Vector3(0.0f, 20.0f, 10.0f));
	SetTarget(SimpleMath::Vector3::Zero);
	SetInitialPosition(GetPosition());
}

// デストラクタ
ResultCamera::~ResultCamera()
{
}

// 更新処理
void ResultCamera::Update()
{
	m_timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	SetPosition(SimpleMath::Vector3(
		cosf(m_timer),
		GetInitialPosition().y + sinf(m_timer) * 2.0f,
		GetInitialPosition().z
	));

	SetTarget(SimpleMath::Vector3(0.0f, -10.0f, -5.0f));

	SetView(SimpleMath::Matrix::CreateLookAt(GetPosition(), GetTarget(), GetUp()));
}
