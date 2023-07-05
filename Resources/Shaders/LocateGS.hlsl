/*
 *	@File	LocateGS.hlsl
 *	@Brief	�W�I���g���V�F�[�_�[�B
 *	@Date	2023-07-04
 *  @Author NakamuraRyo
 */

struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(3)]
void main(
	triangle float4 input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i];
		output.Append(element);
	}
}