/*
 *	@File	PlayerShadowPS.hlsl
 *	@Brief	�v���C���[�̉e�V�F�[�_�[
 *	@Date	2023-07-24
 *  @Author NakamuraRyo
 */

#include "PlayerShadow.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//�v���C���[�̈ʒu���̉摜�\��
	float4 output = tex.Sample(samLinear, input.Tex);

	return output;
}