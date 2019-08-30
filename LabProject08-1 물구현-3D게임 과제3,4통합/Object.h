//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"

class CExplosionBillboardObject;
#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20

#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05


class CShader;

struct CB_GAMEOBJECT_INFO
{
	XMFLOAT4X4						m_xmf4x4World;
	UINT							m_nMaterial;
	UINT							m_nTimeElapsed;
	UINT							m_nObjectID;
};



//인스턴스 정보(게임 객체의 월드 변환 행렬과 객체의 생성)을 위한 구조이다.
struct VS_VB_INSTANCE
{
	XMFLOAT4X4 m_xmf4x4Transform;
	//XMFLOAT4 m_xmColor;
	//XMFLOAT2 m_xmTexcoord;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct SRVROOTARGUMENTINFO
{
	UINT							m_nRootParameterIndex = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dSrvGpuDescriptorHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dUavGpuDescriptorHandle;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//struct CB_TEXTURE_COORD
//{
//	XMFLOAT4X4						m_xmf4x4Texcoord;
//};

class CTexture
{
public:
	CTexture(int nTextureResources = 1, UINT nResourceType = RESOURCE_TEXTURE2D, int nSamplers = 0);
	virtual ~CTexture();


private:
	int								m_nReferences = 0;

	UINT							m_nTextureType = RESOURCE_TEXTURE2D;
	int								m_nTextures = 0;	//텍스쳐도 배열의 원소의 개수만큼 가질 수 있다.
	
	ID3D12Resource					**m_ppd3dTextureUploadBuffers;

	int								m_nSamplers = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE		*m_pd3dSamplerGpuDescriptorHandles = NULL;

public:
	SRVROOTARGUMENTINFO				*m_pRootArgumentInfos = NULL;
	ID3D12Resource					**m_ppd3dTextures = NULL;
public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void SetRootArgumentUav(int nIndex, UINT nRootParaeterIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3duavGpuDescriptorHandle);
	void SetRootArgument(int nIndex, UINT nRootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dsrvGpuDescriptorHandle);
	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, int nIndex);
	void ReleaseShaderVariables();

	ID3D12Resource *CreateTexture(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, UINT nWidth, UINT nHeight, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE *pd3dClearValue, UINT nIndex);

	void LoadTextureFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, UINT nIndex);
	void LoadTextureFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, UINT nIndex, bool bIsDDSFile);
	int GetTextures() { return(m_nTextures); }
	ID3D12Resource *GetTexture(int nIndex) { return(m_ppd3dTextures[nIndex]); }
	UINT GetTextureType() { return(m_nTextureType); }

	void ReleaseUploadBuffers();
};



#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

class CGameObject;

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	//기본 색상
	XMFLOAT4						m_xmf4Albedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	UINT							m_nReflection = 0;
	CTexture						*m_pTexture = NULL;
	CShader							*m_pShader = NULL;

	void SetAlbedo(XMFLOAT4 xmf4Albedo) { m_xmf4Albedo = xmf4Albedo; }
	void SetTexture(CTexture *pTexture);
	void SetShader(CShader *pShader);
	void SetReflection(UINT nReflection) { m_nReflection = nReflection; }

	void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseShaderVariables();

	void ReleaseUploadBuffers();
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CGameObject
{
public:
	CGameObject(int nMeshes=1);
	virtual ~CGameObject();

public:


	bool m_bActive = true;
	XMFLOAT3	m_xmf3Position;
	XMFLOAT3	m_xmf3Right;
	XMFLOAT3	m_xmf3Up;
	XMFLOAT3	m_xmf3Look;
	XMFLOAT4X4						m_xmf4x4World;

	XMFLOAT4X4						m_xmf4x4Texcoord;			//텍스쳐 변환 행렬

	//이제 메시마다 재질을 다르게 해야하기 때문에 subset을 다르게 해야하는 재질의 개수만큼 subset이 필요하다
	//배치 프로세싱 개념
	//subset의 개념은 같은 재질을 사용해서 그려야할것 집합해놓은것
	CMesh							**m_ppMeshes;
	int								m_nMeshes;

	CMaterial						*m_pMaterial = NULL;

	D3D12_GPU_DESCRIPTOR_HANDLE		m_d3dCbvGPUDescriptorHandle;

	BoundingOrientedBox m_xmOOBB;
	BoundingOrientedBox m_xmOOBBTransformed;

	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) {
		m_xmOOBBTransformed = m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation);
	}

protected:
	ID3D12Resource					*m_pd3dcbGameObject = NULL;
	CB_GAMEOBJECT_INFO				*m_pcbMappedGameObject = NULL;

	

	UINT							m_terrainID;
public:
	//게임 객체가 보이는가를 결정한다.
	bool IsVisible(CCamera* pCamera = NULL);
	bool IsVisible(int nIndex,CCamera* pCamera = NULL);

	void SetMesh(int nIndex, CMesh *pMesh);
	void SetShader(CShader *pShader);
	void SetMaterial(CMaterial *pMaterial);


	void SetLook(XMFLOAT3 look);
	void SetRight(XMFLOAT3 right);
	void SetUp(XMFLOAT3 up);
	void SetCbvGPUDescriptorHandle(D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle) { m_d3dCbvGPUDescriptorHandle = d3dCbvGPUDescriptorHandle; }
	void SetCbvGPUDescriptorHandlePtr(UINT64 nCbvGPUDescriptorHandlePtr) { m_d3dCbvGPUDescriptorHandle.ptr = nCbvGPUDescriptorHandlePtr; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetCbvGPUDescriptorHandle() { return(m_d3dCbvGPUDescriptorHandle); }

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	
	virtual void Animate(float fTimeElapsed,CCamera *pCamera){}
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4 *pxmf4x4Parent) {}
	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender() { }
	
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, bool bSolid);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera,int objNum);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, UINT nInstance,int objNum);
	virtual void RenderInstancing(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, UINT nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView);

	virtual void BuildMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) { }
	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT3& pxmf3Axis, float fAngle);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CRotatingObject : public CGameObject
{
public:
	CRotatingObject(int nMeshes);
	virtual ~CRotatingObject();

protected:
	XMFLOAT3 m_xmf3RotationAxis;
	float	 m_fRotationSpeed;
public:
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(XMFLOAT3 xmf3RotationAxis) { m_xmf3RotationAxis = xmf3RotationAxis; }



	float GetRotationSpeed() { return m_fRotationSpeed; }
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4 *pxmf4x4Parent = NULL);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);
};


class CRevolvingObject : public CGameObject
{
public:
	CRevolvingObject(int nMeshes=1);
	virtual ~CRevolvingObject();

private:
	XMFLOAT3						m_xmf3RevolutionAxis;
	float							m_fRevolutionSpeed;

public:
	void SetRevolutionSpeed(float fRevolutionSpeed) { m_fRevolutionSpeed = fRevolutionSpeed; }
	void SetRevolutionAxis(XMFLOAT3 xmf3RevolutionAxis) { m_xmf3RevolutionAxis = xmf3RevolutionAxis; }

	virtual void Animate(float fTimeElapsed);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CMissileObject : public CRotatingObject
{
public:
	CMissileObject(int nMeshes);
	virtual ~CMissileObject();

	//XMFLOAT3	m_xmf3Position;
	//XMFLOAT3	m_xmf3Right;
	//XMFLOAT3	m_xmf3Up;
	//XMFLOAT3	m_xmf3Look;

	bool	m_fired = false;
	bool    m_crashed = false;				//충돌해서 폭발했는지 여부

	float	m_fElapsedTimes = 0.0f;				//발사되고 폭발할때까지 재는 타이머
	float   m_fExplosionTimes = 0.0f;			//폭발하고부터 재는 타이머
	//CFrameObject *pTarget = NULL;

	XMFLOAT3	m_xmf3MovingDirection;
	float		m_fMovingSpeed;
	float		m_fMovingRange;

	static enum eStatus { SHOT = 0, CRASH, SMOKE, IDLE };

	int status = SHOT;

	CExplosionBillboardObject *pExplosiveObject = NULL;
public:


	float	GetTargetDistance(XMFLOAT3	&target);

	void    SetCrash();
	void    SetFire();


	void	RotateMissile(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void	SetMovingDirection(XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection); }
	void	SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void	SetMovingRange(float fRange) { m_fMovingRange = fRange; }
	void	SetUpVector(XMFLOAT3 &upVector) { this->m_xmf3Up = upVector; }

	void	SetRightVector(XMFLOAT3 &rightVector) { this->m_xmf3Right = rightVector; }

	void	Move(XMFLOAT3 &vDirection, float fSpeed);


	virtual void Animate(float fElapsedTime, CCamera *pCamera);

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera, UINT RasterNum);
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);


	virtual void ReleaseUploadBuffers();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CHeightMapTerrain : public CGameObject
{
public:
	CHeightMapTerrain(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color,UINT id);
	virtual ~CHeightMapTerrain();

private:
	CHeightMapImage					*m_pHeightMapImage;

	int								m_nWidth;
	int								m_nLength;

	XMFLOAT3						m_xmf3Scale;
	//UINT							m_terrainID;
public:
	float GetHeight(float x, float z, bool bReverseQuad = false) { return(m_pHeightMapImage->GetHeight(x, z, bReverseQuad) * m_xmf3Scale.y); } //World
	XMFLOAT3 GetNormal(float x, float z) { return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z))); }

	int GetHeightMapWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMapImage->GetHeightMapLength()); }

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }


	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
};


class CWaterTerrain : public CGameObject
{
public:
	CWaterTerrain(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color, UINT id);
	virtual ~CWaterTerrain();

private:
	CHeightMapImage					*m_pHeightMapImage;

	int								m_nWidth;
	int								m_nLength;

	XMFLOAT3						m_xmf3Scale;
	//UINT							m_terrainID;
public:

	float GetHeight(float x, float z, bool bReverseQuad = false) { return(m_pHeightMapImage->GetHeight(x, z, bReverseQuad) * m_xmf3Scale.y); } //World
	XMFLOAT3 GetNormal(float x, float z) { return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z))); }

	int GetHeightMapWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMapImage->GetHeightMapLength()); }

	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }


	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);

};

class CSkyBox : public CGameObject
{
public:
	CSkyBox(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRoogSignature);
	virtual ~CSkyBox();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);

};
class CFrameMaterial;

class CFrameObject
{
private:
	int								m_nReferences = 0;

public:
	void AddRef();
	void Release();

protected:
	XMFLOAT3 m_xmf3RotationAxis;
	float m_fRotationSpeed;
public:

	CFrameObject();
	CFrameObject(int nMaterials);
	virtual ~CFrameObject();

public:
	char							m_pstrFrameName[64];

	bool m_bActive = true;
	CFileMesh							*m_pMesh = NULL;
	/*BoundingOrientedBox m_xmOOBB;
	BoundingOrientedBox m_xmOOBBTransformed;*/

	BoundingSphere *m_sphereBox;			//반경 50m구체 충돌 박스
	int								m_nMaterials = 0;
	CFrameMaterial						**m_ppMaterials = NULL;

	XMFLOAT4X4						m_xmf4x4Transform;
	XMFLOAT4X4						m_xmf4x4World;

	CFrameObject 					*m_pParent = NULL;
	CFrameObject 					*m_pChild = NULL;
	CFrameObject 					*m_pSibling = NULL;

	bool							m_bBlowingUp = false;

	XMFLOAT3	m_xmf3MovingDirection;
	float		m_fMovingSpeed = 0.0f;
	float		m_fMovingRange = 0.0f;

	static enum state { LIVE = 0, EXPLODE, DIMINISH };
	int status = LIVE;

	void SetSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }

	void SetMesh(CFileMesh *pMesh);
	void SetShader(CShader *pShader);
	void SetShader(int nMaterial, CShader *pShader);
	void SetMaterial(int nMaterial, CFrameMaterial *pMaterial);

	/*void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) {
		m_xmOOBBTransformed = m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation);
	}*/


	void SetChild(CFrameObject *pChild);

	virtual void BuildMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) { }

	virtual void PrepareAnimate() { }
	virtual void Animate(float fTimeElapsed, CPlayer *pPlayer, XMFLOAT4X4 *pxmf4x4Parent){}
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4 *pxmf4x4Parent=NULL);

	virtual void OnPrepareRender() { }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL,CPlayer *pPlayer =NULL);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, CFrameMaterial *pMaterial);

	virtual void ReleaseUploadBuffers();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetScale(float x, float y, float z);

	void Move(XMFLOAT3& vDirection, float fSpeed);
	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);


	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4 *pxmf4Quaternion);


	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection); }


	float GetRotationSpeed() { return m_fRotationSpeed; }
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(XMFLOAT3 xmf3RotationAxis) { m_xmf3RotationAxis = xmf3RotationAxis; }

	CFrameObject *GetParent() { return(m_pParent); }
	void UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent = NULL);
	CFrameObject *FindFrame(char *pstrFrameName);

	CTexture *FindReplicatedTexture(_TCHAR *pstrTextureName);
	CTexture *FindRootAndReplicatedTexture(_TCHAR *pstrTextureName, CFrameObject *pParent);

	UINT GetMeshType() { return((m_pMesh) ? m_pMesh->GetType() : 0x00); }

public:
	void LoadMaterialsFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, CFrameObject *pParent, FILE *pInFile, CShader *pShader);

	static CFrameObject *LoadFrameHierarchyFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, CFrameObject *pParent, FILE *pInFile, CShader *pShader);
	static CFrameObject *LoadGeometryFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, char *pstrFileName, CShader *pShader);

	static void PrintFrameInfo(CFrameObject *pGameObject, CFrameObject *pParent);
};

class CFrameMaterial
{
public:
	CFrameMaterial(int nTextures);
	virtual ~CFrameMaterial();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	CShader							*m_pShader = NULL;

	XMFLOAT4						m_xmf4AlbedoColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4						m_xmf4EmissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4SpecularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4AmbientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	void SetShader(CShader *pShader);
	void SetMaterialType(UINT nType) { m_nType |= nType; }
	void SetTexture(CTexture *pTexture, UINT nTexture = 0);

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList);

	virtual void ReleaseUploadBuffers();

public:
	UINT							m_nType = 0x00;

	float							m_fGlossiness = 0.0f;
	float							m_fSmoothness = 0.0f;
	float							m_fSpecularHighlight = 0.0f;
	float							m_fMetallic = 0.0f;
	float							m_fGlossyReflection = 0.0f;

public:
	int 							m_nTextures = 0;
	_TCHAR(*m_ppstrTextureNames)[64] = NULL;
	CTexture						**m_ppTextures = NULL; //0:Albedo, 1:Specular, 2:Metallic, 3:Normal, 4:Emission, 5:DetailAlbedo, 6:DetailNormal

	void LoadTextureFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, UINT nType, UINT nRootParameter, _TCHAR *pwstrTextureName, CTexture **ppTexture, CFrameObject *pParent, FILE *pInFile, CShader *pShader);
};


class CSuperCobraObject : public CFrameObject
{
public:
	CSuperCobraObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~CSuperCobraObject();

private:
	CFrameObject					*m_pMainRotorFrame = NULL;
	CFrameObject					*m_pTailRotorFrame = NULL;

public:
	virtual void PrepareAnimate();
	virtual void Animate(float fTimeElapsed,CPlayer *pPlayer, XMFLOAT4X4 *pxmf4x4Parent = NULL);
};

class CGunshipObject : public CFrameObject
{
public:
	CGunshipObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~CGunshipObject();

private:
	CFrameObject					*m_pMainRotorFrame = NULL;
	CFrameObject					*m_pTailRotorFrame = NULL;

public:
	virtual void PrepareAnimate();
	virtual void Animate(float fTimeElapsed,CPlayer *pPlayer, XMFLOAT4X4 *pxmf4x4Parent = NULL);
};

class CMi24Object : public CFrameObject
{
public:
	CMi24Object(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual ~CMi24Object();

private:
	CFrameObject					*m_pMainRotorFrame = NULL;
	CFrameObject					*m_pTailRotorFrame = NULL;

public:
	virtual void PrepareAnimate();
	virtual void Animate(float fTimeElapsed,CPlayer *pPlayer, XMFLOAT4X4 *pxmf4x4Parent = NULL);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////