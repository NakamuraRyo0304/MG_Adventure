/*
 *	@File	PS_OutLine.hlsl
 *	@Brief	アウトラインシェーダー(ピクセル)
 *	@Date	2024-01-16
 *  @Author NakamuraRyo
 */

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

// ヘッダー
#include "Blocks.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = tex.Sample(samLinear, input.Tex);

    return output;
}