/*
 *	@File	PS_Transition.hlsli
 *	@Brief	�g�����W�V�����V�F�[�_�[
 *	@Date	2023-10-28
 *  @Author NakamuraRyo
 */

#include "Transition.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // ���F���o��
    float4 _output = float4(0.8f, 0.8f, 1.f, 1.f);

    // �e�N�X�`���̐F�����擾
    float4 _color = tex.Sample(samLinear, input.Tex);

    // rgb�̕����傫���ꍇ�͐^����
    _output.a = step(_color.r, Time.x + 0.1f);

    return _output;
}