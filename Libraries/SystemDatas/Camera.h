/*
 *	@File	Camera.h
 *	@Brief	カメラの設定。
 *	@Date	2023-04-28
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef CAMERA
#define CAMERA

class Camera
{
public:	//-------------------------------------------------------------------------------------//
	// スクリーンサイズ
	int m_screenWidth, m_screenHeight;

	// カメラの距離
	static const float DEFAULT_CAMERA_DISTANCE;

	// カメラの移動速度
	static const float DEFAULT_CAMERA_SPEED;

	// カメラホイール上下限
	static const int MAX_SCROLL_VALUE;
	static const int MIN_SCROLL_VALUE;

	// カメラ表示距離
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 245.0f;

	// カメラ画角上下限
	const float ANGLEX_MIN = 44.0f;
	const float ANGLEX_MAX = 45.5f;

private: //-------------------------------------------------------------------------------------//

	// 前回のマウス座標(X,Y)
	DirectX::SimpleMath::Vector2 m_prevMouse;
	// マウスホイールのスクロール値
	int m_scrollWheelValue;
	// 一時的なスクロール値の蓄積用変数
	int m_tempScrollValue;
	// 前回のスクロール値の保存変数
	int m_prevScrollWheelValue;
	// 回転角
	DirectX::SimpleMath::Vector2 m_angle;

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	// 回転量
	DirectX::SimpleMath::Matrix m_rotateMatrix;

	// カメラの座標
	DirectX::SimpleMath::Vector3 m_eye;
	DirectX::SimpleMath::Vector3 m_eyePos;
	// 注視点
	DirectX::SimpleMath::Vector3 m_target;

	// 視点移動モード
	bool is_eagleMode;
	bool is_prevEagleMode;
	bool is_allowMode;

public:
	Camera();
	~Camera();

	// 更新処理
	void Update();

	// オブジェクトを揺らす処理
	void ShakeObject(float duration, float tremor, DirectX::SimpleMath::Vector3* pos);


private:
	// マウスの移動距離の計算
	void DraggedDistance(int x, int y);

	// ビュー行列を計算する
	void CalculateViewMatrix();

	// マウスホイールの拡縮処理
	void RollWheelToRate(DirectX::Mouse::State state);

//--------------------------------------------------------//
//アクセサ                                                //
//--------------------------------------------------------//
public:
	// ビュー行列を取得する
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }

	// カメラの座標を取得する
	const DirectX::SimpleMath::Vector3& GetEye() { return m_eye; }

	// カメラの座標移動用セッター(指定座標に移動)
	void SetEyePosition(const DirectX::SimpleMath::Vector3& pos){ m_eyePos = pos;}

	// ターゲットを取得する
	const DirectX::SimpleMath::Vector3& GetTargetPosition() { return m_target; }

	// 射影行列の取得
	const DirectX::SimpleMath::Matrix& CreateProjection(float width, float height,float angle = 45.0f);
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_proj; }

	// カメラ視点移動モード切替
	void SetEagleMode(const bool& flag) { is_eagleMode = flag; }
	void SetArrowMode(const bool& flag) { is_allowMode = flag; }

};

#endif // CAMERA
