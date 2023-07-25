/*
 *	@File	PlayerPointPS.hlsl
 *	@Brief	�v���C���[�|�C���g�s�N�Z���V�F�[�_�[�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "PlayerPoint.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// �v���C���[�̈ʒu���̉摜�\��
	float4 output = tex.Sample(samLinear, input.Tex);

	return output;
}