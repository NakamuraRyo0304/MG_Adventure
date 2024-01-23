/*
 *  @File   SM_VS_Depth.hlsl
 *  @Brief  頂点の位置を投影空間へ変換して出力する。
 *  @Date   2023-11-24
 *  @Author NakamuraRyo
 */

#include "SM_Common.hlsli"

// 頂点シェーダーの入力構造体
struct VSDInput
{
    float4 Position   : SV_Position;    // 頂点のスクリーン座標
    float3 Normal     : NORMAL;         // 頂点の法線ベクトル
    float2 TexCoord   : TEXCOORD0;      // テクスチャ座標
    float4 Color      : COLOR;          // 頂点の色
};

// 頂点シェーダーの出力構造体
struct VSDOutput
{
    float4 PositionPS : SV_Position;    // ピクセルシェーダーに渡す頂点のスクリーン座標
};

// 頂点シェーダーのメイン関数
VSDOutput main(VSDInput vin)
{
    VSDOutput output;

    // 頂点の位置を投影空間へ変換
    output.PositionPS = mul(vin.Position, WorldViewProj);

    return output;
}
