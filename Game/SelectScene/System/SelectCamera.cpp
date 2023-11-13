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
	: IGameCamera(screenSize)		// 共通部分を初期化
	, is_canChangeFlag{ false }		// 画面切り替え可能かを判定
{
	SetPosition(SimpleMath::Vector3(0.0f, POS_Y, 0.0f));
	SetTarget(SimpleMath::Vector3(LOOK_VALUE, 0.0f, 0.0f));
	SetInitialPosition(GetPosition());
}

// デストラクタ
SelectCamera::~SelectCamera()
{
}

// 更新処理
void SelectCamera::Update()
{
	// 切り替え可能ならTrueにする
	is_canChangeFlag = (GetTarget().x >= LOOK_VALUE * LOOK_SPAN);

	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// フォントまで移動する
	SetTarget(SimpleMath::Vector3(
		UserUtility::EaseInOutLerp(GetTarget().x, 0.0f, APPROACH_SPEED),
		GetTarget().y,
		GetTarget().z)
	);

	// 回転量を計算
	float _rotValueX = CAMERA_RADIUS * sinf(_timer * 0.5f);
	float _rotValueZ = CAMERA_RADIUS * cosf(_timer * 0.5f);
	// 上下移動量を計算(01クランプ×動きの大きさ)
	float _transValueY = (sinf(_timer) + 1.0f) * VERTICAL_MOTION;

	// ビュー行列
	SetPosition(SimpleMath::Vector3(_rotValueX, _transValueY + GetInitialPosition().y, _rotValueZ));
	SetView(SimpleMath::Matrix::CreateLookAt(GetPosition(), GetTarget(), GetUp()));
}

// ターゲットの更新
void SelectCamera::MoveTarget()
{
	SetTarget(SimpleMath::Vector3(
		UserUtility::Lerp(GetTarget().x, LOOK_VALUE, LOOK_SPEED),
		GetTarget().y, GetTarget().z));
}
