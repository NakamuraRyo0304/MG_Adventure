/*
 *	@File	Head.cpp
 *	@Brief	プレイヤーの頭。
 *	@Date	2023-01-18
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/UserUtility.h"
#include "Head.h"

// コンストラクタ
Head::Head(std::unique_ptr<Model> model)
	: IParts(std::move(model))	// 基底クラス
	, m_moveFB{ 0.0f }			// 前後運動
	, m_normalRotate{}			// 首の回転行列
	, m_rotate{}				// 首の回転量
{
}

// デストラクタ
Head::~Head()
{
}

// 通常の更新
void Head::Update()
{
	auto _key = Keyboard::Get().GetState();

	// 前後運動
	(_key.W || _key.S) ? m_moveFB++ : m_moveFB = 0.0f;

	SimpleMath::Matrix _mat =
		SimpleMath::Matrix::CreateTranslation(
			SimpleMath::Vector3(0.0f, 0.0f, sinf(m_moveFB * FB_SPEED) * 0.05f));

	// 行列をセット
	SetMatrix(_mat);
}

// 描画処理
void Head::Draw(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	GetModel()->Draw(_context, states, GetMatrix() * GetParentMatrix(), view, proj);
}
