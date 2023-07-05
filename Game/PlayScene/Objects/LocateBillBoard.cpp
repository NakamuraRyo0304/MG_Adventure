/*
 *	@File	LocateBillBoard.cpp
 *	@Brief	プレイヤーの位置を示すビルボード描画クラス。
 *	@Date	2023-07-04
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "LocateBillBoard.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="引数無し"></param>
LocateBillBoard::LocateBillBoard()
{

}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
LocateBillBoard::~LocateBillBoard()
{

}

/// <summary>
/// ビルボード作成関数
/// </summary>
/// <param name="device">デバイスリソースのID3D11Device1ポインタ</param>
void LocateBillBoard::Create(ID3D11Device1* device)
{
	device;
}

/// <summary>
/// ビルボード作成関数
/// </summary>
/// <param name="eye">カメラの座標</param>
/// <param name="target">注視点</param>
/// <param name="up">上方向の軸指定(基本)</param>
void LocateBillBoard::CreateBillboard(SimpleMath::Vector3 eye, SimpleMath::Vector3 target,SimpleMath::Vector3 up)
{
	eye, target, up;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">シーンのelapsedTime</param>
void LocateBillBoard::Update(float elapsedTime)
{
	elapsedTime;
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void LocateBillBoard::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	view, proj;
}
