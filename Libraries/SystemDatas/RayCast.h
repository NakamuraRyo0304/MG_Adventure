/*
 *	@File	RayCast.h
 *	@Brief	レイを飛ばしてスクリーン座標をワールド座標に変換する。
 *	@Date	2023-05-26
 *  @Author NakamuraRyo
 */

class RayCast
{
private:
	// マウスをワールド座標に変換した値を保存する変数
	DirectX::SimpleMath::Vector3 m_convertPosition;

	// 行列
	DirectX::SimpleMath::Matrix m_view, m_projection;

	// スクリーンサイズ
	DirectX::SimpleMath::Vector2 m_screenSize;

public:
	RayCast();
	~RayCast();

	// 更新処理
	void Update(DirectX::Mouse::State& mouseState);

private:

	// レイを飛ばして面との交点を求める
	DirectX::SimpleMath::Vector3 ShotRay(int mx, int my);

	// マウスのスクリーン座標をワールド座標に変換する
	DirectX::SimpleMath::Vector3 ConvertScreenToWorld(int mx, int my, float fz, int width, int height,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

public:
	// マウスのワールド座標のゲッター
	const DirectX::SimpleMath::Vector3& GetWorldMousePosition() { return m_convertPosition; }

	// スクリーンサイズのセッター
	void SetScreenSize(float width, float height) { m_screenSize = { width,height }; }

	// 行列のセッター
	void SetMatrix(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) { m_view = view; m_projection = proj; }
};