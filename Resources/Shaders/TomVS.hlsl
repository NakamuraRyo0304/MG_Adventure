/*
 *	@File	TomVS.hlsl
 *	@Brief	トムバーテックスシェーダー。
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "Tom.hlsli"

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Pos = float4(input.Pos, 1);

	output.Color = input.Color;
	output.Tex = input.Tex;
	return output;
}