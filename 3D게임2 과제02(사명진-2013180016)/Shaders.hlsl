
struct MATERIAL
{
	float4					m_cAmbient;
	float4					m_cDiffuse;
	float4					m_cSpecular; //a = power
	float4					m_cEmissive;
};



cbuffer cbPlayerInfo : register(b0)							//Player (서술자)
{
	matrix		gmtxPlayerWorld : packoffset(c0);
};

cbuffer cbCameraInfo : register(b1)							//Camera (서술자)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
	matrix		gmtxViewProjection : packoffset(c8);
	float3		gvCameraPosition : packoffset(c12);
};

cbuffer cbGameObjectInfo : register(b2)						//GameObject (서술자 테이블)
{
	matrix		gmtxGameObject : packoffset(c0);
	//uint		gnPartsID	: packoffset(c4);
	uint		gnMaterial : packoffset(c4);
};


cbuffer cbFog : register(b4)
{
	float4 gcFogColor;
	float4 gvFogParameter; //(mode, start, End, Density)
};

////플레이어의 ...
cbuffer cbFrameObjectInfo : register(b7)
{
	matrix		gmtxFrameObject : packoffset(c0);
	MATERIAL	gMaterial : packoffset(c4);
}

#include "Light.hlsl"
#define LINEAR_FOG 1.0f
#define EXP_FOG 2.0f
#define EXP2_FOG 3.0f

float4 Fog(float4 cColor, float3 vPosition)
{
	float3 vCameraPosition = gvCameraPosition.xyz;
	float3 vPositionToCamera = vCameraPosition - vPosition;
	float fDistanceToCamera = length(vPositionToCamera);
	float fFogFactor = 0.0f;
	if (gvFogParameter.x == LINEAR_FOG)
	{
		float fFogRange = gvFogParameter.z - gvFogParameter.y;
		fFogFactor = saturate((gvFogParameter.z - fDistanceToCamera) / fFogRange);

	}

	float4 cColorByFog = lerp(cColor, gcFogColor, (1-fFogFactor));
	return(cColorByFog);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_DIFFUSED_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VS_DIFFUSED_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	
};

VS_DIFFUSED_OUTPUT VSPlayer(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxPlayerWorld), gmtxView), gmtxProjection);
	output.positionW = mul(input.position, gmtxPlayerWorld);
	output.normalW = mul(float4(input.normal, 1.0f), gmtxPlayerWorld);
	//output.color = input.color;

	return(output);
}

float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	//float4 cIllumination = Lighting(input.positionW,input.normalW);
	input.normalW = normalize(input.normalW);
	float4 color = Lighting(input.positionW, input.normalW);

	return(color);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
struct VS_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VS_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
#ifdef _WITH_VERTEX_LIGHTING
	float4 color : COLOR;
#endif
};

VS_LIGHTING_OUTPUT VSLighting(VS_LIGHTING_INPUT input)
{
	VS_LIGHTING_OUTPUT output;

	output.normalW = mul(input.normal, (float3x3)gmtxFrameObject);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxFrameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
#ifdef _WITH_VERTEX_LIGHTING
	output.normalW = normalize(output.normalW);
	output.color = Lighting(output.positionW, output.normalW);
#endif
	return(output);
}

float4 PSLighting(VS_LIGHTING_OUTPUT input) : SV_TARGET
{
#ifdef _WITH_VERTEX_LIGHTING
	return(input.color);
#else
	input.normalW = normalize(input.normalW);
float4 color = Lighting(input.positionW, input.normalW);

return(color);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTexture : register(t0);
Texture2D gtxtBillboardTexture : register(t3);
Texture2D gtxtGrassBillboardTexture : register(t4);
Texture2D gtxtSkyBoxTexture : register(t6);
SamplerState gSamplerState : register(s0);

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_TEXTURED_OUTPUT VSTextured(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

float4 PSTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtBillboardTexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.3f)
	{
		discard;
	}

	//float4 CFog = Fog(cColor, input.position);
	return(cColor);
}


float4 PSGrassTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	
	float4 cColor = gtxtGrassBillboardTexture.Sample(gSamplerState,input.uv);
	
	

	if (cColor.a <0.3f)
	{
		discard;
	}
	//float4 CFog = Fog(cColor, input.position);

	return (cColor);
}

VS_TEXTURED_OUTPUT VSTextureTransform(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	
	
	output.uv = input.uv;

	return(output);

}

float4 PSTextureTransform(VS_TEXTURED_OUTPUT input) : SV_TARGET
{

	float4 cColor = gtxtGrassBillboardTexture.Sample(gSamplerState,input.uv);



	if (cColor.a <0.4f)
	{
		discard;
	}

	return (cColor);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

float4 PSSkyBox(VS_TEXTURED_OUTPUT input,float4 pos:SV_POSITION) : SV_TARGET
{
	float4 cColor = gtxtSkyBoxTexture.Sample(gSamplerState, input.uv);

	//float4 CFog = Fog(cColor, input.position.xyz);
	float4 CFog = Fog(cColor, pos.xyz);
	float4 FColor = lerp(cColor,CFog,0.9f);
	//float4 FColor = lerp()


	return(FColor);
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTerrainBaseTexture : register(t1);
Texture2D gtxtTerrainDetailTexture : register(t2);
Texture2D gtxtSandBaseTexture : register(t5);
Texture2D gtxtNormalTexture : register(t10);


struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 normal : NORMAL;
	
};

struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float4 color : COLOR0;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 normalW : NORMAL;
};

struct GS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float4 color : COLOR0;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 normalW : NORMAL;

};


GS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	GS_TERRAIN_OUTPUT output;
	
	output.position =mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject),gmtxView),gmtxProjection);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.color = Fog(input.color,input.position);
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;
	output.normalW = normalize(mul(input.normal,gmtxGameObject));
	

	return(output);
}

[maxvertexcount(8)]
void GSTerrain(triangle GS_TERRAIN_OUTPUT input[3], inout TriangleStream<GS_TERRAIN_OUTPUT> outStream) {
	GS_TERRAIN_OUTPUT v[6];
	
	v[0].position = input[0].position;
	v[0].positionW = input[0].positionW;
	//v[0].normal = input[0].normal;
	v[0].color = input[0].color;
	v[0].uv0 = input[0].uv0;
	v[0].uv1 = input[0].uv1;
	v[0].normalW = input[0].normalW;
	
	v[1].position = input[1].position;
	v[1].positionW = input[1].positionW;
	v[1].color = input[1].color;
	v[1].uv0 = input[1].uv0;
	v[1].uv1 = input[1].uv1;
	v[1].normalW = input[1].normalW;
	

	v[2].position = input[2].position;
	v[2].positionW = input[2].positionW;
	v[2].color = input[2].color;
	v[2].uv0 = input[2].uv0;
	v[2].uv1 = input[2].uv1;
	v[2].normalW = input[2].normalW;
	


	v[3].position = (input[0].position + input[1].position) * 0.5f;
	v[3].positionW = (input[0].positionW + input[1].positionW) * 0.5f;
	v[3].color = (input[0].color + input[1].color + float4(1.0f, 0.0f, 0.0f, 1.0f)) / 3.0f;
	v[3].uv0 = (input[0].uv0 + input[1].uv0) *0.5f;
	v[3].uv1 = (input[0].uv1 + input[1].uv1) *0.5f;
	v[3].normalW = normalize(input[0].normalW + input[1].normalW);
	

	v[4].position = (input[1].position + input[2].position) * 0.5f;
	v[4].positionW = (input[1].positionW + input[2].positionW) * 0.5f;
	v[4].color = (input[1].color + input[2].color + float4(0.0f, 1.0f, 0.0f, 1.0f)) / 3.0f;
	v[4].uv0 = (input[1].uv0 + input[2].uv0) *0.5f;
	v[4].uv1 = (input[1].uv1 + input[2].uv1) *0.5f;
	v[4].normalW = normalize(input[1].normalW + input[2].normalW);



	v[5].position = (input[0].position + input[2].position)*0.5f;
	v[5].positionW = (input[0].positionW + input[2].positionW) * 0.5f;
	v[5].color = (input[0].color + input[2].color + float4(0.0f, 0.0f, 1.0f, 1.0f)) / 3.0f;
	v[5].uv0 = (input[0].uv0 + input[2].uv0) *0.5f;
	v[5].uv1 = (input[0].uv1 + input[2].uv1) *0.5f;
	v[5].normalW = normalize(input[0].normalW + input[2].normalW);


	
	outStream.Append(v[0]);
	outStream.Append(v[3]);
	outStream.Append(v[5]);
	outStream.Append(v[4]);
	outStream.Append(v[2]);
	outStream.RestartStrip();
	outStream.Append(v[3]);
	outStream.Append(v[1]);
	outStream.Append(v[4]);


}


float4 PSTerrain(GS_TERRAIN_OUTPUT input) : SV_TARGET
{


	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gSamplerState, input.uv0);

	
	
	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gSamplerState, input.uv1);
	
	
	float4 cColor=cBaseTexColor + cDetailTexColor;

	
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 CFog = Fog(cColor, input.positionW);

	return(lerp(CFog,cIllumination,0.5f));

	
}
///////////////////////////////////////////////////////////////////////////

Texture2D gtxtFlowerBillboardTexture : register(t8);

struct INSTANCEDGAMEOBJECTINFO
{
	matrix	m_mtxGameObject :WORLDMATRIX;
	//float2	uv :INSTANCETEXCOORD;
};
StructuredBuffer<INSTANCEDGAMEOBJECTINFO> gGameObjectInfos : register(t7);
struct VS_INSTANCING_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	//float4x4 mtxTransform : WORLDMATRIX;
	//float4 instanceColor : INSTANCECOLOR;

};
struct VS_INSTANCING_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_INSTANCING_OUTPUT VSInstancing(VS_INSTANCING_INPUT input, uint nInstanceID :SV_InstanceID)
{
	VS_INSTANCING_OUTPUT output;


	output.position = mul(mul(mul(float4(input.position, 1.0f),
		gGameObjectInfos[nInstanceID].m_mtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;
	return(output);
}

float4 PSInstancing(VS_INSTANCING_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtFlowerBillboardTexture.Sample(gSamplerState, input.uv);

	if (cColor.a < 0.3f) {
		discard;
	}

	return(cColor);
}

/////////////////////////////////////////////
//
Texture2DArray gtxtTreeBillboardArray : register(t9);
Texture2D gtxtTreeNormal01 : register(t10);   //t10
Texture2D gtxtTreeNormal02 : register(t11);  //t11
Texture2D gtxtTreeNormal03 : register(t12);  //t12


struct VS_IN
{ 
	float3 posW : POSITION;
	float2 sizeW : SIZE;
};

struct VS_OUT
{
	float3 centerW : POSITION;
	float2 sizeW : SIZE;
};

struct GS_OUT {
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
	uint primID : SV_PrimitiveID;
};

VS_OUT VSBillboard(VS_IN input, uint nInstanceID :SV_InstanceID)
{
	VS_OUT output;
	
	//float4 v = mul(float4(input.posW, 1.0f), gGameObjectInfos[nInstanceID].m_mtxGameObject)



	output.centerW = input.posW;
	
	output.sizeW = input.sizeW;

	return (output);
}

[maxvertexcount(4)]
void GSBillboard(point VS_OUT input[1], uint primID : SV_PrimitiveID, inout TriangleStream<GS_OUT> outstream)
{
	float3 vUp = float3(0.0f, 1.0f, 0.0f);
	float3 vLook = gvCameraPosition.xyz - input[0].centerW;
	vLook = normalize(vLook);

	float3 vRight = cross(vUp, vLook);

	float fHalfW = input[0].sizeW.x * 0.5f;
	float fHalfH = input[0].sizeW.y * 0.5f;

	float4 pVertices[4];

	pVertices[0] = float4(input[0].centerW + fHalfW * vRight - fHalfH * vUp, 1.0f);
	pVertices[1] = float4(input[0].centerW + fHalfW * vRight + fHalfH * vUp, 1.0f);
	pVertices[2] = float4(input[0].centerW - fHalfW * vRight - fHalfH * vUp, 1.0f);
	pVertices[3] = float4(input[0].centerW - fHalfW * vRight + fHalfH * vUp, 1.0f);

	float2 pUVs[4] = { float2(0.0f,1.0f),float2(0.0f,0.0f),float2(1.0f,1.0f),float2(1.0f,0.0f) };
	float4 pColors[4] = { float4(1.0f,0.0f,0.0f,1.0f),float4(0.0f,1.0f,0.0f,1.0f),float4(0.0f,0.0f,1.0f,1.0f),float4(1.0f,1.0f,1.0f,1.0f) };

	GS_OUT output;
	//[unroll]
	for (int i = 0; i < 4; i++)
	{
		output.posW = pVertices[i].xyz;
		output.posH = mul(pVertices[i], gmtxViewProjection);
		output.normalW = vLook;
		output.tangentW = vRight;		//각 정점에서의 접선 월드변환
		output.uv = pUVs[i];
		output.color = pColors[i];
		output.primID = primID;
		outstream.Append(output);
	}	
}

float4 PSBillboard(GS_OUT input) :SV_Target
{
	float3 uvw = float3(input.uv,(input.primID % 3));
	//float3 uvwNormal = float3(input.uv, (input.primID % 3));

	float4 cColor = gtxtTreeBillboardArray.Sample(gSamplerState, uvw);
	//float3 cNormal = gtxtTreeNormal.Sample(gSamplerState,input.uv);

	uint primid = input.primID % 3;
	
	float3 cNormal = (float3)0;
	if (primid== 0) {
		cNormal = gtxtTreeNormal01.Sample(gSamplerState, input.uv);
	}
	else if (primid== 1)
	{
		cNormal = gtxtTreeNormal02.Sample(gSamplerState, input.uv);
	}
	else if (primid == 2)
	{
		cNormal = gtxtTreeNormal03.Sample(gSamplerState, input.uv);
	}
	
	float3 vNormal = 2.0f * cNormal - 1.0f;			//[0,1] -> [-1,1]
	
	float3 N = normalize(input.normalW);
	float3 T = normalize(input.tangentW);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float3 normalW = mul(vNormal, TBN);

	if (cColor.a < 0.3f)
	{
		discard;
	}



	float4 cIllumination = Lighting(input.posW, normalW);
	float4 cFogColor = Fog(cColor, input.posW);

	//float4 cColor = input.color;
	//float4 cColor = input.color;
	//float4 CFog = Fog(cColor, input.position);
	return(lerp(cFogColor,cIllumination,0.3f));
}
