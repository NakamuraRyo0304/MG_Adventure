/*
 *	@File	Block.hlsli
 *	@Brief	アウトラインヘッダー
 *	@Date	2024-01-16
 *  @Author NakamuraRyo
 */

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