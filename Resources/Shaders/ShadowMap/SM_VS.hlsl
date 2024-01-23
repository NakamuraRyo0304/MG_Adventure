/*
 *  @File   SM_VS.hlsl
 *  @Brief  �s�N�Z���V�F�[�_�[�ɓn���f�[�^���쐬����B
 *  @Date   2023-11-24
 *  @Author NakamuraRyo
 */

#include "SM_Common.hlsli"

// ���_�V�F�[�_�[�̓��͍\����
struct VSInput
{
    float4 Position     : SV_Position;  // ���_�̃X�N���[�����W
    float3 Normal       : NORMAL;       // ���_�̖@���x�N�g��
    float2 TexCoord     : TEXCOORD0;    // �e�N�X�`�����W
    float4 Color        : COLOR;        // ���_�̐F
};

// ���_�V�F�[�_�[�̏o�͍\����
struct VSOutput
{
    float2 TexCoord     : TEXCOORD0;    // �e�N�X�`�����W
    float4 PositionWS   : TEXCOORD1;    // ���[���h��Ԃł̒��_�̈ʒu
    float3 NormalWS     : TEXCOORD2;    // ���[���h��Ԃł̖@���x�N�g��
    float4 LightPosPS   : TEXCOORD3;    // ���C�g�̓��e��Ԃł̒��_�̈ʒu
    float4 PositionPS   : SV_Position;  // �s�N�Z���V�F�[�_�[�ɓn�����_�̃X�N���[�����W
};

// ���_�V�F�[�_�[�̃��C���֐�
VSOutput main(VSInput vin)
{
    VSOutput output;

    // ���_�̈ʒu�𓊉e��Ԃ֕ϊ�
    output.PositionPS = mul(vin.Position, WorldViewProj);

    // ���_�̈ʒu�����[���h��Ԃ֕ϊ�
    output.PositionWS = mul(vin.Position, World);

    // �@���x�N�g�������[���h��Ԃ֕ϊ�
    output.NormalWS = mul(vin.Normal, WorldInverseTranspose);

    // �e�N�X�`����UV���W���R�s�[
    output.TexCoord = vin.TexCoord;

    // ���_�̈ʒu�����C�g�̓��e��Ԃ֕ϊ�
    output.LightPosPS = mul(output.PositionWS, LightViewProj);

    return output;
}
