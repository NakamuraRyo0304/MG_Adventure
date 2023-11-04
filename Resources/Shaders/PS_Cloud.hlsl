/*
 *	@File	PS_Cloud.hlsl
 *	@Brief	雲ブロックのピクセルシェーダー。
 *	@Date	2023-10-17
 *  @Author NakamuraRyo
 */

float4 main(float4 color : COLOR0) : SV_TARGET
{
    color.a = 0.95f;
    return color;
}