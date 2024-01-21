/*
 *	@File	PS_Transition.hlsli
 *	@Brief	�g�����W�V�����V�F�[�_�[
 *	@Date	2023-10-28
 *  @Author NakamuraRyo
 */

#include "Transition.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

#define INTENSITY 0.2f

float4 main(PS_INPUT input) : SV_TARGET
{
    // ���F���o��
    float4 _output = float4(0.95f, 0.95f, 1.0f, 1.0f);

    // �e�N�X�`���̐F�����擾
    float4 _color = tex.Sample(samLinear, input.Tex);

    // �G�b�W������ݒ肵�A����肵�Ȃ���t�F�[�h������
    float _edge = saturate((_color.b - Time.x) / INTENSITY);
    _output.a *= (Time.x - _edge);
    return _output;
}
