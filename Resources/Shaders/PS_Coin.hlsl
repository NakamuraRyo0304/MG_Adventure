/*
 *	@File	PS_Coin.hlsl
 *	@Brief	コインの輝きピクセルシェーダー。
 *	@Date	2023-11-07
 *  @Author NakamuraRyo
 */

// コンスタントバッファ
cbuffer ConstBuff : register(b0)
{
    float4 time; // 時間
    float4 d1, d2, d3; // ダミーデータ
};

// 輝きを計算する
float CalculateGlowStrength(float time)
{
    // 時間に応じて輝きの強さを計算
    return sin(time);
}
float4 main(float4 color : COLOR0) : SV_TARGET
{
    // 時間に応じて値を変動させる
    float timeValue = time.x;

    // 輝きの強さを計算
    float gs = CalculateGlowStrength(timeValue);

    // ベースの色は黄色
    float4 baseColor = float4(0.9, 0.9, 0.0, 1.0);

    // オレンジっぽい輝きに輝きの強さを加える
    float gsp = (gs + 1) * 0.2;
    float4 finalColor = baseColor + float4(gsp, gsp, 0.1, 1.0) * gs;

    return finalColor;
}