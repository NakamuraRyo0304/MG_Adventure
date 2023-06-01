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

private:
	// 回転角
	DirectX::SimpleMath::Vector2 m_angle;

	// 前回のマウス座標(X,Y)
	DirectX::SimpleMath::Vector2 m_prevMouse;	
	
	// マウスホイールのスクロール値
	int m_scrollWheelValue;

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;

	// カメラの座標
	DirectX::SimpleMath::Vector3 m_eye;	
	DirectX::SimpleMath::Vector3 m_eyePos;	

	// 注視点
	DirectX::SimpleMath::Vector3 m_target;

	// 画角
	float m_viewAngle;

	// カメラ動かすモード
	bool is_moveMode;

	// 視点移動モード
	bool is_eagleMode;

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

	// カメラを移動させる
	void MoveCamera(DirectX::SimpleMath::Vector3 eye,DirectX::SimpleMath::Vector3 up,DirectX::Keyboard::State keyState);
	
	// カメラを揺らす処理
	void ShakeCamera(float duration, float tremor, DirectX::SimpleMath::Vector3* pos);


//--------------------------------------------------------//
//アクセサ                                                //
//--------------------------------------------------------//
	// ビュー行列を取得する
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }

	// カメラの座標を取得する
	const DirectX::SimpleMath::Vector3& GetEyePosition() { return m_eye; }

	// カメラの注視点座標を取得する
	const DirectX::SimpleMath::Vector3& GetTargetPosition() { return m_target; }

	// 射影行列の取得
	const DirectX::SimpleMath::Matrix& GetProjection(float width, float height,float angle);

	// カメラ移動モード切替
	void SetMoveMode(bool flag) { is_moveMode = flag; }

	// カメラ視点移動モード切替
	void SetEagleMode(bool flag) { is_eagleMode = flag; }

	// カメラの座標をセットする
	void SetEyePosition(const DirectX::SimpleMath::Vector3& pos) 
	{ 
		m_eyePos = pos; 
		m_target = pos; 
	}
};

#endif // CAMERA
