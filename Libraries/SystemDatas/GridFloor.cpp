/*
 *	@File	GridFloor.cpp
 *	@Brief	デバッグ用グリッド。
 *	@Date	2021-02-09
 *  @Author T.Matsui
 *  @Editor NakamuraRyo
 */

#include "pch.h"
#include "GridFloor.h"

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
Debug::GridFloor::GridFloor(ID3D11Device1* device, ID3D11DeviceContext1* context, const int divsX, const int divsY)
	: m_divsX(divsX)
	, m_divsY(divsY)
{
	// エフェクトの生成
	m_basicEffect = std::make_unique<BasicEffect>(device);

	// 頂点カラーの設定
	m_basicEffect->SetVertexColorEnabled(true);

	// プリミティブバッチの生成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	// インプットレイアウトの設定
	void const* _shaderByteCode;
	size_t _byteCodeLength;
	m_basicEffect->GetVertexShaderBytecode(&_shaderByteCode, &_byteCodeLength);
	device->CreateInputLayout(
		VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		_shaderByteCode,
		_byteCodeLength,
		m_inputLayout.GetAddressOf()
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
	SimpleMath::Matrix _world = SimpleMath::Matrix::Identity;

	context->IASetInputLayout(m_inputLayout.Get());
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	m_basicEffect->SetWorld(_world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);

	m_primitiveBatch->Begin();

	const XMVECTORF32 _axisX = { (float)m_divsX, 0.0f, 0.0f };
	const XMVECTORF32 _axisY = { 0.0f, 0.0f, (float)m_divsY };

	size_t _divsX = std::max<size_t>(1, m_divsX);
	FXMVECTOR _origin = g_XMZero;
	// 垂直線
	for (size_t i = 0; i <= _divsX; ++i)
	{
		float _scaleFactor = float(i) / float(_divsX);
		_scaleFactor = (_scaleFactor * 1.0f) - 0.5f;
		XMVECTOR _scaleV = XMVectorScale(_axisX, _scaleFactor);
		_scaleV = XMVectorAdd(_scaleV, _origin);

		VertexPositionColor _v1(XMVectorSubtract(_scaleV, _axisY * 0.5f), color);
		VertexPositionColor _v2(XMVectorAdd(_scaleV, _axisY * 0.5f), color);
		m_primitiveBatch->DrawLine(_v1, _v2);
	}
	// 水平線
	size_t _divsY = std::max<size_t>(1, m_divsY);
	for (size_t i = 0; i <= _divsY; i++)
	{
		float _scaleFactor = float(i) / float(_divsY);
		_scaleFactor = (_scaleFactor * 1.0f) - 0.5f;
		XMVECTOR _scaleV = XMVectorScale(_axisY, _scaleFactor);
		_scaleV = XMVectorAdd(_scaleV, _origin);

		VertexPositionColor _v1(XMVectorSubtract(_scaleV, _axisX * 0.5f), color);
		VertexPositionColor _v2(XMVectorAdd(_scaleV, _axisX * 0.5f), color);
		m_primitiveBatch->DrawLine(_v1, _v2);
	}

	m_primitiveBatch->End();
}
