/*
 *	@File	PlayerPointPS.hlsl
 *	@Brief	プレイヤーポイントピクセルシェーダー。
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "PlayerPoint.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// プレイヤーの位置情報の画像表示
	float4 output = tex.Sample(samLinear, input.Tex);

	return output;
}