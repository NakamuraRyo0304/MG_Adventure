/*
 *	@File	PS_Transition.hlsli
 *	@Brief	トランジションシェーダー
 *	@Date	2023-10-28
 *  @Author NakamuraRyo
 */

#include "Transition.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

#define INTENSITY 0.2f

float4 main(PS_INPUT input) : SV_TARGET
{
    // 白色を出力
    float4 _output = float4(0.95f, 0.95f, 1.0f, 1.0f);

    // テクスチャの色情報を取得
    float4 _color = tex.Sample(samLinear, input.Tex);

    // エッジ条件を設定し、縁取りしながらフェードさせる
    float _edge = saturate((_color.b - Time.x) / INTENSITY);
    _output.a *= (Time.x - _edge);
    return _output;
}
