/*
 *	@File	IGameCamera.cpp
 *	@Brief	カメラの基礎部分。
 *	@Date	2023-11-12
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "IGameCamera.h"

// コンストラクタ
IGameCamera::IGameCamera(const SimpleMath::Vector2& screenSize)
	: m_view{}						// ビュー行列
	, m_projection{}				// プロジェクション行列
	, m_position{}					// カメラ座標
	, m_target{}					// 注視点
	, m_up{}						// 上の向き
	, m_initialPosition{}			// 初期位置を設定
	, m_screenSize{ screenSize }	// スクリーンサイズ
{
	m_position = SimpleMath::Vector3::Zero;
	m_target = SimpleMath::Vector3::Zero;
	m_up = SimpleMath::Vector3::Up;
	m_initialPosition = SimpleMath::Vector3::Zero;
	CreateProjection();
}

// 射影行列を作成
void IGameCamera::CreateProjection()
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
