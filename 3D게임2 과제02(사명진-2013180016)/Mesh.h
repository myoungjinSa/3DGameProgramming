//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------

#pragma once
#include <stdio.h>
#include <fstream>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CVertex
{
public:
    XMFLOAT3						m_xmf3Position;	

public:
	CVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); }
	CVertex(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
	~CVertex() { }
};


class CDiffusedVertex : public CVertex
{
public:
    XMFLOAT4						m_xmf4Diffuse;		

public:
	CDiffusedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); }
	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; }
	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse; }
	~CDiffusedVertex() { }
};

class CNormalDiffusedVertex : public CDiffusedVertex
{
public : 
	XMFLOAT3						m_xmf3Normal;

public:
	CNormalDiffusedVertex() {
		m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_xmf3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	CNormalDiffusedVertex(float x,float y, float z, XMFLOAT3 xmf3Normal) 
	{
		m_xmf3Position = XMFLOAT3(x, y, z);
		m_xmf3Normal = xmf3Normal;
	}
	CNormalDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Normal)
	{
		m_xmf3Position = xmf3Position;
		m_xmf3Normal = xmf3Normal;
	}
	~CNormalDiffusedVertex() { }
};

class CTexturedVertex : public CVertex
{
public:
	XMFLOAT2						m_xmf2TexCoord;

public:
	CTexturedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f); }
	CTexturedVertex(float x, float y, float z, XMFLOAT2 xmf2TexCoord) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf2TexCoord = xmf2TexCoord; }
	CTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2TexCoord = XMFLOAT2(0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf2TexCoord = xmf2TexCoord; }
	~CTexturedVertex() { }
};

class CDiffusedTexturedVertex : public CDiffusedVertex
{
public:
	XMFLOAT2						m_xmf2TexCoord;
	XMFLOAT2						m_xmf2TexCoord2;
	

public:
	CDiffusedTexturedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f); }
	CDiffusedTexturedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse, XMFLOAT2 xmf2TexCoord) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse; m_xmf2TexCoord = xmf2TexCoord; }
	CDiffusedTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2 xmf2TexCoord = XMFLOAT2(0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse; m_xmf2TexCoord = xmf2TexCoord; }
	~CDiffusedTexturedVertex() { }
};



class CTreeVertex 
{
public :
	XMFLOAT3 m_xmf3Position;
	XMFLOAT2 m_xmf2Size;

	CTreeVertex() { 
		m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_xmf2Size = XMFLOAT2(0.0f, 0.0f);
	}

	CTreeVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2Size) {
		m_xmf3Position = xmf3Position;
		m_xmf2Size = xmf2Size;
	}

	virtual ~CTreeVertex() {};
};


class CIlluminatedTexturedVertex : public CDiffusedTexturedVertex
{
public:
	XMFLOAT3						m_xmf3Normal;
	UINT							m_xmfPartition;					//파티션 번호

public:
	CIlluminatedTexturedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f); }
	CIlluminatedTexturedVertex(float x, float y, float z, XMFLOAT3 xmf3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f)) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf3Normal = xmf3Normal; }
	CIlluminatedTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf3Normal = xmf3Normal; }
	~CIlluminatedTexturedVertex() { }
};

class CNormalMapTexturedVertex : public CIlluminatedTexturedVertex
{
public:
	XMFLOAT3						m_xmf3Tangent;
	XMFLOAT3						m_xmf3BiTangent;

public:
	CNormalMapTexturedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f); m_xmf3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf3Tangent = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf3BiTangent = XMFLOAT3(0.0f, 0.0f, 0.0f); }
	CNormalMapTexturedVertex(float x, float y, float z, XMFLOAT2 xmf2TexCoord, XMFLOAT3 xmf3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 xmf3Tangent = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 xmf3BiTangent = XMFLOAT3(0.0f, 0.0f, 0.0f)) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf3Normal = xmf3Normal; m_xmf3Tangent = xmf3Tangent; m_xmf3BiTangent = xmf3BiTangent; m_xmf2TexCoord = xmf2TexCoord; }
	CNormalMapTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2 xmf2TexCoord = XMFLOAT2(0.0f, 0.0f), XMFLOAT3 xmf3Tangent = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 xmf3BiTangent = XMFLOAT3(0.0f, 0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf3Normal = xmf3Normal; m_xmf3Tangent = xmf3Tangent; m_xmf3BiTangent = xmf3BiTangent; m_xmf2TexCoord = xmf2TexCoord; }
	~CNormalMapTexturedVertex() { }
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//메시는 이제 재지리이 
class CMesh
{
public:
    CMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
    virtual ~CMesh();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void ReleaseUploadBuffers();


protected:
	ID3D12Resource					*m_pd3dVertexBuffer = NULL;
	ID3D12Resource					*m_pd3dVertexUploadBuffer = NULL;

	ID3D12Resource					*m_pd3dIndexBuffer = NULL;
	ID3D12Resource					*m_pd3dIndexUploadBuffer = NULL;

	D3D12_VERTEX_BUFFER_VIEW		m_d3dVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW			m_d3dIndexBufferView;

	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT							m_nSlot = 0;
	UINT							m_nVertices = 0;
	UINT							m_nStride = 0;
	UINT							m_nOffset = 0;

	UINT							m_nIndices = 0;
	UINT							m_nStartIndex = 0;
	int								m_nBaseVertex = 0;

	BoundingOrientedBox			    m_xmBoundingBox;

public:
	BoundingOrientedBox				GetBoundingBox() { return (m_xmBoundingBox); }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, UINT nInstances);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, UINT nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CTriangleMeshDiffused : public CMesh
{
public:
    CTriangleMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
    virtual ~CTriangleMeshDiffused();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList);
};
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CCubeMeshDiffused : public CMesh
{
public:
	CCubeMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshDiffused();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CCubeMeshTextured : public CMesh
{
public:
	CCubeMeshTextured(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshTextured();
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CAirplaneMeshDiffused : public CMesh
{
public:
	CAirplaneMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f, XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CAirplaneMeshDiffused();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CHeightMapImage
{
private:
	//픽셀 배열 (2차원)
	// 만약 픽셀이 8비트이고 단위가 1m라면 최대 255m까지 밖에 표현 못한다.
	// 픽셀의 개수와 정점의 개수는 가급적이면 똑같이하는게 좋다. 그래야 1대 1 대응이 됨.
	BYTE						*m_pHeightMapPixels;


	int							m_nWidth; //가로 (x축)픽셀의 개수
	int							m_nLength;//세로 (y축) 픽셀의 개수
	XMFLOAT3					m_xmf3Scale;	//픽셀의 크기(해상도: 픽셀 사이의 거리)

public:
	CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);
	virtual ~CHeightMapImage(void);

	float GetHeight(float x, float z, bool bReverseQuad = false);
	XMFLOAT3 GetHeightMapNormal(int x, int z);
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }

	BYTE *GetHeightMapPixels() { return(m_pHeightMapPixels); }
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//class CHeightColorImage 
//{
//private:
//	XMFLOAT4					*m_pHeightMapColor;
//	int							m_nWidth; //가로 (x축)픽셀의 개수
//	int							m_nLength;//세로 (y축) 픽셀의 개수
//	XMFLOAT3					m_xmf3Scale;	//픽셀의 크기(해상도: 픽셀 사이의 거리)
//
//
//public:
//	CHeightColorImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);
//	virtual ~CHeightColorImage();
//
//
//	//XMFLOAT3 GetHeightMapNormal(int x, int z);
//	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
//
//	int GetHeightMapWidth() { return(m_nWidth); }
//	int GetHeightMapLength() { return(m_nLength); }
//
//};


//////////////////////////////////////////////////////////////////////////////////////////////////////


class CHeightMapGridMesh : public CMesh
{
protected:
	int							m_nWidth;
	int							m_nLength;
	XMFLOAT3					m_xmf3Scale;
	XMFLOAT3					m_xmf3Normal;
	UINT						m_TerrainInfo;

	
public:
	std::vector<XMFLOAT3> m_positionVector;
	
	CHeightMapGridMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, int xStart, int zStart, int nWidth, int nLength,XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), void *pContext=NULL);
	virtual ~CHeightMapGridMesh();

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	
	virtual float OnGetHeight(int x, int z, void *pContext);
	virtual XMFLOAT4 OnGetColor(int x, int z, void *pContext,int partitionX,int partitionZ);

public :
	static float **m_pixelHeight;
};



////////////////////////////////////////////////////////////////
//billboardMesh

class CTexturedRectMesh :public CMesh
{
public:
	CTexturedRectMesh(ID3D12Device *pd3dDevice,ID3D12GraphicsCommandList *pd3dCommandList,float fWidth,float fHeight,float fDepth,float fxPosition,float fyPosition,float fzPosition);
	virtual ~CTexturedRectMesh();
};


//////////////////////////////////////////////////////////////
//기하 셰이더로 만든 빌보드 
class CGeometeryBillboard : public CMesh
{
public:
	CGeometeryBillboard(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,int cxTerrain,int czTerrain ,int nTrees,void* pContext =NULL);
	virtual ~CGeometeryBillboard();
};

//////////////////////////////////////////////////////////////////
//해골 모델

class CSkullMesh : public CMesh
{
public:
	CSkullMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~CSkullMesh();
};


/////////////////////////////////////////////////////////////////
//자동차 모델
class CCarMesh : public CMesh
{
public:
	CCarMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~CCarMesh();
};

/////////////////////////////////////////////////////////////////////////////
class CFileMesh						//파일로 부터 모델 정보를 가져올 메시
{
public:
	CFileMesh() { }
	virtual ~CFileMesh() { }

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual void ReleaseUploadBuffers() { }

protected:
	D3D12_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT							m_nSlot = 0;
	UINT							m_nVertices = 0;
	UINT							m_nOffset = 0;

	UINT							m_nType = 0;

public:
	UINT GetType() { return(m_nType); }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList) { }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, int nSubSet) { }
};



/////////////////////////////////////////////////////////////////////////////
#define VERTEXT_POSITION			0x01
#define VERTEXT_COLOR				0x02
#define VERTEXT_NORMAL				0x04
#define VERTEXT_TANGENT				0x08
#define VERTEXT_TEXTURE_COORD0		0x10
#define VERTEXT_TEXTURE_COORD1		0x20

#define VERTEXT_TEXTURE					(VERTEXT_POSITION | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_DETAIL					(VERTEXT_POSITION | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)
#define VERTEXT_NORMAL_TEXTURE			(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_NORMAL_TANGENT_TEXTURE	(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TANGENT | VERTEXT_TEXTURE_COORD0)
#define VERTEXT_NORMAL_DETAIL			(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)
#define VERTEXT_NORMAL_TANGENT__DETAIL	(VERTEXT_POSITION | VERTEXT_NORMAL | VERTEXT_TANGENT | VERTEXT_TEXTURE_COORD0 | VERTEXT_TEXTURE_COORD1)

class CMeshLoadInfo
{
public:
	CMeshLoadInfo() { }
	~CMeshLoadInfo();

public:
	TCHAR							m_pstrMeshName[256] = { 0 };

	UINT							m_nType = 0x00;

	XMFLOAT3						m_xmf3AABBCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3						m_xmf3AABBExtents = XMFLOAT3(0.0f, 0.0f, 0.0f);

	int								m_nVertices = 0;
	XMFLOAT3						*m_pxmf3Positions = NULL;
	XMFLOAT4						*m_pxmf4Colors = NULL;
	XMFLOAT3						*m_pxmf3Normals = NULL;
	XMFLOAT3						*m_pxmf3Tangents = NULL;
	XMFLOAT3						*m_pxmf3BiNormals = NULL;
	XMFLOAT2						*m_pxmf2TextureCoords0 = NULL;
	XMFLOAT2						*m_pxmf2TextureCoords1 = NULL;

	int								m_nIndices = 0;
	UINT							*m_pnIndices = NULL;

	int								m_nSubMeshes = 0;
	int								*m_pnSubSetIndices = NULL;
	UINT							**m_ppnSubSetIndices = NULL;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class CMeshFromFile : public CFileMesh
{
public:
	CMeshFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CMeshLoadInfo *pMeshInfo);
	virtual ~CMeshFromFile();

public:
	virtual void ReleaseUploadBuffers();

protected:
	ID3D12Resource * m_pd3dPositionBuffer = NULL;
	ID3D12Resource					*m_pd3dPositionUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dPositionBufferView;

	int								m_nSubMeshes = 0;
	int								*m_pnSubSetIndices = NULL;
	ID3D12Resource					**m_ppd3dSubSetIndexBuffers = NULL;
	ID3D12Resource					**m_ppd3dSubSetIndexUploadBuffers = NULL;
	D3D12_INDEX_BUFFER_VIEW			*m_pd3dSubSetIndexBufferViews = NULL;

public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, int nSubSet);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMeshIlluminatedFromFile : public CMeshFromFile
{
public:
	CMeshIlluminatedFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CMeshLoadInfo *pMeshInfo);
	virtual ~CMeshIlluminatedFromFile();

	virtual void ReleaseUploadBuffers();

protected:
	ID3D12Resource * m_pd3dNormalBuffer = NULL;
	ID3D12Resource					*m_pd3dNormalUploadBuffer = NULL;
	D3D12_VERTEX_BUFFER_VIEW		m_d3dNormalBufferView;

public:
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, int nSubSet);
};
