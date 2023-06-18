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
public:
	// カメラの距離
	static const float DEFAULT_CAMERA_DISTANCE;

	// カメラの移動速度
	static const float DEFAULT_CAMERA_SPEED;

	// カメラホイール上下限
	static const int MAX_SCROLL_VALUE;
	static const int MIN_SCROLL_VALUE;

private:
	// 回転角
	DirectX::SimpleMath::Vector2 m_angle;

	// 前回のマウス座標(X,Y)
	DirectX::SimpleMath::Vector2 m_prevMouse;	
	
	// マウスホイールのスクロール値
	int m_scrollWheelValue;
	// 一時的なスクロール値の蓄積用変数
	int m_tempScrollValue;
	// 前回のスクロール値の保存変数
	int m_prevScrollWheelValue;


	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;

	// カメラの座標
	DirectX::SimpleMath::Vector3 m_eye;	
	DirectX::SimpleMath::Vector3 m_eyePos;	

	// 注視点
	DirectX::SimpleMath::Vector3 m_target;

	// 視点移動モード
	bool is_eagleMode;
	bool is_prevEagleMode;

	// スクリーンサイズ
	int m_screenWidth, m_screenHeight;

public:
	Camera();
	~Camera();

	// 更新処理
	void Update();

	// マウスの移動距離の計算
	void DraggedDistance(int x, int y);

	// ビュー行列を計算する
	void CalculateViewMatrix();
		
	// カメラを揺らす処理
	void ShakeCamera(float duration, float tremor, DirectX::SimpleMath::Vector3* pos);

	// カメラとオブジェクトの距離を求める
	float CalculateDistanceToObject(const DirectX::SimpleMath::Vector3& objPos);


//--------------------------------------------------------//
//アクセサ                                                //
//--------------------------------------------------------//
	// ビュー行列を取得する
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }

	// カメラの座標を取得する
	const DirectX::SimpleMath::Vector3& GetEyePosition() { return m_eye; }
	void SetEyePosition(const DirectX::SimpleMath::Vector3& pos){ m_eye = pos;}
	
	// カメラの注視点座標を取得する
	const DirectX::SimpleMath::Vector3& GetTargetPosition() { return m_target; }
	void SetTargetPosition(const DirectX::SimpleMath::Vector3& pos) { m_target = pos; }

	// 射影行列の取得
	const DirectX::SimpleMath::Matrix& CreateProjection(float width, float height,float angle = 45.0f);
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_proj; }

	// カメラ視点移動モード切替
	void SetEagleMode(bool flag) { is_eagleMode = flag; }
	const bool GetEagleMode() { return is_eagleMode; }

};

#endif // CAMERA
