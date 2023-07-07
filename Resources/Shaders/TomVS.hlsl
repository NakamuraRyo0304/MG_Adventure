/*
 *	@File	TomVS.hlsl
 *	@Brief	�g���o�[�e�b�N�X�V�F�[�_�[�B
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