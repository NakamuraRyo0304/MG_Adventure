/*
 *	@File	PS_Transition.hlsli
 *	@Brief	トランジションシェーダー
 *	@Date	2023-10-28
 *  @Author NakamuraRyo
 */

#include "Transition.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // 黒色を出力
    float4 _output = float4(0.8f, 0.8f, 1.f, 1.f);

    // テクスチャの色情報を取得
    float4 _color = tex.Sample(samLinear, input.Tex);

    // rgbの方が大きい場合は真っ黒
    _output.a = step(_color.r, Time.x + 0.1f);

    return _output;
}