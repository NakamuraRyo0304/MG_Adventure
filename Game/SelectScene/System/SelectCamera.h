/*
 *	@File	SelectCamera.h
 *	@Brief	セレクトシーンのカメラクラス。
 *	@Date	2023-11-11
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTCAMERA
#define SELECTCAMERA

class SelectCamera
{
private:

	// カメラ行列
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	// カメラポジション
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_target;

	// スクリーンサイズ
	DirectX::SimpleMath::Vector2 m_screenSize;

	// 切り替え可能か判定する
	bool is_canChangeFlag;

private:

	// カメラの最近距離
	const float NEAR_PLANE = 0.1f;

	// カメラの最遠距離
	const float FAR_PLANE = 245.0f;

	// カメラアングル
	const float ANGLE = 45.0f;

	// カメラのY座標
	const float POS_Y = 30.0f;

	// カメラの回転半径
	const float CAMERA_RADIUS = 4.0f;

	// 接近速度
	const float APPROACH_SPEED = 0.045f;

	// 上下割合
	const float VERTICAL_MOTION = 1.5f;

	// 視点の動作速度
	const float UP_VALUE = 60.0f;
	const float UP_SPAN = 0.25f;
	const float UP_SPEED = 0.8f;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="screenSize">スクリーンサイズ</param>
	/// <returns>なし</returns>
	SelectCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~SelectCamera();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update();
	/// <summary>
	/// カメラのターゲットの更新
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void MoveTarget();
	// 切り替え可能か判定を返す
	const bool& IsCanChange(){ return is_canChangeFlag;}
	// ビュー行列を取得
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }
	// プロジェクション行列を取得
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }
	// 座標を取得
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
	// 座標を設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos = { 0.f,1.f,1.f }) { m_position = pos; }
	// 注視点を取得
	const DirectX::SimpleMath::Vector3& GetTarget() { return m_target; }
	// 注視点を設定
	void SetTarget(const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero) { m_target = target; }

private:

	/// <summary>
	/// プロジェクションを作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateProjection();

};

#endif // SELECTCAMERA