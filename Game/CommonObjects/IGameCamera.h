/*
 *	@File	IGameCamera.h
 *	@Brief	カメラの基礎部分。
 *	@Date	2023-11-12
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef IGAMECAMERA
#define IGAMECAMERA

class IGameCamera
{
private:

	// カメラ行列
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	// 描画用座標系
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_target;
	DirectX::SimpleMath::Vector3 m_up;

	// 初期位置
	DirectX::SimpleMath::Vector3 m_initialPosition;

	// スクリーンサイズ
	DirectX::SimpleMath::Vector2 m_screenSize;

private:

	// カメラの最近距離
	const float NEAR_PLANE = 0.1f;

	// カメラの最遠距離
	const float FAR_PLANE = 245.0f;

	// カメラアングル
	const float ANGLE = 45.0f;


public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="screenSize">スクリーンサイズ</param>
	/// <returns>なし</returns>
	IGameCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~IGameCamera() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	virtual void Update() = 0;
	// ビュー行列を取得
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }
	// ビュー行列を設定
	void SetView(const DirectX::SimpleMath::Matrix& view) { m_view = view; }
	// プロジェクション行列を取得
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }
	// 座標を取得
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
	// 座標を設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// 上方向を取得
	const DirectX::SimpleMath::Vector3& GetUp() { return m_up; }
	// 注視点を取得
	const DirectX::SimpleMath::Vector3& GetTarget() { return m_target; }
	// 注視点を設定
	void SetTarget(const DirectX::SimpleMath::Vector3& target) { m_target = target; }
	// 初期位置を設定
	const DirectX::SimpleMath::Vector3& GetInitialPosition() { return m_initialPosition; }
	// 初期位置を取得
	void SetInitialPosition(const DirectX::SimpleMath::Vector3& pos) { m_initialPosition = pos; }

private:

	/// <summary>
	/// プロジェクションを作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateProjection();

};

#endif // IGAMECAMERA