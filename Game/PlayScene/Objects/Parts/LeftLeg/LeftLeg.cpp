/*
 *	@File	LeftLeg.cpp
 *	@Brief	プレイヤーの左足。
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/UserUtility.h"
#include "LeftLeg.h"

 // コンストラクタ
LeftLeg::LeftLeg(std::unique_ptr<Model> model)
	: IParts(std::move(model))	// 基底クラス
{
}

// デストラクタ
LeftLeg::~LeftLeg()
{
}

// 通常の更新
void LeftLeg::Update()
{

}

// 描画処理
void LeftLeg::Draw(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	GetModel()->Draw(_context, states, GetMatrix() * GetParentMatrix(), view, proj);
}

