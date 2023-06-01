/*
 *	@File	RayCast.cpp
 *	@Brief	レイを飛ばしてスクリーン座標をワールド座標に変換する。
 *	@Date	2023-05-26
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "RayCast.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
RayCast::RayCast():
	m_screenSize{},				// スクリーンのサイズ
	is_clickFlag{},				// クリック判定
	m_conScreenPos{}			// ワールド座標に変換した値を保存する変数
{

}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
RayCast::~RayCast()
{

}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
void RayCast::Update(DirectX::Mouse::State& mouseState)
{
	// クリックしてるときはTrueを返す
	is_clickFlag = mouseState.leftButton;

	// レイを飛ばす処理
	m_conScreenPos = ShotRayToWorld(mouseState.x, mouseState.y);
}

//--------------------------------------------------------//
//マウスのスクリーン座標をワールド座標に変換する          //
//--------------------------------------------------------//
DirectX::SimpleMath::Vector3 RayCast::ConvertScreenToWorld(int mx, int my, float fz,
	int width, int height, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	// 各行列の逆行列を算出
	DirectX::XMMATRIX InvView, InvProj, VP, InvViewport = DirectX::XMMATRIX::XMMATRIX();

	// 逆行列に変換
	InvView = DirectX::XMMatrixInverse(nullptr, view);
	InvProj = DirectX::XMMatrixInverse(nullptr, proj);

	VP = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT4X4 matrix = DirectX::XMFLOAT4X4::XMFLOAT4X4();

	// スケールとオフセットを変換
	matrix._11 =   width / 2.0f;
	matrix._22 = -height / 2.0f;
	matrix._41 =   width / 2.0f;
	matrix._42 =  height / 2.0f;

	VP += DirectX::XMLoadFloat4x4(&matrix);

	InvViewport = DirectX::XMMatrixInverse(nullptr,VP);

	// 逆変換
	DirectX::SimpleMath::Matrix tmp = InvViewport * InvProj * InvView;

	DirectX::SimpleMath::Vector3 value = DirectX::XMVector3TransformCoord(
		DirectX::SimpleMath::Vector3(static_cast<float>(mx), static_cast<float>(my), fz), tmp);

	return value;
}

//--------------------------------------------------------//
//レイを飛ばして面との交点を求める                        //
//--------------------------------------------------------//
DirectX::SimpleMath::Vector3 RayCast::ShotRayToWorld(int mx, int my)
{
	// 最近、最遠、レイを定義
	DirectX::SimpleMath::Vector3 nearpos;
	DirectX::SimpleMath::Vector3 farpos;
	DirectX::SimpleMath::Vector3 ray;

	// 最近距離をコンバート
	nearpos = ConvertScreenToWorld(mx, my, 0.0f,
		static_cast<int>(m_screenSize.x),  static_cast<int>(m_screenSize.y), 
		m_view, m_proj);

	// 最遠距離をコンバート
	farpos  = ConvertScreenToWorld(mx, my, 1.0f,
		static_cast<int>(m_screenSize.x),  static_cast<int>(m_screenSize.y), 
		m_view, m_proj);
	
	// レイの長さを求めて正規化する
	ray = farpos - nearpos;
	ray.Normalize();

	// Y座標打消しの初期化
	DirectX::SimpleMath::Vector3 output = DirectX::SimpleMath::Vector3::Zero;

	// 床との交差が起きている場合は交点、起きていない場合は遠くの壁との交点を出力
	if (ray.y <= 0) 
	{
		// 床交点
		DirectX::SimpleMath::Vector3 rDot = DirectX::XMVector3Dot(     ray, DirectX::SimpleMath::Vector3(0, 1, 0));
		DirectX::SimpleMath::Vector3 nDot = DirectX::XMVector3Dot(-nearpos, DirectX::SimpleMath::Vector3(0, 1, 0));
  		output = nearpos + (nDot / rDot) * ray;


		// Y軸移動なくすために0代入
		output.y = 0;
	}
	else 
	{
		output = farpos;
	}

	return output;
}