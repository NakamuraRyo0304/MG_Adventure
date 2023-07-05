/*
 *	@File	LocatePS.hlsl
 *	@Brief	ピクセルシェーダー。
 *	@Date	2023-07-04
 *  @Author NakamuraRyo
 */

#include "Locate.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// ポリゴン画像表示
	float4 output = tex.Sample(samLinear, input.Tex);

	return output;
}