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
	, m_move{ 0.0f }			// 足の動き
{
}

// デストラクタ
LeftLeg::~LeftLeg()
{
}

// 通常の更新
void LeftLeg::Update()
{
	auto _key = Keyboard::Get().GetState();

	// 脚の動き
	(_key.W || _key.A || _key.S || _key.D) ? m_move++ : m_move = 0.0f;

	// 座標の更新
	SimpleMath::Vector3 _pos = SimpleMath::Vector3(0.0f, 0.0f, sinf(m_move) * SPEED);

	// 行列の作成
	SimpleMath::Matrix _trans =	SimpleMath::Matrix::CreateTranslation(_pos);

	// 行列の設定
	SetMatrix(_trans);
}

// 描画処理
void LeftLeg::Draw(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	GetModel()->Draw(_context, states, GetMatrix() * GetParentMatrix(), view, proj);
}

