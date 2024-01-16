/*
 *	@File	PS_OutLine.hlsl
 *	@Brief	�A�E�g���C���V�F�[�_�[(�s�N�Z��)
 *	@Date	2024-01-16
 *  @Author NakamuraRyo
 */

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

// �w�b�_�[
#include "Blocks.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = tex.Sample(samLinear, input.Tex);

    return output;
}