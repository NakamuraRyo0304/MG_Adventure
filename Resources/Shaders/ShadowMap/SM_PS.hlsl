/*
 *  @File   SM_PS.hlsl
 *  @Brief  �V���h�E�}�b�v�ɏ����o���s�N�Z���V�F�[�_�[�B
 *  @Date   2023-11-24
 *  @Author NakamuraRyo
 */

#include "SM_Common.hlsli"

// �V���h�E�}�b�v�e�N�X�`��
Texture2D Tex : register(t1);

// �V���h�E�}�b�v�p�e�N�X�`���T���v���[
SamplerComparisonState Sam : register(s1);

// �s�N�Z���V�F�[�_�[�̓��͍\����
struct PSInput
{
    float2 TexCoord     : TEXCOORD0; // �e�N�X�`�����W
    float4 PositionWS   : TEXCOORD1; // ���[���h��Ԃ̒��_���W
    float3 NormalWS     : TEXCOORD2; // ���[���h��Ԃ̖@���x�N�g��
    float4 LightPosPS   : TEXCOORD3; // ���C�g�̓��e��Ԃ̒��_���W
};

// �}�b�n�o���h
#define EPSILON 0.0005f

// �s�N�Z���V�F�[�_�[�̃��C���֐�
float4 main(PSInput pin) : SV_TARGET0
{
    // �X�؃L�����̏�����
    float3 specular = 0.f;

    // ���C�g�̌�������ςŋ��߂�
    float3 lightDir = (pin.PositionWS.xyz - LightPos.xyz) *
    rsqrt(dot(pin.PositionWS.xyz - LightPos.xyz, pin.PositionWS.xyz - LightPos.xyz));

    // ���K���f�o�C�X���W�ɂ���
    pin.LightPosPS.xyz /= pin.LightPosPS.w;

    // �Q�Ƃ���V���h�E�}�b�v��UV�l�����߂�
    float2 uv = pin.LightPosPS.xy * float2(0.5f, -0.5f) + 0.5f;

    // �V���h�E�}�b�v�̐[�x�l�ƃ��C�g��Ԃ̃s�N�Z����Z�l���r���ĉe�ɂȂ邩���ׂ�
    float percentLit = Tex.SampleCmpLevelZero(Sam, uv, pin.LightPosPS.z - EPSILON).x;

    // �f�B�q���[�Y�F�̌v�Z
    // �@���𐳋K��
    float3 worldNormal = pin.NormalWS *
    rsqrt(dot(pin.NormalWS, pin.NormalWS));

    // ���̋�������ςŋ��߂�
    float3 dotLightPower = saturate(dot(-lightDir, worldNormal));

    // ���C�g�ɂ�閾�邳�����߂�
    float3 lightAmount = dotLightPower * percentLit * (1.0f - LightAmbient) + LightAmbient;

    // �e�N�X�`���̐F���擾���ăf�B�t���[�Y�F���|����
    float4 diffuse = float4(DiffuseColor.rgb * lightAmount, DiffuseColor.a);

    // �X�؃L�����F�̌v�Z
    // �����x�N�g������ςŋ��߂�
    float3 eyeVector = (EyePosition - pin.PositionWS.xyz) *
    rsqrt(dot(EyePosition - pin.PositionWS.xyz, EyePosition - pin.PositionWS.xyz));

    // �n�[�t�x�N�g������ςŋ��߂�
    float3 halfVector = (eyeVector - lightDir) *
    rsqrt(dot(eyeVector - lightDir, eyeVector - lightDir));

    // �X�y�L�����̉e����������ς��g�����߂�
    diffuse.rgb += pow(saturate(dot(halfVector, worldNormal)), SpecularPower) *
    dotLightPower * SpecularColor * percentLit * diffuse.a;

    // �e�N�X�`���F���|����
    float4 color = Texture.Sample(Sampler, pin.TexCoord) * diffuse;

    return color;
}
