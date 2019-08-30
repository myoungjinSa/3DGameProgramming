
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
	float3		gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)						//GameObject (서술자 테이블)
{
	matrix		gmtxGameObject;
	//uint		gnObjectID	: packoffset(c4);
	uint		gnMaterial;
	uint		gnTimeElapsed;
	uint		gnObjectID;
};



////플레이어의 ...
cbuffer cbFrameObjectInfo : register(b3)
{
	matrix		gmtxFrameObject : packoffset(c0);
	MATERIAL	gMaterial : packoffset(c4);
	uint		gnTexturesMask : packoffset(c8);
}



#include "Light.hlsl"

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//struct VS_DIFFUSED_INPUT
//{
//	float3 position : POSITION;
//	float4 color : COLOR;
//};
//
//struct VS_DIFFUSED_OUTPUT
//{
//	float4 position : SV_POSITION;
//	float4 color : COLOR;
//};
//
//VS_DIFFUSED_OUTPUT VSPlayer(VS_DIFFUSED_INPUT input)
//{
//	VS_DIFFUSED_OUTPUT output;
//
//	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxPlayerWorld), gmtxView), gmtxProjection);
//	output.color = input.color;
//
//	return(output);
//}
//
//float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
//{
//	return(input.color);
//}

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

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView),gmtxProjection);
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
	return(cColor);
}



float4 PSGrassTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	
	float4 cColor = gtxtGrassBillboardTexture.Sample(gSamplerState,input.uv);
	
	

	if (cColor.a <0.4f)
	{
		discard;
	}
	return (cColor);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _WITH_VERTEX_LIGHTING
struct VS_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};
struct VS_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	//	nointerpolation float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
#ifdef _WITH_VERTEX_LIGHTING
	float4 color : COLOR;
#endif
};

VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLighting(VS_TEXTURED_LIGHTING_INPUT input)
{
	VS_TEXTURED_LIGHTING_OUTPUT output;

	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;
#ifdef _WITH_VERTEX_LIGHTING
	output.normalW = normalize(output.normalW);
	output.color = Lighting(output.positionW, output.normalW);
#endif
	return(output);
}

float4 PSTexturedLighting(VS_TEXTURED_LIGHTING_OUTPUT input, uint nPrimitiveID : SV_PrimitiveID) : SV_TARGET
{
	float4 cColor = gtxtBillboardTexture.Sample(gSamplerState, input.uv);
#ifdef _WITH_VERTEX_LIGHTING
	float4 cIllumination = input.color;
#else
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
#endif
	return(cColor * cIllumination);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

float4 PSSkyBox(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtSkyBoxTexture.Sample(gSamplerState, input.uv);

	return(cColor);
}



////////////////////////////////////////////////////////////
Texture2D gtxtMissileTexture : register(t17);
Texture2D gtxtExplosionTexture : register(t18);


//폭발상태가 아닐때의 미사일 ps셰이더
float4 PSTexturedMissile(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtMissileTexture.Sample(gSamplerState, input.uv);

	//input.normalW = normalize(input.normalW);
	//float4 cIllumination = Lighting(input.positionW, input.normalW);

	//return (lerp(cColor, cIllumination, 0.5f));
	return (cColor);
}

////////////////////////////////////////////////////////////////////////////////////
//폭발 상태일때의 미사일 셰이더 과정


float4 PSExplosion(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float2 texcoord = input.uv;
	texcoord.x = texcoord.x / 6.0f + (1.0f / 6.0f) *gnTimeElapsed;
	//texcoord.y = texcoord.y * 0.2f + (1.0f /5.0f) *2;


	float4 cColor = gtxtExplosionTexture.Sample(gSamplerState,texcoord);


	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////
Texture2D gtxtSmoke : register(t21);
float4 PSSmoke(VS_TEXTURED_OUTPUT input ):SV_TARGET
{
	float2 texcoord = input.uv;
	texcoord.x = texcoord.x / 6.0f + (1.0f / 6.0f) *gnTimeElapsed;
	texcoord.y = texcoord.y / 6.0f;
	float4 cColor = gtxtSmoke.Sample(gSamplerState, texcoord);

	return (cColor);
}

/////////////////////////////////////////////////////////////////////////////////////////



VS_TEXTURED_OUTPUT VSPostProcessing(uint nVertexID : SV_VertexID)
{
	//if (nVertexID == 0) return(float4(-1.0f, -0.5f, 0.0f, 1.0f));
	//if (nVertexID == 1) return(float4(-0.5f, -0.5f, 0.0f, 1.0f));
	//if (nVertexID == 2) return(float4(-0.5f, -1.0f, 0.0f, 1.0f));
	//if (nVertexID == 3) return(float4(-1.0f, -0.5f, 0.0f, 1.0f));
	//if (nVertexID == 4) return(float4(-0.5f, -0.5f, 0.0f, 1.0f));
	//if (nVertexID == 5) return(float4(-1.0f, -1.0f, 0.0f, 1.0f));
	VS_TEXTURED_OUTPUT output;

	//return(float4(0, 0, 0, 0));
	if (nVertexID == 0)
	{
		output.position = float4(-1.0f, -0.5f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);
		
	}
	if (nVertexID == 1)
	{
		output.position = float4(-0.5f, -0.5f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 0.0f);
		
	}
	if (nVertexID == 2)
	{
		output.position = float4(-0.5f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);
		
	}
	if (nVertexID == 3)
	{
		output.position = float4(-1.0f, -0.5f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);
		
	}
	if (nVertexID == 4)
	{
		output.position = float4(-0.5f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);
		
	}
	if (nVertexID == 5)
	{
		output.position = float4(-1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 1.0f);
		
	}

	return (output);

}

Texture2D<float4> gtxtScene : register(t20);

float4 PSPostProcessing(VS_TEXTURED_OUTPUT input) : SV_Target
{
	float4 cColor = gtxtScene.Sample(gSamplerState,input.uv);
	//float3 cColor = gtxtMissileTexture[int2(position.xy)].rgb;

	//return(cColor);
	return(cColor);

}

struct PS_MULTIPLE_RENDER_TARGETS_OUTPUT
{
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
};



VS_TEXTURED_OUTPUT VSToonShader(uint nVertexID : SV_VertexID)
{
	
	VS_TEXTURED_OUTPUT output;

	//return(float4(0, 0, 0, 0));
	if (nVertexID == 0)
	{
		output.position = float4(-1.0f, 1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 1)
	{
		output.position = float4(1.0f, 1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 0.0f);

	}
	if (nVertexID == 2)
	{
		output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);

	}
	if (nVertexID == 3)
	{
		output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 4)
	{
		output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);

	}
	if (nVertexID == 5)
	{
		output.position = float4(-1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 1.0f);

	}

	return (output);

}
///////////////////////////////////////////////////////////////////////
//
Texture2D<float4> gBaseMap : register(t0);
Texture2D<float4> gEdgeMap : register(t1);
SamplerState gsSamplerToonState : register(s0);
float4 PSToonShader(VS_TEXTURED_OUTPUT input) : SV_Target
{
	//float4 cColor = gtxtSceneColor.Sample(gSamplerState ,input.uv);

	//
	//float3 cDiffuse = saturate(cColor.xyz);

	//cDiffuse = ceil(cDiffuse * 5) / 5.0f;


	//return(float4(gtxtSceneColor[int2(position.xy)].rgb,1.0f)*float4(cDiffuse.xyz,1.0f));



	float4 cBaseTexture = gBaseMap.SampleLevel(gsSamplerToonState, input.uv, 0.0f);
	float4 cEdgeTexture = gEdgeMap.SampleLevel(gsSamplerToonState, input.uv, 0.0f);

	return cBaseTexture * cEdgeTexture;
}



////////////////////////////////////////////////////////////////////////


VS_TEXTURED_OUTPUT VSTexturedUI(VS_TEXTURED_INPUT input,uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;

	if (nVertexID == 0)
	{
		output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 1)
	{
		output.position = float4(+1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 0.0f);

	}
	if (nVertexID == 2)
	{
		output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);
		
	}
	if (nVertexID == 3)
	{
		output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);
		
	}
	if (nVertexID == 4)
	{
		output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);
		
	}
	if (nVertexID == 5)
	{
		output.position = float4(-1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 1.0f);
		
	}
	return (output);
	
}

Texture2D gtxtPilot : register(t19);

//SamplerState gSampler : register(s1);
float4 PSTexturedUI(VS_TEXTURED_OUTPUT input) :SV_Target
{


	
	float4 cColor = gtxtPilot.Sample(gSamplerState, input.uv);

	


	return(cColor);

}

VS_TEXTURED_OUTPUT VSTexturedMenuUI(VS_TEXTURED_INPUT input, uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;

	if (nVertexID == 0)
	{
		output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 1)
	{
		output.position = float4(+1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 0.0f);

	}
	if (nVertexID == 2)
	{
		output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);

	}
	if (nVertexID == 3)
	{
		output.position = float4(-1.0f, +1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 4)
	{
		output.position = float4(+1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);

	}
	if (nVertexID == 5)
	{
		output.position = float4(-1.0f, -1.0f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 1.0f);

	}
	return (output);

}

Texture2D gtxtGameMode : register(t23);
float4 PSTexturedMenuUI(VS_TEXTURED_OUTPUT input) :SV_Target
{

	float4 cColor = gtxtGameMode.Sample(gSamplerState, input.uv);


	return(cColor);
	
}

VS_TEXTURED_OUTPUT VSTexturedTargetUI(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;

	if (nVertexID == 0)
	{
		output.position = float4(-0.1f, +0.1f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 1)
	{
		output.position = float4(+0.1f, +0.1f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 0.0f);

	}
	if (nVertexID == 2)
	{
		output.position = float4(+0.1f, -0.1f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);

	}
	if (nVertexID == 3)
	{
		output.position = float4(-0.1f, +0.1f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 0.0f);

	}
	if (nVertexID == 4)
	{
		output.position = float4(+0.1f, -0.1f, 0.0f, 1.0f);
		output.uv = float2(1.0f, 1.0f);

	}
	if (nVertexID == 5)
	{
		output.position = float4(-0.1f, -0.1f, 0.0f, 1.0f);
		output.uv = float2(0.0f, 1.0f);

	}
	return (output);
}

Texture2D gtxtTarget : register(t22);
float4 PSTexturedTargetUI(VS_TEXTURED_OUTPUT input) :SV_Target
{
	float4 cColor = gtxtTarget.Sample(gSamplerState,input.uv);

	return (cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTerrainBaseTexture : register(t1);
Texture2D gtxtTerrainDetailTexture : register(t2);
Texture2D gtxtSandBaseTexture : register(t5);


struct VS_TERRAIN_LIGHTING_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 normal : NORMAL;
};

struct VS_TERRAIN_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 normalW : NORMAL;
};


VS_TERRAIN_LIGHTING_OUTPUT VSIlluminatedTerrain(VS_TERRAIN_LIGHTING_INPUT input)
{
	VS_TERRAIN_LIGHTING_OUTPUT output;
	
	output.position = mul(mul(mul(float4(input.position,1.0f),gmtxGameObject), gmtxView), gmtxProjection);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.color = input.color;
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;
	output.normalW = normalize(mul(input.normal, gmtxGameObject));
	return(output);
}





//float4 PSIlluminatedTerrain(VS_TERRAIN_LIGHTING_OUTPUT input) : SV_TARGET
//{
//
//
//	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gSamplerState, input.uv0);
//
//	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gSamplerState, input.uv1);
//	float4 cColor = /*input.color **/ cBaseTexColor + cDetailTexColor;	//기존 albedo * 텍스쳐 원본 으로 초록색 지형이나 빨간색 지형을 만들 수 있다.
//	//float4 cColor=saturate(input.color * 0.5f + (cBaseTexColor + cDetailTexColor)*0.5f);
//	float4 cIllumination = Lighting(input.positionW, input.normalW);
//
//	return(lerp(cColor,cIllumination,0.5f));
//
//}

float4 PSIlluminatedTerrain(VS_TERRAIN_LIGHTING_OUTPUT input) : SV_TARGET
{
	PS_MULTIPLE_RENDER_TARGETS_OUTPUT output;

	
	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gSamplerState, input.uv0);
	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gSamplerState, input.uv1);

	input.normalW = normalize(input.normalW);

	float4 cColor = saturate(input.color * 0.5f + (cBaseTexColor + cDetailTexColor)*0.5f);
	//cColor = cColor * input.color;
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	output.color = lerp(cColor,cIllumination,0.5f);

	output.normal.xyz = input.normalW.xyz * 0.5f + 0.5f;

	
	return (output.color);
	
}


/////////////////////////////////////////////////////////////////////////////

struct VS_WATER_TERRAIN_LIGHTING_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VS_WATER_TERRAIN_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
	float3 normalW : NORMAL;
};

VS_WATER_TERRAIN_LIGHTING_OUTPUT VSIlluminatedWater(VS_WATER_TERRAIN_LIGHTING_INPUT input)
{
	VS_WATER_TERRAIN_LIGHTING_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.color = input.color;
	output.uv = input.uv;	
	output.normalW = normalize(mul(input.normal, gmtxGameObject));
	return(output);
}

float4 PSIlluminatedWater(VS_WATER_TERRAIN_LIGHTING_OUTPUT input) : SV_TARGET
{
	PS_MULTIPLE_RENDER_TARGETS_OUTPUT output;


	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gSamplerState, input.uv);

	input.normalW = normalize(input.normalW);

	float4 cColor = saturate(input.color * 0.5f + cBaseTexColor *0.5f);
	//cColor = cColor * input.color;
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	output.color = lerp(cColor,cIllumination,0.5f);

	output.normal.xyz = input.normalW.xyz * 0.5f + 0.5f;


	return (output.color);

}


///////////////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////////////////////////


Texture2D gtxtAlbedoTexture : register(t9);
Texture2D gtxtSpecularTexture : register(t10);
Texture2D gtxtNormalTexture : register(t11);
Texture2D gtxtMetallicTexture : register(t12);
Texture2D gtxtEmissionTexture : register(t13);
Texture2D gtxtDetailAlbedoTexture : register(t14);
Texture2D gtxtDetailNormalTexture : register(t15);

struct VS_STANDARD_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VS_STANDARD_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGENT;
	float2 uv : TEXCOORD;
};

VS_STANDARD_OUTPUT VSStandard(VS_STANDARD_INPUT input)
{
	VS_STANDARD_OUTPUT output;

	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxFrameObject);
	output.normalW = mul(input.normal, (float3x3)gmtxFrameObject);
	output.tangentW = (float3)mul(float4(input.tangent, 1.0f), gmtxFrameObject);
	output.bitangentW = (float3)mul(float4(input.bitangent, 1.0f), gmtxFrameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}

float4 PSStandard(VS_STANDARD_OUTPUT input) : SV_TARGET
{
	float4 cAlbedoColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (gnTexturesMask & MATERIAL_ALBEDO_MAP) cAlbedoColor = gtxtAlbedoTexture.Sample(gSamplerState, input.uv);
	float4 cSpecularColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (gnTexturesMask & MATERIAL_SPECULAR_MAP) cSpecularColor = gtxtSpecularTexture.Sample(gSamplerState, input.uv);
	float4 cNormalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (gnTexturesMask & MATERIAL_NORMAL_MAP) cNormalColor = gtxtNormalTexture.Sample(gSamplerState, input.uv);
	float4 cMetallicColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (gnTexturesMask & MATERIAL_METALLIC_MAP) cMetallicColor = gtxtMetallicTexture.Sample(gSamplerState, input.uv);
	float4 cEmissionColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (gnTexturesMask & MATERIAL_EMISSION_MAP) cEmissionColor = gtxtEmissionTexture.Sample(gSamplerState, input.uv);

	float4 cIllumination = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 cColor = cAlbedoColor + cSpecularColor + cEmissionColor;
	if (gnTexturesMask & MATERIAL_NORMAL_MAP)
	{
		float3 normalW = input.normalW;
		float3x3 TBN = float3x3(normalize(input.tangentW), normalize(input.bitangentW), normalize(input.normalW));
		float3 vNormal = normalize(cNormalColor.rgb * 2.0f - 1.0f); //[0, 1] → [-1, 1]
		normalW = normalize(mul(vNormal, TBN));
		cIllumination = Lighting(input.positionW, normalW);
		return(lerp(cColor, cIllumination, 0.5f));
	}
	else
	{
		return(cColor);
	}
}



//////////////////////////////////////////////////////////////
Texture2DArray gtxtTreeBillboardArray : register(t16);
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
		output.posH = mul(mul(pVertices[i], gmtxView),gmtxProjection);
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

	float3 cNormal = (float3)1;						//빌보드는 항상 플레이어와 수직이므로

	float3 vNormal = 2.0f * cNormal - 1.0f;			//[0,1] -> [-1,1]

	float3 N = normalize(input.normalW);
	float3 T = normalize(input.tangentW);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float3 normalW = mul(vNormal, TBN);
	
	float4 cIllumination = Lighting(input.posW, normalW);

	return(lerp(cColor,cIllumination,0.3f));
}


VS_TEXTURED_OUTPUT VSMirror(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;


	return(output);
}





