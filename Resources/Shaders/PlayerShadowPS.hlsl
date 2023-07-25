/*
 *	@File	PlayerShadowPS.hlsl
 *	@Brief	プレイヤーの影シェーダー
 *	@Date	2023-07-24
 *  @Author NakamuraRyo
 */

#include "PlayerShadow.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

// オフセット値
const float shadowOffsetY = -1.0f;

float4 main(PS_INPUT input) : SV_TARGET
{
    // プレイヤーの位置情報の画像表示
    float4 output = tex.Sample(samLinear, input.Tex);

    // プレイヤーの影のY座標をオフセット値だけ調整
    float shadowY = input.Tex.y + shadowOffsetY;

    // 影の座標が画面外に出ないように制限
    shadowY = max(0.0f, shadowY);

    // プレイヤーの影の位置を再構築
    output.w = tex.Sample(samLinear, float2(input.Tex.x, shadowY)).w;

    return output;
}