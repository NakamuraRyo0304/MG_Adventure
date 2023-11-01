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
	const float ALLOW_SPEED = 0.01f;

	// カメラホイール上下限
	static const int MAX_SCROLL_VALUE;
	static const int MIN_SCROLL_VALUE;

	// カメラ表示距離
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 245.0f;

	// カメラ画角上下限
	const float ANGLE_X_MIN = 44.0f;
	const float ANGLE_X_MAX = 45.5f;

private: //-------------------------------------------------------------------------------------//

	struct PrevMouse { DirectX::SimpleMath::Vector2 pos; int wheel; bool eagle; };

	// 前回のマウス情報
	PrevMouse m_prevMouse;
	// マウスホイールのスクロール値
	int m_scrollWheelValue, m_tempScrollValue;

	// 回転角
	DirectX::SimpleMath::Vector2 m_angle;

	// ビュー行列・プロジェクション行列・回転行列
	DirectX::SimpleMath::Matrix m_view, m_projection, m_rotateMatrix;

	// カメラの座標・加算用座標・注視点
	DirectX::SimpleMath::Vector3 m_position, m_addPos, m_target;

	// 視点移動モード
	bool is_eagleMode, is_allowMode;

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
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

	// カメラの座標移動用セッター(指定座標に移動)
	void AddEyePosition(const DirectX::SimpleMath::Vector3& pos){ m_addPos = pos;}

	// ターゲットを取得する
	const DirectX::SimpleMath::Vector3& GetTarget() { return m_target; }

	// 射影行列の取得
	const DirectX::SimpleMath::Matrix& CreateProjection(float width, float height,float angle = 45.0f);
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	// マウスドラッグで視点を回転
	void SetEagleMode(const bool& flag = true) { is_eagleMode = flag; }
	// 十字キーで視点を回転
	void SetArrowMode(const bool& flag = true) { is_allowMode = flag; }

};

#endif // CAMERA
