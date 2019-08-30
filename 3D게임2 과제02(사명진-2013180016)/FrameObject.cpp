#include "stdafx.h"
#include "FrameObject.h"
#include "Shader.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMaterialColors::CMaterialColors(MATERIALLOADINFO *pMaterialInfo)
{
	m_xmf4Diffuse = pMaterialInfo->m_xmf4AlbedoColor;
	m_xmf4Specular = pMaterialInfo->m_xmf4SpecularColor; //(r,g,b,a=power)
	m_xmf4Specular.w = (pMaterialInfo->m_fGlossiness * 255.0f);
	m_xmf4Emissive = pMaterialInfo->m_xmf4EmissiveColor;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CShader *CFrameMaterial::m_pIlluminatedShader = NULL;

CFrameMaterial::CFrameMaterial()
{

}

CFrameMaterial::~CFrameMaterial()
{
	if (m_pMaterialColors) m_pMaterialColors->Release();
}

void CFrameMaterial::SetShader(CShader *pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

void CFrameMaterial::SetMaterialColors(CMaterialColors *pMaterialColors)
{
	if (m_pMaterialColors) m_pMaterialColors->Release();
	m_pMaterialColors = pMaterialColors;
	if (m_pMaterialColors) m_pMaterialColors->AddRef();
}

void CFrameMaterial::PrepareShaders(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	m_pIlluminatedShader = new CIlluminatedShader();
	m_pIlluminatedShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	m_pIlluminatedShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}
void CFrameMaterial::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRoot32BitConstants(20, 4, &(m_pMaterialColors->m_xmf4Ambient), 16);
	pd3dCommandList->SetGraphicsRoot32BitConstants(20, 4, &(m_pMaterialColors->m_xmf4Diffuse), 20);
	pd3dCommandList->SetGraphicsRoot32BitConstants(20, 4, &(m_pMaterialColors->m_xmf4Specular), 24);
	pd3dCommandList->SetGraphicsRoot32BitConstants(20, 4, &(m_pMaterialColors->m_xmf4Emissive), 28);
}

/////////////////////////////


CFrameObject::CFrameObject()
{
	m_xmf4x4Transform = Matrix4x4::Identity();
	m_xmf4x4World = Matrix4x4::Identity();
}

CFrameObject::~CFrameObject()
{
	if (m_pMesh) m_pMesh->Release();

	if (m_nMaterials > 0)
	{
		for (int i = 0; i < m_nMaterials; i++)
		{
			if (m_ppMaterials[i]) m_ppMaterials[i]->Release();
		}
	}
	if (m_ppMaterials) delete[] m_ppMaterials;

	if (m_pSibling) delete m_pSibling;
	if (m_pChild) delete m_pChild;
}

void CFrameObject::SetChild(CFrameObject *pChild)
{
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}
	if (pChild) pChild->m_pParent = this;
}

void CFrameObject::SetMesh(CFileMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void CFrameObject::SetShader(CShader *pShader)
{
	m_nMaterials = 1;
	m_ppMaterials = new CFrameMaterial*[m_nMaterials];
	m_ppMaterials[0] = new CFrameMaterial();
	m_ppMaterials[0]->SetShader(pShader);
}

void CFrameObject::SetShader(int nMaterial, CShader *pShader)
{
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->SetShader(pShader);
}

void CFrameObject::SetMaterial(int nMaterial, CFrameMaterial *pMaterial)
{
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->Release();
	m_ppMaterials[nMaterial] = pMaterial;
	if (m_ppMaterials[nMaterial]) m_ppMaterials[nMaterial]->AddRef();
}

void CFrameObject::Animate(float fTimeElapsed, XMFLOAT4X4 *pxmf4x4Parent)
{

	if (m_pSibling) m_pSibling->Animate(fTimeElapsed, pxmf4x4Parent);
	if (m_pChild) m_pChild->Animate(fTimeElapsed, &m_xmf4x4World);
}

CFrameObject *CFrameObject::FindFrame(char *pstrFrameName)
{
	CFrameObject *pFrameObject = NULL;
	if (!strncmp(m_pstrFrameName, pstrFrameName, strlen(pstrFrameName))) return(this);

	if (m_pSibling) if (pFrameObject = m_pSibling->FindFrame(pstrFrameName)) return(pFrameObject);
	if (m_pChild) if (pFrameObject = m_pChild->FindFrame(pstrFrameName)) return(pFrameObject);

	return(NULL);
}

void CFrameObject::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	OnPrepareRender();

	UpdateShaderVariable(pd3dCommandList, &m_xmf4x4World);

	if (m_nMaterials > 0)
	{
		for (int i = 0; i < m_nMaterials; i++)
		{
			if (m_ppMaterials[i])
			{
				if (m_ppMaterials[i]->m_pShader) m_ppMaterials[i]->m_pShader->Render(pd3dCommandList, pCamera);
				m_ppMaterials[i]->UpdateShaderVariable(pd3dCommandList);
			}

			if (m_pMesh) m_pMesh->Render(pd3dCommandList, i);
		}
	}
	if (m_pSibling) m_pSibling->Render(pd3dCommandList, pCamera);
	if (m_pChild) m_pChild->Render(pd3dCommandList, pCamera);
}

void CFrameObject::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CFrameObject::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
}

void CFrameObject::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(20, 16, &xmf4x4World, 0);
}

void CFrameObject::UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, CFrameMaterial *pMaterial)
{
	if (pMaterial && pMaterial->m_pMaterialColors)
	{
		pd3dCommandList->SetGraphicsRoot32BitConstants(20, 4, &(pMaterial->m_pMaterialColors->m_xmf4Ambient), 16);
		pd3dCommandList->SetGraphicsRoot32BitConstants(20, 4, &(pMaterial->m_pMaterialColors->m_xmf4Diffuse), 20);
		pd3dCommandList->SetGraphicsRoot32BitConstants(20, 4, &(pMaterial->m_pMaterialColors->m_xmf4Specular), 24);
		pd3dCommandList->SetGraphicsRoot32BitConstants(20, 4, &(pMaterial->m_pMaterialColors->m_xmf4Emissive), 28);
	}
}

void CFrameObject::ReleaseShaderVariables()
{
}

void CFrameObject::ReleaseUploadBuffers()
{
	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();

	if (m_pSibling) m_pSibling->ReleaseUploadBuffers();
	if (m_pChild) m_pChild->ReleaseUploadBuffers();
}

void CFrameObject::UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent)
{
	m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(m_xmf4x4Transform, *pxmf4x4Parent) : m_xmf4x4Transform;

	if (m_pSibling) m_pSibling->UpdateTransform(pxmf4x4Parent);
	if (m_pChild) m_pChild->UpdateTransform(&m_xmf4x4World);
}

void CFrameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4Transform._41 = x;
	m_xmf4x4Transform._42 = y;
	m_xmf4x4Transform._43 = z;

	UpdateTransform(NULL);
}

void CFrameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

void CFrameObject::SetScale(float x, float y, float z)
{
	XMMATRIX mtxScale = XMMatrixScaling(x, y, z);
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxScale, m_xmf4x4Transform);

	UpdateTransform(NULL);
}

XMFLOAT3 CFrameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

XMFLOAT3 CFrameObject::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33)));
}

XMFLOAT3 CFrameObject::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23)));
}

XMFLOAT3 CFrameObject::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13)));
}

void CFrameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	CFrameObject::SetPosition(xmf3Position);
}

void CFrameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	CFrameObject::SetPosition(xmf3Position);
}

void CFrameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();

	XMFLOAT3 xmf3Look = GetLook();
	
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	CFrameObject::SetPosition(xmf3Position);
}

void CFrameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4Transform);

	UpdateTransform(NULL);
}

void CFrameObject::Rotate(XMFLOAT3 *pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4Transform);

	UpdateTransform(NULL);
}

void CFrameObject::Rotate(XMFLOAT4 *pxmf4Quaternion)
{
	XMMATRIX mtxRotate = XMMatrixRotationQuaternion(XMLoadFloat4(pxmf4Quaternion));
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4Transform);

	UpdateTransform(NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define _WITH_DEBUG_FRAME_HIERARCHY

CMeshLoadInfo *CFrameObject::LoadMeshInfoFromFile(FILE *pInFile)
{
	char pstrToken[64] = { '\0' };
	BYTE nStrLength = 0;

	int nPositions = 0, nColors = 0, nNormals = 0, nTangents = 0, nBiNormals = 0, nTextureCoords = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	CMeshLoadInfo *pMeshInfo = new CMeshLoadInfo;

	UINT nReads = (UINT)::fread(&(pMeshInfo->m_nVertices), sizeof(int), 1, pInFile);
	nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
	nReads = (UINT)::fread(pMeshInfo->m_pstrMeshName, sizeof(char), nStrLength, pInFile);
	pMeshInfo->m_pstrMeshName[nStrLength] = '\0';

	for (; ; )
	{
		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
		pstrToken[nStrLength] = '\0';

		if (!strcmp(pstrToken, "<Bounds>:"))
		{
			nReads = (UINT)::fread(&(pMeshInfo->m_xmf3AABBCenter), sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&(pMeshInfo->m_xmf3AABBExtents), sizeof(XMFLOAT3), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Positions>:"))
		{
			nReads = (UINT)::fread(&nPositions, sizeof(int), 1, pInFile);
			if (nPositions > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_POSITION;
				pMeshInfo->m_pxmf3Positions = new XMFLOAT3[nPositions];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf3Positions, sizeof(XMFLOAT3), nPositions, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Colors>:"))
		{
			nReads = (UINT)::fread(&nColors, sizeof(int), 1, pInFile);
			if (nColors > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_COLOR;
				pMeshInfo->m_pxmf4Colors = new XMFLOAT4[nColors];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf4Colors, sizeof(XMFLOAT4), nColors, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords0>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_TEXTURE_COORD0;
				pMeshInfo->m_pxmf2TextureCoords0 = new XMFLOAT2[nTextureCoords];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf2TextureCoords0, sizeof(XMFLOAT2), nTextureCoords, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords1>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_TEXTURE_COORD1;
				pMeshInfo->m_pxmf2TextureCoords1 = new XMFLOAT2[nTextureCoords];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf2TextureCoords1, sizeof(XMFLOAT2), nTextureCoords, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Normals>:"))
		{
			nReads = (UINT)::fread(&nNormals, sizeof(int), 1, pInFile);
			if (nNormals > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_NORMAL;
				pMeshInfo->m_pxmf3Normals = new XMFLOAT3[nNormals];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf3Normals, sizeof(XMFLOAT3), nNormals, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Tangents>:"))
		{
			nReads = (UINT)::fread(&nTangents, sizeof(int), 1, pInFile);
			if (nTangents > 0)
			{
				pMeshInfo->m_nType |= VERTEXT_TANGENT;
				pMeshInfo->m_pxmf3Tangents = new XMFLOAT3[nTangents];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf3Tangents, sizeof(XMFLOAT3), nTangents, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<BiNormals>:"))
		{
			nReads = (UINT)::fread(&nBiNormals, sizeof(int), 1, pInFile);
			if (nBiNormals > 0)
			{
				pMeshInfo->m_pxmf3BiNormals = new XMFLOAT3[nBiNormals];
				nReads = (UINT)::fread(pMeshInfo->m_pxmf3BiNormals, sizeof(XMFLOAT3), nBiNormals, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Indices>:"))
		{
			nReads = (UINT)::fread(&nIndices, sizeof(int), 1, pInFile);
			if (nIndices > 0)
			{
				pMeshInfo->m_pnIndices = new UINT[nIndices];
				nReads = (UINT)::fread(pMeshInfo->m_pnIndices, sizeof(int), nIndices, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<SubMeshes>:"))
		{
			nReads = (UINT)::fread(&(pMeshInfo->m_nSubMeshes), sizeof(int), 1, pInFile);
			if (pMeshInfo->m_nSubMeshes > 0)
			{
				pMeshInfo->m_pnSubSetIndices = new int[pMeshInfo->m_nSubMeshes];
				pMeshInfo->m_ppnSubSetIndices = new UINT*[pMeshInfo->m_nSubMeshes];
				for (int i = 0; i < pMeshInfo->m_nSubMeshes; i++)
				{
					nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
					nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
					pstrToken[nStrLength] = '\0';
					if (!strcmp(pstrToken, "<SubMesh>:"))
					{
						int nIndex = 0;
						nReads = (UINT)::fread(&nIndex, sizeof(int), 1, pInFile);
						nReads = (UINT)::fread(&(pMeshInfo->m_pnSubSetIndices[i]), sizeof(int), 1, pInFile);
						if (pMeshInfo->m_pnSubSetIndices[i] > 0)
						{
							pMeshInfo->m_ppnSubSetIndices[i] = new UINT[pMeshInfo->m_pnSubSetIndices[i]];
							nReads = (UINT)::fread(pMeshInfo->m_ppnSubSetIndices[i], sizeof(int)*pMeshInfo->m_pnSubSetIndices[i], 1, pInFile);
						}

					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Mesh>"))
		{
			break;
		}
	}
	return(pMeshInfo);
}

MATERIALSLOADINFO *CFrameObject::LoadMaterialsInfoFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, FILE *pInFile)
{
	char pstrToken[64] = { '\0' };

	int nMaterial = 0;
	BYTE nStrLength = 0;

	MATERIALSLOADINFO *pMaterialsInfo = new MATERIALSLOADINFO;

	UINT nReads = (UINT)::fread(&(pMaterialsInfo->m_nMaterials), sizeof(int), 1, pInFile);
	pMaterialsInfo->m_pMaterials = new MATERIALLOADINFO[pMaterialsInfo->m_nMaterials];

	for (; ; )
	{
		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
		pstrToken[nStrLength] = '\0';

		if (!strcmp(pstrToken, "<Material>:"))
		{
			nReads = (UINT)::fread(&nMaterial, sizeof(int), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<AlbedoColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4AlbedoColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<EmissiveColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4EmissiveColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularColor>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_xmf4SpecularColor), sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<Glossiness>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_fGlossiness), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Smoothness>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_fSmoothness), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Metallic>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_fSpecularHighlight), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularHighlight>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_fMetallic), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<GlossyReflection>:"))
		{
			nReads = (UINT)::fread(&(pMaterialsInfo->m_pMaterials[nMaterial].m_fGlossyReflection), sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<AlbedoMap>:"))
		{
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
			pstrToken[nStrLength] = '\0';
			if (strcmp(pstrToken, "null")) pMaterialsInfo->m_pMaterials[nMaterial].m_nType |= MATERIAL_ALBEDO_MAP;
		}
		else if (!strcmp(pstrToken, "<SpecularMap>:"))
		{
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
			pstrToken[nStrLength] = '\0';
			if (strcmp(pstrToken, "null")) pMaterialsInfo->m_pMaterials[nMaterial].m_nType |= MATERIAL_SPECULAR_MAP;
		}
		else if (!strcmp(pstrToken, "<MetallicMap>:"))
		{
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
			pstrToken[nStrLength] = '\0';
			if (strcmp(pstrToken, "null")) pMaterialsInfo->m_pMaterials[nMaterial].m_nType |= MATERIAL_METALLIC_MAP;
		}
		else if (!strcmp(pstrToken, "<NormalMap>:"))
		{
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
			pstrToken[nStrLength] = '\0';
			if (strcmp(pstrToken, "null")) pMaterialsInfo->m_pMaterials[nMaterial].m_nType |= MATERIAL_NORMAL_MAP;
		}
		else if (!strcmp(pstrToken, "<EmissionMap>:"))
		{
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
			pstrToken[nStrLength] = '\0';
			if (strcmp(pstrToken, "null")) pMaterialsInfo->m_pMaterials[nMaterial].m_nType |= MATERIAL_EMISSION_MAP;
		}
		else if (!strcmp(pstrToken, "<DetailAlbedoMap>:"))
		{
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
			pstrToken[nStrLength] = '\0';
			if (strcmp(pstrToken, "null")) pMaterialsInfo->m_pMaterials[nMaterial].m_nType |= MATERIAL_DETAIL_ALBEDO_MAP;
		}
		else if (!strcmp(pstrToken, "<DetailNormalMap>:"))
		{
			UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
			pstrToken[nStrLength] = '\0';
			if (strcmp(pstrToken, "null")) pMaterialsInfo->m_pMaterials[nMaterial].m_nType |= MATERIAL_DETAIL_NORMAL_MAP;
		}
		else if (!strcmp(pstrToken, "</Materials>"))
		{
			break;
		}
	}
	return(pMaterialsInfo);
}

CFrameObject *CFrameObject::LoadFrameHierarchyFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, FILE *pInFile)
{
	char pstrToken[64] = { '\0' };

	BYTE nStrLength = 0;
	UINT nReads = 0;

	int nFrame = 0, nTextures = 0;

	CFrameObject *pGameObject = NULL;

	for (; ; )
	{
		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
		pstrToken[nStrLength] = '\0';

		if (!strcmp(pstrToken, "<Frame>:"))
		{
			pGameObject = new CFrameObject();

			nReads = (UINT)::fread(&nFrame, sizeof(int), 1, pInFile);
			nReads = (UINT)::fread(&nTextures, sizeof(int), 1, pInFile);

			nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
			nReads = (UINT)::fread(pGameObject->m_pstrFrameName, sizeof(char), nStrLength, pInFile);
			pGameObject->m_pstrFrameName[nStrLength] = '\0';
		}
		else if (!strcmp(pstrToken, "<Transform>:"))
		{
			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
			XMFLOAT4 xmf4Rotation;
			nReads = (UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
			nReads = (UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion
		}
		else if (!strcmp(pstrToken, "<TransformMatrix>:"))
		{
			nReads = (UINT)::fread(&pGameObject->m_xmf4x4Transform, sizeof(float), 16, pInFile);
		}
		else if (!strcmp(pstrToken, "<Mesh>:"))
		{
			CMeshLoadInfo *pMeshInfo = pGameObject->LoadMeshInfoFromFile(pInFile);
			if (pMeshInfo)
			{
				CFileMesh *pMesh = NULL;
				if (pMeshInfo->m_nType & VERTEXT_NORMAL)
				{
					pMesh = new CMeshIlluminatedFromFile(pd3dDevice, pd3dCommandList, pMeshInfo);
				}
				if (pMesh) pGameObject->SetMesh(pMesh);
				delete pMeshInfo;
			}
		}
		else if (!strcmp(pstrToken, "<Materials>:"))
		{
			MATERIALSLOADINFO *pMaterialsInfo = pGameObject->LoadMaterialsInfoFromFile(pd3dDevice, pd3dCommandList, pInFile);
			if (pMaterialsInfo && (pMaterialsInfo->m_nMaterials > 0))
			{
				pGameObject->m_nMaterials = pMaterialsInfo->m_nMaterials;
				pGameObject->m_ppMaterials = new CFrameMaterial*[pMaterialsInfo->m_nMaterials];

				UINT nMaterialTextures = 0;

				for (int i = 0; i < pMaterialsInfo->m_nMaterials; i++)
				{
					pGameObject->m_ppMaterials[i] = NULL;

					CFrameMaterial *pMaterial = new CFrameMaterial();

					CMaterialColors *pMaterialColors = new CMaterialColors(&pMaterialsInfo->m_pMaterials[i]);
					pMaterial->SetMaterialColors(pMaterialColors);

					if (pGameObject->GetMeshType() & VERTEXT_NORMAL) pMaterial->SetIlluminatedShader();

					pGameObject->SetMaterial(i, pMaterial);
				}
			}
		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = 0;
			nReads = (UINT)::fread(&nChilds, sizeof(int), 1, pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					CFrameObject *pChild = CFrameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pInFile);
					if (pChild) pGameObject->SetChild(pChild);
#ifdef _WITH_DEBUG_FRAME_HIERARCHY
					TCHAR pstrDebug[256] = { 0 };
					_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pChild, pGameObject);
					OutputDebugString(pstrDebug);
#endif
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
	return(pGameObject);
}

void CFrameObject::PrintFrameInfo(CFrameObject *pGameObject, CFrameObject *pParent)
{
	TCHAR pstrDebug[256] = { 0 };

	_stprintf_s(pstrDebug, 256, _T("(Frame: %p) (Parent: %p)\n"), pGameObject, pParent);
	OutputDebugString(pstrDebug);

	if (pGameObject->m_pSibling) CFrameObject::PrintFrameInfo(pGameObject->m_pSibling, pParent);
	if (pGameObject->m_pChild) CFrameObject::PrintFrameInfo(pGameObject->m_pChild, pGameObject);
}

CFrameObject *CFrameObject::LoadGeometryFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, char *pstrFileName)
{
	FILE *pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	CFrameObject *pGameObject = CFrameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pInFile);

#ifdef _WITH_DEBUG_FRAME_HIERARCHY
	TCHAR pstrDebug[256] = { 0 };
	_stprintf_s(pstrDebug, 256, _T("Frame Hierarchy\n"));
	OutputDebugString(pstrDebug);

	CFrameObject::PrintFrameInfo(pGameObject, NULL);
#endif

	return(pGameObject);
}


////////////////////////////////////////////////////////////////////////////////////
CApacheObject::CApacheObject()
	:CFrameObject()
{
}

CApacheObject::~CApacheObject()
{
}

void CApacheObject::OnInitialize()
{
	m_pMainRotorFrame = FindFrame("rotor");
}

void CApacheObject::Animate(float fTimeElapsed, XMFLOAT4X4 *pxmf4x4Parent)
{
	if (m_pMainRotorFrame)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationY(XMConvertToRadians(360.0f * 2.0f) * fTimeElapsed);
		m_pMainRotorFrame->m_xmf4x4Transform = Matrix4x4::Multiply(xmmtxRotate, m_pMainRotorFrame->m_xmf4x4Transform);
	}

	CFrameObject::Animate(fTimeElapsed, pxmf4x4Parent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
CSuperCobraObject::CSuperCobraObject()
	:CFrameObject()
{
}

CSuperCobraObject::~CSuperCobraObject()
{
}

void CSuperCobraObject::OnInitialize()
{
	m_pMainRotorFrame = FindFrame("MainRotor");
	m_pTailRotorFrame = FindFrame("TailRotor");
}

void CSuperCobraObject::Animate(float fTimeElapsed, XMFLOAT4X4 *pxmf4x4Parent)
{
	if (m_pMainRotorFrame)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationY(XMConvertToRadians(360.0f * 2.0f) * fTimeElapsed);
		m_pMainRotorFrame->m_xmf4x4Transform = Matrix4x4::Multiply(xmmtxRotate, m_pMainRotorFrame->m_xmf4x4Transform);
	}
	if (m_pTailRotorFrame)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationX(XMConvertToRadians(360.0f * 4.0f) * fTimeElapsed);
		m_pTailRotorFrame->m_xmf4x4Transform = Matrix4x4::Multiply(xmmtxRotate, m_pTailRotorFrame->m_xmf4x4Transform);
	}

	CFrameObject::Animate(fTimeElapsed, pxmf4x4Parent);
}

