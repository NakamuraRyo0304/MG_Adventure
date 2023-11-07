/*
 *	@File	PS_Gravity.hlsl
 *	@Brief	�d�̓u���b�N�̃s�N�Z���V�F�[�_�[�B
 *	@Date	2023-11-07
 *  @Author NakamuraRyo
 */

// �Z�}���e�B�N�X
struct PS_INPUT
{
    float4 Color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    input.Color *= 0.5;
    input.Color.a *= 1.9;
    return input.Color;
}