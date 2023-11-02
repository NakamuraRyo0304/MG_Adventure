/*
 *	@File	PlayCamera.h
 *	@Brief	最初のカメラ演出。
 *	@Date	2023-10-03
 *  @Author NakamuraRyo
 */

#ifndef PLAYCAMERA
#define PLAYCAMERA

class PlayCamera
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
	PlayCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~PlayCamera();

	/// <summary>
	/// ビューを作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>ビュー行列</returns>
	const DirectX::SimpleMath::Matrix& CreateView();
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

#endif // PLAYCAMERA