/*
 *	@File	RayCast.cpp
 *	@Brief	レイを飛ばしてスクリーン座標をワールド座標に変換する。
 *	@Date	2023-05-26
 *  @Author NakamuraRyo
 */

 // 使用関数群の参考文献
 //-------------------------------------------------------------------------------------//
 //   まるぺけ
 //   その４８　スクリーン座標でワールド空間の地面を指す
 //   http://marupeke296.com/DXG_No48_PointGroundInScreen.html
 // 　�F 公開、スクリーン座標点と地面の交点算出関数より
 //-------------------------------------------------------------------------------------//

#include "pch.h"

#include "RayCast.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
RayCast::RayCast()
	: m_screenSize{}			// スクリーンのサイズ
	, m_conScreenPos{}			// ワールド座標に変換した値を保存する変数
	, is_clickFlag{}			// クリック判定
{

}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
RayCast::~RayCast()
{

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="mouseState">マウスを渡す</param>
/// <returns>なし</returns>
void RayCast::Update(Mouse::State& mouseState)
{
	// クリックしてるときはTrueを返す
	is_clickFlag = mouseState.leftButton;

	// レイを飛ばす処理
	m_conScreenPos = ShotRay(mouseState.x, mouseState.y);
}

/// <summary>
/// スクリーン座標をワールド座標に変換する
/// </summary>
/// <param name="mx">マウスX</param>
/// <param name="my">マウスY</param>
/// <param name="fz">任意の値</param>
/// <param name="width">画面横幅</param>
/// <param name="height">画面縦幅</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <returns>変換後の座標</returns>
SimpleMath::Vector3 RayCast::ConvertScreenToWorld(int mx, int my, float fz,
	int width, int height, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// 各行列の逆行列を算出
	XMMATRIX revView, revProj, viewport, revViewport = XMMATRIX::XMMATRIX();

	// 逆行列に変換
	revView = XMMatrixInverse(nullptr, view);
	revProj = XMMatrixInverse(nullptr, proj);

	// ビューポートを初期化
	viewport = XMMatrixIdentity();

	XMFLOAT4X4 matrix = XMFLOAT4X4::XMFLOAT4X4();

	// スケールとオフセットを変換
	matrix._11 =   width / 2.0f;
	matrix._22 = -height / 2.0f;
	matrix._41 =   width / 2.0f;
	matrix._42 =  height / 2.0f;

	// ビューポートに行列変換
	viewport += XMLoadFloat4x4(&matrix);

	// ビューポートの逆行列を作成
	revViewport = XMMatrixInverse(nullptr,viewport);

	// 逆変換
	SimpleMath::Matrix tmp = revViewport * revProj * revView;

	SimpleMath::Vector3 value = XMVector3TransformCoord(
		SimpleMath::Vector3(static_cast<float>(mx), static_cast<float>(my), fz), tmp);

	return value;
}

/// <summary>
/// レイを飛ばして地面との交点を取る
/// </summary>
/// <param name="mx">マウスX</param>
/// <param name="my">マウスY</param>
/// <returns>当たった地面との交点</returns>
SimpleMath::Vector3 RayCast::ShotRay(int mx, int my)
{
	// 最近、最遠、レイを定義
	SimpleMath::Vector3 nearpos;
	SimpleMath::Vector3 farpos;
	SimpleMath::Vector3 ray;

	// 最近距離をスクリーンからワールドに変換
	nearpos = ConvertScreenToWorld(mx, my, 0.0f,
		static_cast<int>(m_screenSize.x),  static_cast<int>(m_screenSize.y),
		m_view, m_projection);

	// 最遠距離をスクリーンからワールドに変換
	farpos  = ConvertScreenToWorld(mx, my, 1.0f,
		static_cast<int>(m_screenSize.x),  static_cast<int>(m_screenSize.y),
		m_view, m_projection);

	// レイの長さを求めて正規化する
	ray = farpos - nearpos;
	ray.Normalize();

	// Y座標打消しの初期化
	SimpleMath::Vector3 output = SimpleMath::Vector3::Zero;

	// 床との交差が起きている場合は交点、起きていない場合は遠くの壁との交点を出力
	if (ray.y <= 0)
	{
		// 床との交点を求める
		SimpleMath::Vector3 rDot = XMVector3Dot(     ray, SimpleMath::Vector3(0, 1, 0));
		SimpleMath::Vector3 nDot = XMVector3Dot(-nearpos, SimpleMath::Vector3(0, 1, 0));
  		output = nearpos + (nDot / rDot) * ray;

		// Y軸移動なくすために0代入
		// 本ゲームでは、マウスホイールでY軸移動
		output.y = 0;
	}
	else
	{
		output = farpos;
	}

	return output;
}