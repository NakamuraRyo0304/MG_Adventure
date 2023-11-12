/*
 *	@File	TitleCamera.h
 *	@Brief	タイトルシーンのカメラクラス。
 *	@Date	2023-11-11
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "TitleCamera.h"

// コンストラクタ
TitleCamera::TitleCamera(const SimpleMath::Vector2& screenSize)
	: IGameCamera(screenSize)
	, m_logoPosition{}
{
	SetPosition(SimpleMath::Vector3(0.0f, 0.0f, 8.0f));
	SetTarget(SimpleMath::Vector3::Zero);
	SetInitialPosition(GetPosition());
}

// デストラクタ
TitleCamera::~TitleCamera()
{
}

// 更新処理
void TitleCamera::Update()
{
	SetPosition(SimpleMath::Vector3(0.0f, m_logoPosition.y, 8.0f));
	SetView(SimpleMath::Matrix::CreateLookAt(GetPosition(), GetTarget(), GetUp()));
}
