/*
 *	@File	PlayerPointGS.hlsl
 *	@Brief	�v���C���[�|�C���g�W�I���g���V�F�[�_�[�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "PlayerPoint.hlsli"

static const int vnum = 4;

// ���_�����S�̐ݒ�
static const float4 offset_array[vnum] =
{
	float4(-0.5f,  0.5f, 0.0f, 0.0f),	// ����
	float4( 0.5f,  0.5f, 0.0f, 0.0f),	// �E��
	float4(-0.5f, -0.5f, 0.0f, 0.0f),	// ����
	float4( 0.5f, -0.5f, 0.0f, 0.0f),	// �E��

};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output)
{

	for (int i = 0; i < vnum; i++)
	{
		PS_INPUT element;

		float4 res = offset_array[i];

		element.Pos = input[0].Pos + mul(offset_array[i], matWorld);

		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);

		element.Color = input[0].Color;
		element.Tex.x = offset_array[i].x + 0.5f;
		element.Tex.y = -offset_array[i].y + 0.5f;



		output.Append(element);
	}
	output.RestartStrip();
}