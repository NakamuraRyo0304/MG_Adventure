/*
 *	@File	TomPS.hlsl
 *	@Brief	�g���s�N�Z���V�F�[�_�[�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "Tom.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//�g���̉摜�\��
	float4 output = tex.Sample(samLinear, input.Tex);

	//�^�����Ȕ|���S��
	float4 white = float4(1, 1, 1, 1);

	return output;
}