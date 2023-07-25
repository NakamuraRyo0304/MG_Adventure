/*
 *	@File	PlayerShadowPS.hlsl
 *	@Brief	�v���C���[�̉e�V�F�[�_�[
 *	@Date	2023-07-24
 *  @Author NakamuraRyo
 */

#include "PlayerShadow.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

// �I�t�Z�b�g�l
const float shadowOffsetY = -1.0f;

float4 main(PS_INPUT input) : SV_TARGET
{
    // �v���C���[�̈ʒu���̉摜�\��
    float4 output = tex.Sample(samLinear, input.Tex);

    // �v���C���[�̉e��Y���W���I�t�Z�b�g�l��������
    float shadowY = input.Tex.y + shadowOffsetY;

    // �e�̍��W����ʊO�ɏo�Ȃ��悤�ɐ���
    shadowY = max(0.0f, shadowY);

    // �v���C���[�̉e�̈ʒu���č\�z
    output.w = tex.Sample(samLinear, float2(input.Tex.x, shadowY)).w;

    return output;
}