/*
 *	@File	SelectCamera.cpp
 *	@Brief	セレクトシーンのカメラクラス。
 *	@Date	2023-11-11
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/UserUtility.h"
#include "SelectCamera.h"

// コンストラクタ
SelectCamera::SelectCamera(const SimpleMath::Vector2& screenSize)
	: m_view{}						// ビュー行列
	, m_projection{}				// プロジェクション行列
	, m_position{}					// カメラ座標
	, m_target{}					// 注視点
	, m_screenSize{ screenSize }	// スクリーンサイズ
	, is_canChangeFlag{ false }		// 画面切り替え可能かを判定
{
	CreateProjection();

	m_position.y = POS_Y;
	m_target.x = UP_VALUE;
}

// デストラクタ
SelectCamera::~SelectCamera()
{
}

// 更新処理
void SelectCamera::Update()
{
	// 切り替え可能ならTrueにする
	is_canChangeFlag = (m_target.x >= UP_VALUE * UP_SPAN);

	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// フォントまで移動する
	m_target.x = UserUtility::Lerp(m_target.x, 0.0f, APPROACH_SPEED);

	// 回転量を計算
	float _rotValueX = CAMERA_RADIUS * sinf(_timer * 0.5f);
	float _rotValueZ = CAMERA_RADIUS * cosf(_timer * 0.5f);
	// 上下移動量を計算(01クランプ×動きの大きさ)
	float _transValueY = m_position.y + (sinf(_timer) + 1.0f) * VERTICAL_MOTION;

	// ビュー行列
	SimpleMath::Vector3 _eye(_rotValueX, _transValueY, _rotValueZ);
	m_view = SimpleMath::Matrix::CreateLookAt(_eye, m_target, SimpleMath::Vector3::Up);
}

// ターゲットの更新
void SelectCamera::MoveTarget()
{
	m_target.x = UserUtility::Lerp(m_target.x, UP_VALUE, UP_SPEED);
}

// 射影行列を作成
void SelectCamera::CreateProjection()
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
