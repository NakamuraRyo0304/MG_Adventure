/*
 *  @File   SM_PS.hlsl
 *  @Brief  シャドウマップに書き出すピクセルシェーダー。
 *  @Date   2023-11-24
 *  @Author NakamuraRyo
 */

#include "SM_Common.hlsli"

// シャドウマップテクスチャ
Texture2D Tex : register(t1);

// シャドウマップ用テクスチャサンプラー
SamplerComparisonState Sam : register(s1);

// ピクセルシェーダーの入力構造体
struct PSInput
{
    float2 TexCoord     : TEXCOORD0; // テクスチャ座標
    float4 PositionWS   : TEXCOORD1; // ワールド空間の頂点座標
    float3 NormalWS     : TEXCOORD2; // ワールド空間の法線ベクトル
    float4 LightPosPS   : TEXCOORD3; // ライトの投影空間の頂点座標
};

// マッハバンド
#define EPSILON 0.0005f

// ピクセルシェーダーのメイン関数
float4 main(PSInput pin) : SV_TARGET0
{
    // スぺキュラの初期化
    float3 specular = 0.f;

    // ライトの向きを内積で求める
    float3 lightDir = (pin.PositionWS.xyz - LightPos.xyz) *
    rsqrt(dot(pin.PositionWS.xyz - LightPos.xyz, pin.PositionWS.xyz - LightPos.xyz));

    // 正規化デバイス座標にする
    pin.LightPosPS.xyz /= pin.LightPosPS.w;

    // 参照するシャドウマップのUV値を求める
    float2 uv = pin.LightPosPS.xy * float2(0.5f, -0.5f) + 0.5f;

    // シャドウマップの深度値とライト空間のピクセルのZ値を比較して影になるか調べる
    float percentLit = Tex.SampleCmpLevelZero(Sam, uv, pin.LightPosPS.z - EPSILON).x;

    // ディヒューズ色の計算
    // 法線を正規化
    float3 worldNormal = pin.NormalWS *
    rsqrt(dot(pin.NormalWS, pin.NormalWS));

    // 光の強さを内積で求める
    float3 dotLightPower = saturate(dot(-lightDir, worldNormal));

    // ライトによる明るさを求める
    float3 lightAmount = dotLightPower * percentLit * (1.0f - LightAmbient) + LightAmbient;

    // テクスチャの色を取得してディフューズ色を掛ける
    float4 diffuse = float4(DiffuseColor.rgb * lightAmount, DiffuseColor.a);

    // スぺキュラ色の計算
    // 視線ベクトルを内積で求める
    float3 eyeVector = (EyePosition - pin.PositionWS.xyz) *
    rsqrt(dot(EyePosition - pin.PositionWS.xyz, EyePosition - pin.PositionWS.xyz));

    // ハーフベクトルを内積で求める
    float3 halfVector = (eyeVector - lightDir) *
    rsqrt(dot(eyeVector - lightDir, eyeVector - lightDir));

    // スペキュラの影響割合を内積を使い求める
    diffuse.rgb += pow(saturate(dot(halfVector, worldNormal)), SpecularPower) *
    dotLightPower * SpecularColor * percentLit * diffuse.a;

    // テクスチャ色を掛ける
    float4 color = Texture.Sample(Sampler, pin.TexCoord) * diffuse;

    return color;
}
