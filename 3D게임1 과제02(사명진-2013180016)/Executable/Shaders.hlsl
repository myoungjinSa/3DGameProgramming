//���� ��ü�� ������ ���� ��� ���۸� �����Ѵ�.
cbuffer cbGameObjectInfo : register(b0)
{
	matrix gmtxWorld;
	int1 gcolor;		//����� �÷�
	int3 paddle; 

};
//ī�޶��� ������ ���� ��� ���۸� �����Ѵ�.
cbuffer cbCameraInfo : register(b1)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
};
//���� ���̴��� �Է��� ���� ����ü�� �����Ѵ�.
struct VS_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};
//���� ���̴��� ���(�ȼ� ���̴��� �Է�)�� ���� ����ü�� �����Ѵ�.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};
//���� ���̴��� �����Ѵ�.
VS_OUTPUT VSDiffused(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView),
		gmtxProjection);
	if (gcolor == 0 )
	{
		input.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (gcolor == 1) {
		input.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (gcolor == 2) {
		input.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (gcolor == 3) {
		input.color = float4(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (gcolor == 4) {
		input.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else if (gcolor == 5) {
		input.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	
	output.color = input.color;
	return(output);
}
//�ȼ� ���̴��� �����Ѵ�.
float4 PSDiffused(VS_OUTPUT input) : SV_TARGET
{
	//input.color = gcolor;
	return(input.color);
}