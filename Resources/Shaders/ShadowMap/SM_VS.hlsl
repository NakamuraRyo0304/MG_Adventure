/*
 *  @File   SM_VS.hlsl
 *  @Brief  ピクセルシェーダーに渡すデータを作成する。
 *  @Date   2023-11-24
 *  @Author NakamuraRyo
 */

#include "SM_Common.hlsli"

// 頂点シェーダーの入力構造体
struct VSInput
{
    float4 Position     : SV_Position;  // 頂点のスクリーン座標
    float3 Normal       : NORMAL;       // 頂点の法線ベクトル
    float2 TexCoord     : TEXCOORD0;    // テクスチャ座標
    float4 Color        : COLOR;        // 頂点の色
};

// 頂点シェーダーの出力構造体
struct VSOutput
{
    float2 TexCoord     : TEXCOORD0;    // テクスチャ座標
    float4 PositionWS   : TEXCOORD1;    // ワールド空間での頂点の位置
    float3 NormalWS     : TEXCOORD2;    // ワールド空間での法線ベクトル
    float4 LightPosPS   : TEXCOORD3;    // ライトの投影空間での頂点の位置
    float4 PositionPS   : SV_Position;  // ピクセルシェーダーに渡す頂点のスクリーン座標
};

// 頂点シェーダーのメイン関数
VSOutput main(VSInput vin)
{
    VSOutput output;

    // 頂点の位置を投影空間へ変換
    output.PositionPS = mul(vin.Position, WorldViewProj);

    // 頂点の位置をワールド空間へ変換
    output.PositionWS = mul(vin.Position, World);

    // 法線ベクトルをワールド空間へ変換
    output.NormalWS = mul(vin.Normal, WorldInverseTranspose);

    // テクスチャのUV座標をコピー
    output.TexCoord = vin.TexCoord;

    // 頂点の位置をライトの投影空間へ変換
    output.LightPosPS = mul(output.PositionWS, LightViewProj);

    return output;
}
