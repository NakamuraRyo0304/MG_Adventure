/*
 *  @File   SM_PS_Depth.hlsl
 *  @Brief  正規化デバイス座標の深度を出力する。
 *  @Date   2023-11-24
 *  @Author NakamuraRyo
 */

// ピクセルシェーダーの入力構造体
struct PSDInput
{
    float4 PositionPS : SV_Position; // 頂点のスクリーン座標
};

// ピクセルシェーダーのメイン関数
float4 main(PSDInput pdin) : SV_TARGET0
{
    // Z値を出力する
    return pdin.PositionPS.z;
}
