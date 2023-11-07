/*
 *	@File	PS_Coin.hlsl
 *	@Brief	�R�C���̋P���s�N�Z���V�F�[�_�[�B
 *	@Date	2023-11-07
 *  @Author NakamuraRyo
 */

// �R���X�^���g�o�b�t�@
cbuffer ConstBuff : register(b0)
{
    float4 time; // ����
    float4 d1, d2, d3; // �_�~�[�f�[�^
};

// �P�����v�Z����
float CalculateGlowStrength(float time)
{
    // ���Ԃɉ����ċP���̋������v�Z
    return sin(time);
}
float4 main(float4 color : COLOR0) : SV_TARGET
{
    // ���Ԃɉ����Ēl��ϓ�������
    float timeValue = time.x;

    // �P���̋������v�Z
    float gs = CalculateGlowStrength(timeValue);

    // �x�[�X�̐F�͉��F
    float4 baseColor = float4(0.9, 0.9, 0.0, 1.0);

    // �I�����W���ۂ��P���ɋP���̋�����������
    float gsp = (gs + 1) * 0.2;
    float4 finalColor = baseColor + float4(gsp, gsp, 0.1, 1.0) * gs;

    return finalColor;
}