/*
 *  @File   SM_VS_Depth.hlsl
 *  @Brief  ���_�̈ʒu�𓊉e��Ԃ֕ϊ����ďo�͂���B
 *  @Date   2023-11-24
 *  @Author NakamuraRyo
 */

#include "SM_Common.hlsli"

// ���_�V�F�[�_�[�̓��͍\����
struct VSDInput
{
    float4 Position   : SV_Position;    // ���_�̃X�N���[�����W
    float3 Normal     : NORMAL;         // ���_�̖@���x�N�g��
    float2 TexCoord   : TEXCOORD0;      // �e�N�X�`�����W
    float4 Color      : COLOR;          // ���_�̐F
};

// ���_�V�F�[�_�[�̏o�͍\����
struct VSDOutput
{
    float4 PositionPS : SV_Position;    // �s�N�Z���V�F�[�_�[�ɓn�����_�̃X�N���[�����W
};

// ���_�V�F�[�_�[�̃��C���֐�
VSDOutput main(VSDInput vin)
{
    VSDOutput output;

    // ���_�̈ʒu�𓊉e��Ԃ֕ϊ�
    output.PositionPS = mul(vin.Position, WorldViewProj);

    return output;
}
