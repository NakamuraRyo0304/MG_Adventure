/*
 *	@File	GridFloor.cpp
 *	@Brief	デバッグ用グリッド。
 *	@Date	2021-02-09
 *  @Author T.Matsui
 */

#include "pch.h"
#include "GridFloor.h"

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
Debug::GridFloor::GridFloor(ID3D11Device1* device, ID3D11DeviceContext1* context, const int divsX, const int divsY)
	: mDivsX(divsX)
	, mDivsY(divsY)
{
	// エフェクトの生成
	mBasicEffect = std::make_unique<BasicEffect>(device);

	// 頂点カラーの設定
	mBasicEffect->SetVertexColorEnabled(true);

	// プリミティブバッチの生成
	mPrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	// インプットレイアウトの設定
	void const* shaderByteCode;
	size_t byteCodeLength;
	mBasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		mInputLayout.GetAddressOf()
	);
}

//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
Debug::GridFloor::~GridFloor()
{
}

//-------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------
void Debug::GridFloor::Draw(ID3D11DeviceContext1* context, CommonStates* states, const SimpleMath::Matrix view, const SimpleMath::Matrix proj, const GXMVECTOR color)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	context->IASetInputLayout(mInputLayout.Get());
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	mBasicEffect->SetWorld(world);
	mBasicEffect->SetView(view);
	mBasicEffect->SetProjection(proj);
	mBasicEffect->Apply(context);

	mPrimitiveBatch->Begin();

	const XMVECTORF32 axisX = { (float)mDivsX, 0.0f, 0.0f };
	const XMVECTORF32 axisY = { 0.0f, 0.0f, (float)mDivsY };

	size_t divsX = std::max<size_t>(1, mDivsX);
	FXMVECTOR origin = g_XMZero;
	// 垂直線
	for (size_t i = 0; i <= divsX; ++i)
	{
		float scaleFactor = float(i) / float(divsX);
		scaleFactor = (scaleFactor * 1.0f) - 0.5f;
		XMVECTOR scaleV = XMVectorScale(axisX, scaleFactor);
		scaleV = XMVectorAdd(scaleV, origin);

		VertexPositionColor v1(XMVectorSubtract(scaleV, axisY * 0.5f), color);
		VertexPositionColor v2(XMVectorAdd(scaleV, axisY * 0.5f), color);
		mPrimitiveBatch->DrawLine(v1, v2);
	}
	// 水平線
	size_t divsY = std::max<size_t>(1, mDivsY);
	for (size_t i = 0; i <= divsY; i++)
	{
		float scaleFactor = float(i) / float(divsY);
		scaleFactor = (scaleFactor * 1.0f) - 0.5f;
		XMVECTOR scaleV = XMVectorScale(axisY, scaleFactor);
		scaleV = XMVectorAdd(scaleV, origin);

		VertexPositionColor v1(XMVectorSubtract(scaleV, axisX * 0.5f), color);
		VertexPositionColor v2(XMVectorAdd(scaleV, axisX * 0.5f), color);
		mPrimitiveBatch->DrawLine(v1, v2);
	}

	mPrimitiveBatch->End();
}
