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
const float Camera::DEFAULT_CAMERA_DISTANCE = 15.0f;

const float Camera::DEFAULT_CAMERA_SPEED = 0.05f;

// スクロール値の上限と下限を定義
const int Camera::MAX_SCROLL_VALUE = 1440;
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
	m_target{},					// 注視点
	m_view{},					// ビュー行列
	m_proj{},					// プロジェクション行列
	is_moveMode{false},			// カメラを動かすフラグ
	is_eagleMode{false},		// カメラの視点移動フラグマウスホイールを使用する
	is_prevEagleMode{false},	// カメラの視点移動フラグマウスホイールを使用する(前回の保存用)
	m_screenHeight{},			// 画面の高さ
	m_screenWidth{}				// 画面の幅
{
	// マウスの回転量をリセット
	DirectX::Mouse::Get().ResetScrollWheelValue();
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
	auto state = DirectX::Mouse::Get().GetState();

	// マウスの左クリック＆ドラッグでカメラ座標を更新する
	if (state.leftButton)
	{
		// マウスのドラッグによるカメラ移動
		DraggedDistance(state.x, state.y);
	}

	// マウスの座標を前回の値として保存
	m_prevMouse.x = state.x; // X座標を保存
	m_prevMouse.y = state.y; // Y座標を保存

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
		float angleX = dy * DirectX::XM_PI / 180.0f;
		float angleY = dx * DirectX::XM_PI / 180.0f;

		// 角度の更新
		m_angle.x += angleX;
		m_angle.y += angleY;
	}
}

//-------------------------------------------------------------------
// ビュー行列の算出と、カメラ座標・注視点の取得
//-------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	// ビュー行列を算出する
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_angle.y);
	DirectX::SimpleMath::Matrix rotX = DirectX::SimpleMath::Matrix::CreateRotationX(m_angle.x);

	// 回転量を計算
	DirectX::SimpleMath::Matrix rt = rotY * rotX;

	// ポジション
	DirectX::SimpleMath::Vector3    eye(0.0f, 0.1f, 1.0f);

	// カメラの傾き（目線の角度）:0.1.0で正位置
	DirectX::SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	DirectX::SimpleMath::Vector3 target(m_target);

	eye = DirectX::SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - static_cast<float>(m_scrollWheelValue / 100));
	up  = DirectX::SimpleMath::Vector3::Transform(up,  rt.Invert());

	// デフォルトの初期位置
	m_eye = eye;
	m_target = target;
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);


	// 移動モードじゃなければ処理しない----------------------------------------------------
	if (!is_moveMode)return;

	// キーボードの取得
	auto keyState = DirectX::Keyboard::Get().GetState();

	// 移動モード起動中ならカメラ移動
	MoveCamera(eye, up, keyState);
}

//--------------------------------------------------------//
//カメラを動かす                                          //
//--------------------------------------------------------//
// 第１引数：カメラのレンズの位置 / 第２引数：カメラの傾き / 第３引数：キーボード
void Camera::MoveCamera(DirectX::SimpleMath::Vector3 eye,DirectX::SimpleMath::Vector3 up,DirectX::Keyboard::State keyState)
{
	// カメラの方向ベクトルから正面方向ベクトルを取得する
	DirectX::SimpleMath::Vector3 front = DirectX::SimpleMath::Vector3(-eye.x, 0.0f, -eye.z);
	DirectX::SimpleMath::Vector3 updo = DirectX::SimpleMath::Vector3(0.0f, -eye.y, 0.0f);
	
	// 方向ベクトルを正規化
	front.Normalize();
	updo.Normalize();

	// 正規化ベクトルを小さくして移動ベクトルに変換
	DirectX::SimpleMath::Vector3 moveW = front * DEFAULT_CAMERA_SPEED;
	DirectX::SimpleMath::Vector3 moveH = updo * DEFAULT_CAMERA_SPEED;

	// 前後移動
	if (keyState.W)
	{
		m_target += moveW;
		m_eyePos += moveW;
	}
	if (keyState.S)
	{
		m_target -= moveW;
		m_eyePos -= moveW;
	}

	// 左右移動
	if (keyState.A)
	{
		DirectX::SimpleMath::Vector3 mX = DirectX::SimpleMath::Vector3::Zero;
		mX.x = moveW.z;
		mX.z = -moveW.x;
		m_target += mX;
		m_eyePos += mX;
	}
	if (keyState.D)
	{
		DirectX::SimpleMath::Vector3 mX = DirectX::SimpleMath::Vector3::Zero;
		mX.x = moveW.z;
		mX.z = -moveW.x;
		m_target -= mX;
		m_eyePos -= mX;
	}

	// 上下移動
	if (keyState.Up)
	{
		DirectX::SimpleMath::Vector3 mY = DirectX::SimpleMath::Vector3::Zero;
		mY.y += moveH.y;
		m_target -= mY;
		m_eyePos -= mY;
	}
	if (keyState.Down)
	{
		DirectX::SimpleMath::Vector3 mY = DirectX::SimpleMath::Vector3::Zero;
		mY.y += moveH.y;
		m_target += mY;
		m_eyePos += mY;
	}

	m_eye = eye + m_eyePos;

	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, up);
}

//--------------------------------------------------------//
//カメラを揺らす関数                                      //
//--------------------------------------------------------//
// 第１引数：持続時間(60FPS = 1sec) / 第２引数：揺れ幅 / 第３引数：対象のポジション
void Camera::ShakeCamera(float duration, float tremor, DirectX::SimpleMath::Vector3* pos)
{
	int counta = 0;
	counta++;

	float d = (rand() % 10) * 0.01f * tremor;

	DirectX::SimpleMath::Vector3 sav = (*pos);

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
const DirectX::SimpleMath::Matrix& Camera::GetProjection(float width, float height,float angle)
{
	// 画面サイズとアングルの保存
	m_screenWidth = width;
	m_screenHeight = height;

	// 画角
	float fieldOfView = DirectX::XMConvertToRadians(angle);
	
	// 画面縦横比
	float aspectRatio = width / height;
	
	// カメラから一番近い投影面
	float nearPlane = 1.0f;

	// カメラから一番遠い投影面
	float farPlane = 100.0f;
	
	// カメラのレンズの作成
	DirectX::SimpleMath::Matrix projection = 
	DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
			fieldOfView, 
			aspectRatio, 
			nearPlane, 
			farPlane
	);

	// カメラ内で使う変数
	m_proj = projection;

	// プロジェクション行列を返却
	return m_proj;
}