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
	DirectX::SimpleMath::Vector3 m_conScreenPos;

	// 行列
	DirectX::SimpleMath::Matrix m_view, m_proj;

	// スクリーンサイズ
	DirectX::SimpleMath::Vector2 m_screenSize;

	// クリック判定
	bool is_clickFlag;

public:
	RayCast();
	~RayCast();

	// 更新処理
	void Update(DirectX::Mouse::State& mouseState);

private:
	// マウスのスクリーン座標をワールド座標に変換する
	DirectX::SimpleMath::Vector3 ConvertScreenToWorld(
												 int mx,										// マウスX
												 int my,										// マウスY
												 float fz,										// 単位値
												 int width, 									// 画面横幅
												 int height,									// 画面縦幅
												 DirectX::SimpleMath::Matrix view,				// ビュー行列
												 DirectX::SimpleMath::Matrix proj);		    	// 射影行列

	// レイを飛ばして面との交点を求める
	DirectX::SimpleMath::Vector3 ShotRayToWorld(int mx, int my);

public:
	// マウスのワールド座標のゲッター
	const DirectX::SimpleMath::Vector3& GetWorldMousePosition() { return m_conScreenPos; }

	// クリック判定のゲッター
	const bool GetClickFlag() { return is_clickFlag; }

	// スクリーンサイズのセッター
	void SetScreenSize(float width, float height) { m_screenSize = { width,height }; }

	// 行列のセッター
	void SetMatrix(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) { m_view = view; m_proj = proj; }
};