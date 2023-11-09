/*
 *	@File	PlayCamera.cpp
 *	@Brief	最初のカメラ演出。
 *	@Date	2023-10-03
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "PlayCamera.h"

// コンストラクタ
PlayCamera::PlayCamera(const SimpleMath::Vector2& screenSize)
	: m_view{}						// ビュー行列
	, m_projection{}				// プロジェクション行列
	, m_position{}					// カメラ座標
	, m_target{}					// 注視点
	, m_screenSize{ screenSize }	// スクリーンサイズ
{
	CreateProjection();
}

// デストラクタ
PlayCamera::~PlayCamera()
{
}

// ビュー行列を作成
const SimpleMath::Matrix& PlayCamera::CreateView()
{
	m_view = SimpleMath::Matrix::CreateLookAt(
		m_position,
		m_target,
		SimpleMath::Vector3::UnitY
	);

	return m_view;
}

// 射影行列を作成
void PlayCamera::CreateProjection()
{
	// 画角
	float _fieldOfView = XMConvertToRadians(ANGLE);

	// 画面縦横比
	float _aspectRatio = m_screenSize.x / m_screenSize.y;

	// カメラのレンズの作成
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		_fieldOfView,		// カメラの画角
		_aspectRatio,		// アスペクト比
		NEAR_PLANE,			// 最近距離
		FAR_PLANE			// 最遠距離
	);
}
