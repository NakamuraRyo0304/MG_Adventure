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
Camera::Camera()
	: m_screenHeight{}			// 画面の高さ
	, m_screenWidth{}			// 画面の幅
	, m_angle{}					// 回転角
	, m_position{}				// カメラの座標
	, m_addPos{}				// カメラ加算用の座標
	, m_prevMouse{}				// 過去のマウスの位置
	, m_scrollWheelValue{}		// マウスホイールの回転量
	, m_tempScrollValue{}		// マウスホイールの回転量(不動時の蓄積用)
	, m_prevScrollWheelValue{}	// マウスホイールの回転量(前回の保存用)
	, m_view{}					// ビュー行列
	, m_projection{}			// プロジェクション行列
	, m_rotateMatrix{}			// 回転量
	, is_allowMode{ false }		// カメラの視点移動フラグ(十字操作)
	, is_eagleMode{ false }		// カメラの視点移動フラグ(マウス操作)
	, is_prevEagleMode{ false }	// カメラの視点移動フラグ(前回の保存用)
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
	auto _kbState = Keyboard::Get().GetState();
	// マウスのインスタンス取得
	auto _msState = Mouse::Get().GetState();

	// マウスの左クリック＆ドラッグでカメラ座標を更新する
	if (_msState.leftButton)
	{
		// 視点移動しなければ処理しない
		if (!is_eagleMode)	return;

		// マウスのドラッグによるカメラ移動
		DraggedDistance(_msState.x, _msState.y);
	}

	// マウスの座標を前回の値として保存
	m_prevMouse.x = static_cast<float>(_msState.x); // X座標を保存
	m_prevMouse.y = static_cast<float>(_msState.y); // Y座標を保存

	// 拡縮処理
	RollWheelToRate(_msState);

	// ビュー行列の算出
	CalculateViewMatrix();

	// 十字スイッチがオフなら動かさない
	if (!is_allowMode) return;

	// 移動量
	SimpleMath::Vector2 _moveVal = SimpleMath::Vector2::Zero;

	// 移動量を設定
	if (_kbState.Right) { _moveVal.y =  ALLOW_SPEED; }
	if (_kbState.Left)  { _moveVal.y = -ALLOW_SPEED; }
	if (_kbState.Up)    { _moveVal.x =  ALLOW_SPEED; }
	if (_kbState.Down)  { _moveVal.x = -ALLOW_SPEED; }

	// 角度変更
	m_angle += _moveVal;
}

/// <summary>
/// マウスのドラッグした距離を計算
/// </summary>
/// <param name="x">スクリーン座標X</param>
/// <param name="y">スクリーン座標Y</param>
/// <returns>なし</returns>
void Camera::DraggedDistance(int x, int y)
{
	// マウスポインタの前回からの変位
	// DEFAULT_CAMERA_SPEEDを乗算してドラッグの移動量を調整する
	float _dx = static_cast<float>(x - m_prevMouse.x) * DEFAULT_CAMERA_SPEED;
	float _dy = static_cast<float>(y - m_prevMouse.y) * DEFAULT_CAMERA_SPEED;

	if (_dx != 0.0f || _dy != 0.0f)
	{
		// マウスポインタの変位を元に、Ｘ軸Ｙ軸の回転角を求める
		float _angleX = _dy * XM_PI / 180.0f;
		float _angleY = _dx * XM_PI / 180.0f;

		// 角度の更新
		m_angle.x += _angleX;
		m_angle.y += _angleY;
	}

	// カメラの角度をクランプ
	m_angle.x = UserUtility::Clamp(m_angle.x, ANGLE_X_MIN, ANGLE_X_MAX);

}

/// <summary>
/// オブジェクトの振動処理
/// </summary>
/// <param name="duration">揺れ継続時間</param>
/// <param name="tremor">揺れ幅</param>
/// <param name="pos">対象オブジェのポジション</param>
/// <returns>なし</returns>
void Camera::ShakeObject(float duration, float tremor, SimpleMath::Vector3* pos)
{
	int _counta = 0;
	_counta++;

	float _d = (rand() % 10) * 0.01f * tremor;

	SimpleMath::Vector3 _sav = (*pos);

	if (_counta > duration)
	{
		(*pos) = _sav;
	}
	else
	{
		(*pos).x += _d * (rand() % 3 - 1);
		(*pos).y += _d * (rand() % 3 - 1);
		(*pos).z += _d * (rand() % 3 - 1);
	}
}


/// <summary>
/// 射影行列を作成
/// </summary>
/// <param name="width">画面横幅</param>
/// <param name="height">画面縦幅</param>
/// <param name="angle">カメラ画角</param>
/// <returns>射影行列</returns>
const SimpleMath::Matrix& Camera::CreateProjection(float width, float height, float angle)
{
	// 画面サイズとアングルの保存
	m_screenWidth  = static_cast<int>(width);
	m_screenHeight = static_cast<int>(height);

	// 画角
	float _fieldOfView = XMConvertToRadians(angle);

	// 画面縦横比
	float _aspectRatio = width / height;

	// カメラから一番近い投影面
	float _nearPlane = NEAR_PLANE;

	// カメラから一番遠い投影面
	float _farPlane = FAR_PLANE;

	// カメラのレンズの作成
	SimpleMath::Matrix _projection =
		SimpleMath::Matrix::CreatePerspectiveFieldOfView(
			_fieldOfView,
			_aspectRatio,
			_nearPlane,
			_farPlane
		);

	// カメラ画角
	m_angle.x = angle;

	// プロジェクション行列を返却
	return m_projection = _projection;
}

/// <summary>
/// ビュー行列計算
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Camera::CalculateViewMatrix()
{
	// ビュー行列を算出する
	SimpleMath::Matrix _rotY = SimpleMath::Matrix::CreateRotationY(m_angle.y);
	SimpleMath::Matrix _rotX = SimpleMath::Matrix::CreateRotationX(m_angle.x);

	// 回転量を計算
	SimpleMath::Matrix _rt = _rotY * _rotX;
	m_rotateMatrix = _rt;

	// ポジション
	SimpleMath::Vector3    _eye(0.0f, 0.1f, 1.0f);

	// カメラの傾き（目線の角度)
	SimpleMath::Vector3     _up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 _target(0.0f, 0.0f, 0.0f);

	_eye = SimpleMath::Vector3::Transform(_eye, _rt.Invert());
	_eye *= (DEFAULT_CAMERA_DISTANCE - static_cast<float>(m_scrollWheelValue / 100));
	_up  = SimpleMath::Vector3::Transform(_up,  _rt.Invert());

	// デフォルトの初期位置
	m_position = _eye;
	m_target = _target;
	m_view = SimpleMath::Matrix::CreateLookAt(_eye + m_addPos, _target + m_addPos, _up);

	// 上下回転のみ制限
	m_angle.x = UserUtility::Clamp(m_angle.x, ANGLE_X_MIN, ANGLE_X_MAX);
}

/// <summary>
/// カメラの拡大率の変更
/// </summary>
/// <param name="state">マウスのステート</param>
/// <returns>なし</returns>
void Camera::RollWheelToRate(Mouse::State state)
{
	// マウスホイールのスクロール値の差分を計算
	int _scrollDelta = state.scrollWheelValue - m_prevScrollWheelValue;

	// カメラモードの時の処理
	if (is_eagleMode)
	{
		// スクロール値を一時変数に保存
		int _newScrollValue = m_scrollWheelValue + _scrollDelta;

		// スクロール値が上限・下限を超えないように制限
		_newScrollValue = UserUtility::Clamp(_newScrollValue, MIN_SCROLL_VALUE, MAX_SCROLL_VALUE);

		// スクロール値が上限・下限に達していない場合にのみ反映する
		if (_newScrollValue != MAX_SCROLL_VALUE && _newScrollValue != MIN_SCROLL_VALUE)
		{
			m_scrollWheelValue = _newScrollValue;
		}
		else
		{
			// 上限・下限に達した場合はスクロール値をそのままにする
			_scrollDelta = 0;
		}

		// マウスホイールの前回のTrueの値を保持
		m_tempScrollValue = m_scrollWheelValue;

		// スクロールがクランプされている間に回された分を戻す
		if (m_scrollWheelValue == MAX_SCROLL_VALUE && _scrollDelta > 0)
		{
			m_scrollWheelValue -= _scrollDelta;
		}
		else if (m_scrollWheelValue == MIN_SCROLL_VALUE && _scrollDelta < 0)
		{
			m_scrollWheelValue -= _scrollDelta;
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
