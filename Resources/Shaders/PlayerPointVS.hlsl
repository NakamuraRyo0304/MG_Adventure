/*
 *	@File	PlayerPointVS.hlsl
 *	@Brief	�v���C���[�|�C���g�o�[�e�b�N�X�V�F�[�_�[�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "PlayerPoint.hlsli"

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Pos = float4(input.Pos, 1);

	output.Color = input.Color;
	output.Tex = input.Tex;
	return output;
}