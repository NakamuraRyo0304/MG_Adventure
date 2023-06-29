/*
 *	@File	Camera.cpp
 *	@Brief	カメラの設定。
 *	@Date	2023-04-28
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../UserUtility.h"

#include "Camera.h"

 // 定数の初期化
const float Camera::DEFAULT_CAMERA_DISTANCE = 20.0f;

const float Camera::DEFAULT_CAMERA_SPEED = 0.05f;

// スクロール値の上限と下限を定義
const int Camera::MAX_SCROLL_VALUE = 1640;
const int Camera::MIN_SCROLL_VALUE = -1440;

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
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

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
Camera::~Camera()
{
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
void Camera::Update()
{
	// マウスのインスタンス取得
	auto state = Mouse::Get().GetState();

	// マウスの左クリック＆ドラッグでカメラ座標を更新する
	if (state.leftButton)
	{
		// マウスのドラッグによるカメラ移動
		DraggedDistance(state.x, state.y);
	}

	// マウスの座標を前回の値として保存
	m_prevMouse.x = static_cast<float>(state.x); // X座標を保存
	m_prevMouse.y = static_cast<float>(state.y); // Y座標を保存

	// 以下、カメラの拡大率の計算

	// マウスホイールのスクロール値の差分を計算
	int scrollDelta = state.scrollWheelValue - m_prevScrollWheelValue;

	// カメラモードの時の処理
	if (is_eagleMode)
	{
		// スクロール値を一時変数に保存
		int newScrollValue = m_scrollWheelValue + scrollDelta;

		// スクロール値が上限・下限を超えないように制限
		newScrollValue = UserUtillity::Clamp(newScrollValue, MIN_SCROLL_VALUE, MAX_SCROLL_VALUE);

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

	// ビュー行列の算出
	CalculateViewMatrix();

	// 前回のフラグを更新
	is_prevEagleMode = is_eagleMode;

	// マウスホイールの前回の値を更新(一連の作業が終わってから更新する)
	m_prevScrollWheelValue = state.scrollWheelValue;

}

//--------------------------------------------------------//
//マウスの移動距離の計算                                  //
//--------------------------------------------------------//
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
	m_angle.x = UserUtillity::Clamp(m_angle.x, ANGLEX_MIN, ANGLEX_MAX);

}

//-------------------------------------------------------------------
// ビュー行列の算出と、カメラ座標・注視点の取得
//-------------------------------------------------------------------
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
	m_view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

}

//--------------------------------------------------------//
//カメラを揺らす関数                                      //
//--------------------------------------------------------//
// 第１引数：持続時間(60FPS = 1sec) / 第２引数：揺れ幅 / 第３引数：対象のポジション
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

//--------------------------------------------------------//
//射影行列の作成と取得                                    //
//--------------------------------------------------------//
// 第１引数：画面横幅 / 第２引数：画面縦幅 / 第３引数：カメラ画角(float値を変換なしで渡す)
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

//--------------------------------------------------------//
//カメラとオブジェクトの距離を求める                      //
//--------------------------------------------------------//
float Camera::CalculateDistanceToObject(const SimpleMath::Vector3& objPos)
{
	SimpleMath::Vector3 distanceVector = objPos - m_eye;
	float distance = distanceVector.Length();

	return distance;
}