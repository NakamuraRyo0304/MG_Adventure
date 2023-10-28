/*
 *	@File	VS_Transition.hlsli
 *	@Brief	�g�����W�V�����V�F�[�_�[
 *	@Date	2023-10-28
 *  @Author NakamuraRyo
 */

#include "Transition.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    output.Pos = float4(input.Pos, 1);

    output.Color = input.Color;
    output.Tex = input.Tex;
    return output;
}