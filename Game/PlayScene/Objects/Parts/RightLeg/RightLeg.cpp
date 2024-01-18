/*
 *	@File	RightLeg.cpp
 *	@Brief	プレイヤーの右足。
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/UserUtility.h"
#include "RightLeg.h"

 // コンストラクタ
RightLeg::RightLeg(std::unique_ptr<Model> model)
	: IParts(std::move(model))	// 基底クラス
{
}

// デストラクタ
RightLeg::~RightLeg()
{
}

// 通常の更新
void RightLeg::Update()
{

}

// 描画処理
void RightLeg::Draw(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	GetModel()->Draw(_context, states, GetMatrix() * GetParentMatrix(), view, proj);
}

