/*
 *  @File   SM_PS_Depth.hlsl
 *  @Brief  ���K���f�o�C�X���W�̐[�x���o�͂���B
 *  @Date   2023-11-24
 *  @Author NakamuraRyo
 */

// �s�N�Z���V�F�[�_�[�̓��͍\����
struct PSDInput
{
    float4 PositionPS : SV_Position; // ���_�̃X�N���[�����W
};

// �s�N�Z���V�F�[�_�[�̃��C���֐�
float4 main(PSDInput pdin) : SV_TARGET0
{
    // Z�l���o�͂���
    return pdin.PositionPS.z;
}
