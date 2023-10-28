/*
 *	@File	Transition.hlsli
 *	@Brief	�g�����W�V�����V�F�[�_�[
 *	@Date	2023-10-28
 *  @Author NakamuraRyo
 */

cbuffer ConstBuffer : register(b0)
{
    matrix matWorld;
    matrix matView;
    matrix matProj;
    float4 diffuse;
    float4 Time;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};