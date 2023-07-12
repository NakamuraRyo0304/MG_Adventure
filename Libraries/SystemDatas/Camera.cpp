/*
 *	@File	Camera.cpp
 *	@Brief	カメラの設定。
 *	@Date	2023-04-28
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ユーザーユーティリティ
#include "../UserUtility.h"

#include "Camera.h"

 // 定数の初期化
const float Camera::DEFAULT_CAMERA_DISTANCE = 20.0f;

const float Camera::DEFAULT_CAMERA_SPEED = 0.05f;

// スクロール値の上限と下限を定義
const int Camera::MAX_SCROLL_VALUE = 1640;
const int Camera::MIN_SCROLL_VALUE = -1440;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
Camera::Camera():
	m_angle{},					// 回転角
	m_eye{},					// カメラの座標
	m_eyePos{},					// カメラの移動座標
	m_prevMouse{},				// 過去のマウスの位置
	m_scrollWheelValue{},		// マウスホイールの回転量
	m_tempScrollValue{},		// マウスホイールの回転量(不動時の蓄積用)
	m_prevScrollWheelValue{},	// マウスホイールの回転量(前回の保存用)
	m_view{},					// ビュー行列
	m_proj{},					// プロジェクション行列
	is_eagleMode{false},		// カメラの視点移動フラグマウスホイールを使用する
	is_prevEagleMode{false},	// カメラの視点移動フラグマウスホイールを使用する(前回の保存用)
	m_screenHeight{},			// 画面の高さ
	m_screenWidth{},			// 画面の幅
	m_rot{}						// 回転量
{
	// マウスの回転量をリセット
	Mouse::Get().ResetScrollWheelValue();
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
Camera::~Camera()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Camera::Update()
{
	// キーボードのインスタンス取得
	auto kbState = Keyboard::Get().GetState();
	// マウスのインスタンス取得
	auto msState = Mouse::Get().GetState();

	// マウスの左クリック＆ドラッグでカメラ座標を更新する
	if (msState.leftButton)
	{
		// マウスのドラッグによるカメラ移動
		DraggedDistance(msState.x, msState.y);
	}

	// マウスの座標を前回の値として保存
	m_prevMouse.x = static_cast<float>(msState.x); // X座標を保存
	m_prevMouse.y = static_cast<float>(msState.y); // Y座標を保存

	// 拡縮処理
	RollWheelToRate(msState);

	// ビュー行列の算出
	CalculateViewMatrix();

	// 上下回転のみ制限
	m_angle.x = UserUtility::Clamp(m_angle.x, ANGLEX_MIN, ANGLEX_MAX);

	// 移動量
	SimpleMath::Vector2 moveVal = SimpleMath::Vector2::Zero;

	// 移動量を設定
	if (kbState.Right){ moveVal.y =  0.01f; }
	if (kbState.Left) { moveVal.y = -0.01f; }
	if (kbState.Up)   { moveVal.x =  0.01f; }
	if (kbState.Down) { moveVal.x = -0.01f; }

	// 角度変更
	m_angle += moveVal;
}

/// <summary>
/// マウスのドラッグした距離を計算
/// </summary>
/// <param name="x">スクリーン座標X</param>
/// <param name="y">スクリーン座標Y</param>
/// <returns>なし</returns>
void Camera::DraggedDistance(int x, int y)
{
	// 視点移動しなければ処理しない
	if (!is_eagleMode)	return;

	// マウスポインタの前回からの変位
	// DEFAULT_CAMERA_SPEEDを乗算してドラッグの移動量を調整する
	float dx = static_cast<float>(x - m_prevMouse.x) * DEFAULT_CAMERA_SPEED;
	float dy = static_cast<float>(y - m_prevMouse.y) * DEFAULT_CAMERA_SPEED;

	if (dx != 0.0f || dy != 0.0f)
	{
		// マウスポインタの変位を元に、Ｘ軸Ｙ軸の回転角を求める
		float angleX = dy * XM_PI / 180.0f;
		float angleY = dx * XM_PI / 180.0f;

		// 角度の更新
		m_angle.x += angleX;
		m_angle.y += angleY;
	}

	// カメラの角度をクランプ
	m_angle.x = UserUtility::Clamp(m_angle.x, ANGLEX_MIN, ANGLEX_MAX);

}

/// <summary>
/// ビュー行列計算
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Camera::CalculateViewMatrix()
{
	// ビュー行列を算出する
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_angle.y);
	SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(m_angle.x);

	// 回転量を計算
	SimpleMath::Matrix rt = rotY * rotX;
	m_rot = rt;

	// ポジション
	SimpleMath::Vector3    eye(0.0f, 0.1f, 1.0f);

	// カメラの傾き（目線の角度)
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);

	eye = SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - static_cast<float>(m_scrollWheelValue / 100));
	up  = SimpleMath::Vector3::Transform(up,  rt.Invert());

	// デフォルトの初期位置
	m_eye = eye;
	m_target = target;
	m_view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

}

/// <summary>
/// カメラの振動処理
/// </summary>
/// <param name="duration">揺れ継続時間</param>
/// <param name="tremor">揺れ幅</param>
/// <param name="pos">対象オブジェのポジション</param>
/// <returns>なし</returns>
void Camera::ShakeCamera(float duration, float tremor, SimpleMath::Vector3* pos)
{
	int counta = 0;
	counta++;

	float d = (rand() % 10) * 0.01f * tremor;

	SimpleMath::Vector3 sav = (*pos);

	if (counta > duration)
	{
		(*pos) = sav;
	}
	else
	{
		(*pos).x += d * (rand() % 3 - 1);
		(*pos).y += d * (rand() % 3 - 1);
		(*pos).z += d * (rand() % 3 - 1);
	}
}

/// <summary>
/// 射影行列を作成
/// </summary>
/// <param name="width">画面横幅</param>
/// <param name="height">画面縦幅</param>
/// <param name="angle">カメラ画角</param>
/// <returns>射影行列</returns>
const SimpleMath::Matrix& Camera::CreateProjection(float width, float height,float angle)
{
	// 画面サイズとアングルの保存
	m_screenWidth = static_cast<int>(width);
	m_screenHeight = static_cast<int>(height);

	// 画角
	float fieldOfView = XMConvertToRadians(angle);
	
	// 画面縦横比
	float aspectRatio = width / height;
	
	// カメラから一番近い投影面
	float nearPlane = NEAR_PLANE;

	// カメラから一番遠い投影面
	float farPlane = FAR_PLANE;
	
	// カメラのレンズの作成
	SimpleMath::Matrix projection = 
	SimpleMath::Matrix::CreatePerspectiveFieldOfView(
			fieldOfView, 
			aspectRatio, 
			nearPlane, 
			farPlane
	);

	m_angle.x = angle;

	// カメラ内で使う変数
	m_proj = projection;

	// プロジェクション行列を返却
	return m_proj;
}

/// <summary>
/// カメラからの距離を計算
/// </summary>
/// <param name="objPos">対象オブジェのポジション</param>
/// <returns>カメラから対象物までの距離</returns>
float Camera::CalculateDistanceToObject(const SimpleMath::Vector3& objPos)
{
	SimpleMath::Vector3 distanceVector = objPos - m_eye;
	float distance = distanceVector.Length();

	return distance;
}

/// <summary>
/// カメラの拡大率の変更
/// </summary>
/// <param name="state">マウスのステート</param>
/// <returns>なし</returns>
void Camera::RollWheelToRate(Mouse::State state)
{
	// マウスホイールのスクロール値の差分を計算
	int scrollDelta = state.scrollWheelValue - m_prevScrollWheelValue;

	// カメラモードの時の処理
	if (is_eagleMode)
	{
		// スクロール値を一時変数に保存
		int newScrollValue = m_scrollWheelValue + scrollDelta;

		// スクロール値が上限・下限を超えないように制限
		newScrollValue = UserUtility::Clamp(newScrollValue, MIN_SCROLL_VALUE, MAX_SCROLL_VALUE);

		// スクロール値が上限・下限に達していない場合にのみ反映する
		if (newScrollValue != MAX_SCROLL_VALUE && newScrollValue != MIN_SCROLL_VALUE)
		{
			m_scrollWheelValue = newScrollValue;
		}
		else
		{
			// 上限・下限に達した場合はスクロール値をそのままにする
			scrollDelta = 0;
		}

		// マウスホイールの前回のTrueの値を保持
		m_tempScrollValue = m_scrollWheelValue;

		// スクロールがクランプされている間に回された分を戻す
		if (m_scrollWheelValue == MAX_SCROLL_VALUE && scrollDelta > 0)
		{
			m_scrollWheelValue -= scrollDelta;
		}
		else if (m_scrollWheelValue == MIN_SCROLL_VALUE && scrollDelta < 0)
		{
			m_scrollWheelValue -= scrollDelta;
		}
	}
	// 非カメラモードの時の処理
	else
	{
		// フラグがFalseの場合は前回のTrueの値を代入
		m_scrollWheelValue = m_tempScrollValue;
	}

	// 前回のフラグを更新
	is_prevEagleMode = is_eagleMode;

	// マウスホイールの前回の値を更新(一連の作業が終わってから更新する)
	m_prevScrollWheelValue = state.scrollWheelValue;
}
