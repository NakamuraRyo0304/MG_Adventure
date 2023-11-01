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
 // 　⑦ 公開、スクリーン座標点と地面の交点算出関数より
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
	, m_convertPosition{}		// ワールド座標に変換した座標
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
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void RayCast::Update()
{
	auto _mouse = Mouse::Get().GetState();

	// レイを飛ばして交点を求める
	m_convertPosition = ShotRay(_mouse.x, _mouse.y);
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
	SimpleMath::Vector3 _nearPos;
	SimpleMath::Vector3 _farPos;
	SimpleMath::Vector3 _ray;

	// 最近距離をスクリーンからワールドに変換
	_nearPos = ConvertScreenToWorld(mx, my, 0.0f,
		static_cast<int>(m_screenSize.x), static_cast<int>(m_screenSize.y),
		m_view, m_projection);

	// 最遠距離をスクリーンからワールドに変換
	_farPos = ConvertScreenToWorld(mx, my, 1.0f,
		static_cast<int>(m_screenSize.x), static_cast<int>(m_screenSize.y),
		m_view, m_projection);

	// レイの方向を求める
	_ray = _farPos - _nearPos;
	_ray.Normalize();

	// Y座標打消しの初期化
	SimpleMath::Vector3 _output = SimpleMath::Vector3::Zero;

	// 床との交差が起きている場合は交点、起きていない場合は遠くの壁との交点を出力
	if (_ray.y <= 0)
	{
		// 床との交点を求める
		SimpleMath::Vector3 _rayDot = XMVector3Dot(_ray, SimpleMath::Vector3(0, 1, 0));
		SimpleMath::Vector3 _nearDot = XMVector3Dot(-_nearPos, SimpleMath::Vector3(0, 1, 0));
		_output = _nearPos + (_nearDot / _rayDot) * _ray;

		// Yで交点XZを見つけて、Y要素を消す
		_output.y = 0;
	}
	else
	{
		// 当たっていなければ最遠距離（実質的な無限）を出力
		_output = _farPos;
	}

	return _output;
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
	XMMATRIX _revView, _revProj, _vPort, _revVPort = XMMATRIX::XMMATRIX();

	// 逆行列に変換
	_revView = XMMatrixInverse(nullptr, view);
	_revProj = XMMatrixInverse(nullptr, proj);

	// ビューポートを初期化
	_vPort = XMMatrixIdentity();

	XMFLOAT4X4 _mat = XMFLOAT4X4::XMFLOAT4X4();

	// スケールとオフセットを変換
	_mat._11 =   width / 2.0f;
	_mat._22 = -height / 2.0f;
	_mat._41 =   width / 2.0f;
	_mat._42 =  height / 2.0f;

	// ビューポートに行列変換
	_vPort += XMLoadFloat4x4(&_mat);

	// ビューポートの逆行列を作成
	_revVPort = XMMatrixInverse(nullptr,_vPort);

	// 逆変換
	XMMATRIX _revTmp = _revVPort * _revProj * _revView;

	XMVECTOR _value = XMVector3TransformCoord(
		SimpleMath::Vector3(static_cast<float>(mx), static_cast<float>(my), fz), _revTmp);

	return _value;
}
