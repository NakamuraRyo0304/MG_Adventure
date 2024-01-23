/*
 *  @File   SM_Common.hlsli
 *  @Brief  シャドウマップに必要なセマンティクスをまとめたヘッダー。
 *  @Date   2023-11-24
 *  @Author NakamuraRyo
 */

// DXTKのパラメーターヘッダー
#include "ParametersHeader.hlsli"

// シャドウマップに使用するテクスチャとサンプラー
Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

// 定数バッファ
cbuffer Parameters : register(b1)
{
    float4x4 LightViewProj          : packoffset(c0);   // ライトのビュープロジェクション行列
    float4   LightPos               : packoffset(c4);   // ライトの位置
    float3   LightAmbient           : packoffset(c5);   // ライトのアンビエントカラー
};
