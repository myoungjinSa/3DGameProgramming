//게임 객체의 정보를 위한 상수 버퍼를 선언한다.
cbuffer cbGameObjectInfo : register(b0)
{
	matrix gmtxWorld;
	int1 gcolor;		//비행기 컬러
	int3 paddle; 

};
//카메라의 정보를 위한 상수 버퍼를 선언한다.
cbuffer cbCameraInfo : register(b1)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
};
//정점 셰이더의 입력을 위한 구조체를 선언한다.
struct VS_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};
//정점 셰이더의 출력(픽셀 셰이더의 입력)을 위한 구조체를 선언한다.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};
//정점 셰이더를 정의한다.
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
//픽셀 셰이더를 정의한다.
float4 PSDiffused(VS_OUTPUT input) : SV_TARGET
{
	//input.color = gcolor;
	return(input.color);
}