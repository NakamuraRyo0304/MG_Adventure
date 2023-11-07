/*
 *	@File	PS_Gravity.hlsl
 *	@Brief	重力ブロックのピクセルシェーダー。
 *	@Date	2023-11-07
 *  @Author NakamuraRyo
 */

// セマンティクス
struct PS_INPUT
{
    float4 Color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    input.Color *= 0.5;
    input.Color.a *= 1.9;
    return input.Color;
}